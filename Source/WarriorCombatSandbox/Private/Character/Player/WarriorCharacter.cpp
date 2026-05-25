// Copyright Epic Games, Inc. All Rights Reserved.


#include "Character/Player/WarriorCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "WarriorCombatSandbox/WarriorCombatSandbox.h"

DEFINE_LOG_CATEGORY(LogWarriorCharacter);

// Sets default values
AWarriorCharacter::AWarriorCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// These variables are set in the Blueprint, but we initialize them here to avoid null pointer errors
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Attach combat and health components
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}


// Called to bind functionality to input
void AWarriorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AWarriorCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AWarriorCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWarriorCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AWarriorCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWarriorCharacter::Look);

		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AWarriorCharacter::Sprint);

		// Combat
		EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Triggered, this, &AWarriorCharacter::DoAttack);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &AWarriorCharacter::HandleHeavyAttack);
		EnhancedInputComponent->BindAction(DefenseSkillAction, ETriggerEvent::Triggered, this, &AWarriorCharacter::HandleDefenseSkill);
		EnhancedInputComponent->BindAction(InterruptAction, ETriggerEvent::Triggered, this, &AWarriorCharacter::HandleInterrupt);
		EnhancedInputComponent->BindAction(UltimateAction, ETriggerEvent::Triggered, this, &AWarriorCharacter::HandleUltimate);
	}
	else
	{
		UE_LOG(LogWarriorCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}

void AWarriorCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HUDClass)
	{
		PlayerHUD = CreateWidget<UPlayerHUD>(GetWorld(), HUDClass);

		if (PlayerHUD)
		{
			PlayerHUD->AddToViewport();
		}
		else
		{
			UE_LOG(LogWarriorCharacter, Error, TEXT("'%s' Failed to create Player HUD!"), *GetNameSafe(this));
		}
	}

	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &AWarriorCharacter::HandleHealthChanged);
	}
	else
	{
		UE_LOG(LogWarriorCharacter, Error, TEXT("'%s' Failed to find Health Component!"), *GetNameSafe(this));
	}

	if (CombatComponent)
	{
		CombatComponent->OnRageChanged.AddDynamic(this, &AWarriorCharacter::HandleRageChanged);
	}
	else
	{
		UE_LOG(LogWarriorCharacter, Error, TEXT("'%s' Failed to find Combat Component!"), *GetNameSafe(this));
	}

	// Update the HUD with the initial health values
	if (PlayerHUD && HealthComponent)
	{
		PlayerHUD->UpdateHealth(HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth());
	}

	// Set up weapon
	if (WeaponClass)
	{
		EquippedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);

		if (EquippedWeapon)
		{
			EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
		}
		else
		{
			UE_LOG(LogWarriorCharacter, Error, TEXT("'%s' Failed to spawn equipped weapon!"), *GetNameSafe(this));
		}

		if (CombatComponent)
		{
			CombatComponent->SetEquippedWeapon(EquippedWeapon);
		}
		else
		{
			UE_LOG(LogWarriorCharacter, Error, TEXT("'%s' Failed to find Combat Component to set equipped weapon!"), *GetNameSafe(this));
		}
	}
}

void AWarriorCharacter::Move(const FInputActionValue& Value)
{
	// Input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// Route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AWarriorCharacter::Look(const FInputActionValue& Value)
{
	// Input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// Route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AWarriorCharacter::Sprint(const FInputActionValue& Value)
{
	// stub
}

void AWarriorCharacter::HandleBasicAttack()
{
	if (CombatComponent && BasicAttackData)
	{
		CombatComponent->TryUseAbility(BasicAttackData);
	}
}

void AWarriorCharacter::HandleHeavyAttack()
{
	if (CombatComponent && HeavyAttackData)
	{
		CombatComponent->TryUseAbility(HeavyAttackData);
	}
}

void AWarriorCharacter::HandleDefenseSkill()
{
	if (CombatComponent && DefenseSkillData)
	{
		CombatComponent->TryUseAbility(DefenseSkillData);
	}
}

void AWarriorCharacter::HandleInterrupt()
{
	if (CombatComponent && InterruptData)
	{
		CombatComponent->TryUseAbility(InterruptData);
	}
}

void AWarriorCharacter::HandleUltimate()
{
	if (CombatComponent && UltimateData)
	{
		CombatComponent->TryUseAbility(UltimateData);
	}
}

void AWarriorCharacter::HandleHealthChanged(float Current, float Max)
{
	if (PlayerHUD)
	{
		PlayerHUD->UpdateHealth(Current, HealthComponent->GetMaxHealth());
	}
}

void AWarriorCharacter::HandleRageChanged(float Current, float Max)
{
	if (PlayerHUD)
	{
		PlayerHUD->UpdateRage(Current, CombatComponent->GetMaxRage());
	}
}

void AWarriorCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward and right vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement in that direction
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AWarriorCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AWarriorCharacter::DoJumpStart()
{
	// Signal the character to jump
	Jump();
}

void AWarriorCharacter::DoJumpEnd()
{
	// Signal the character to stop jumping
	StopJumping();
}

void AWarriorCharacter::DoAttack()
{
	HandleBasicAttack();
}
