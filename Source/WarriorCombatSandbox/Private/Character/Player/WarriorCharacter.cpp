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
#include "Combat/AbilityInputSystem.h"
#include "InputActionValue.h"
#include "Character/Enemy/EnemyCharacter.h"
#include "WarriorCombatSandbox/WarriorCombatSandbox.h"
#include <Kismet/GameplayStatics.h>

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

	// Create ability system component
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
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

		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AWarriorCharacter::DoInteract);

		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AWarriorCharacter::Sprint);

		// Combat
		EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Triggered, this, &AWarriorCharacter::InputBasicAttack);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &AWarriorCharacter::InputHeavyAttack);
		EnhancedInputComponent->BindAction(AbilitySlot1Action, ETriggerEvent::Triggered, this, &AWarriorCharacter::InputSlot1);
		EnhancedInputComponent->BindAction(AbilitySlot2Action, ETriggerEvent::Triggered, this, &AWarriorCharacter::InputSlot2);
		EnhancedInputComponent->BindAction(AbilitySlot3Action, ETriggerEvent::Triggered, this, &AWarriorCharacter::InputSlot3);
	}
	else
	{
		UE_LOG(LogWarriorCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}

void AWarriorCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentCheckpoint = GetActorTransform();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
		if (!PlayerHUD)
		{
			UE_LOG(LogWarriorCharacter, Error, TEXT("'%s' Failed to find Player HUD!"), *GetNameSafe(this));
		}
		else
		{
			UE_LOG(LogWarriorCharacter, Log, TEXT("'%s' Successfully found Player HUD!"), *GetNameSafe(this));
		}
	}
	else
	{
		UE_LOG(LogWarriorCharacter, Error, TEXT("'%s' Failed to find Player Controller!"), *GetNameSafe(this));
	}

	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &AWarriorCharacter::HandleHealthChanged);
		HealthComponent->OnDeath.AddDynamic(this, &AWarriorCharacter::HandleDeath);
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

void AWarriorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update interaction target every 0.05 seconds to avoid doing expensive line traces every frame
	InteractionUpdateTimer += DeltaTime;
	if (InteractionUpdateTimer >= 0.05f)
	{
		UpdateInteractionTarget();
		InteractionUpdateTimer = 0.f;
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

void AWarriorCharacter::InputBasicAttack()
{
	DoAttack(EAbilityInput::BasicAttack);
}

void AWarriorCharacter::InputHeavyAttack()
{
	DoAttack(EAbilityInput::HeavyAttack);
}

void AWarriorCharacter::InputSlot1()
{
	DoAttack(EAbilityInput::Slot1);
}

void AWarriorCharacter::InputSlot2()
{
	DoAttack(EAbilityInput::Slot2);
}

void AWarriorCharacter::InputSlot3()
{
	DoAttack(EAbilityInput::Slot3);
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

void AWarriorCharacter::HandleDeath()
{
	bIsDead = true;

	// Disable input
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->DisableInput(PlayerController);
	}

	// Disable collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	// Play death animation
	if (DeathMontage)
	{
		UAnimInstance* Anim = GetMesh()->GetAnimInstance();
		if (Anim)
		{
			Anim->Montage_Play(DeathMontage);
		}
	}

	OnPlayerDeath();
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

void AWarriorCharacter::DoAttack(EAbilityInput InputType)
{
	if (!AbilitySystem)
	{
		UE_LOG(LogWarriorCharacter, Error, TEXT("'%s' Failed to find Ability System Component!"), *GetNameSafe(this));
		return;
	}

	AbilitySystem->HandleInput(InputType);
}

void AWarriorCharacter::DoInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("DoInteract fired"));


	if (ActiveInteractable && ActiveInteractable->CanInteract())
	{
		ActiveInteractable->Interact();
	}
}

void AWarriorCharacter::RespawnPlayer()
{
	bIsDead = false;

	if (HealthComponent)
		HealthComponent->ResetHealth();

	if (CombatComponent)
		CombatComponent->ResetRage();

	SetActorTransform(CurrentCheckpoint);

	// Re-enable input
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->EnableInput(PlayerController);
	}

	// Re-enable collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// Re-enable movement
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	// Update the HUD with the reset health and rage values
	if (PlayerHUD)
	{
		PlayerHUD->UpdateHealth(HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth());
		PlayerHUD->UpdateRage(CombatComponent->GetCurrentRage(), CombatComponent->GetMaxRage());
	}

	// Reset enemies
	TArray<AActor*> EnemyActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), EnemyActors);

	for (AActor* Actor : EnemyActors)
	{
		if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Actor))
		{
			Enemy->ResetEnemy();
		}
	}
}

void AWarriorCharacter::UpdateCheckpoint(const FTransform& NewCheckpoint)
{
	CurrentCheckpoint = NewCheckpoint;
}

void AWarriorCharacter::UpdateInteractionTarget()
{
	FVector Start = GetActorLocation() + FVector(0.f, 0.f, InteractHeight); // Start from the character's chest level
	FVector End = Start + (GetActorForwardVector() * InteractRange);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel1, Params);

	AInteractableBase* NewTarget = nullptr;

	if (bHit)
	{
		AInteractableBase* HitInteractable = Cast<AInteractableBase>(Hit.GetActor());
		if (HitInteractable && NearbyInteractables.Contains(HitInteractable))
		{
			NewTarget = HitInteractable;
		}
	}

	// Handle focus switching
	if (NewTarget != ActiveInteractable)
	{
		if (ActiveInteractable)
			ActiveInteractable->OnUnfocus();

		ActiveInteractable = NewTarget;

		if (ActiveInteractable)
			ActiveInteractable->OnFocus();
	}

	// Debug line to visualize interaction ray
#if WITH_EDITOR
	FColor LineColor = bHit ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), Start, End, LineColor, false, 0.1f, 0, 1.f);
#endif
}

void AWarriorCharacter::AddNearbyInteractable(AInteractableBase* Interactable)
{
	NearbyInteractables.AddUnique(Interactable);
	UE_LOG(LogTemp, Warning, TEXT("Nearby added: %s"), *Interactable->GetName());

}

void AWarriorCharacter::RemoveNearbyInteractable(AInteractableBase* Interactable)
{
	NearbyInteractables.Remove(Interactable);

	// If the one leaving range was the active one, clear it
	if (ActiveInteractable == Interactable)
	{
		ActiveInteractable->OnUnfocus();
		ActiveInteractable = nullptr;
	}
}
