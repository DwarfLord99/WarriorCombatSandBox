// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogEnemyCharacter);

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Attach combat and health components
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	// Set up health bar widget
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(RootComponent);

	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidget->SetDrawSize(FVector2D(150.f, 20.f));
	HealthBarWidget->SetVisibility(true);

	HealthBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 220.f));

	// Set up rage bar widget
	RageBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("RageBarWidget"));
	RageBarWidget->SetupAttachment(RootComponent);

	RageBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	RageBarWidget->SetDrawSize(FVector2D(150.f, 20.f));
	RageBarWidget->SetVisibility(true);

	RageBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 120.f));

	// Set up cast bar widget
	CastBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("CastBarWidget"));
	CastBarWidget->SetupAttachment(RootComponent);

	CastBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	CastBarWidget->SetDrawSize(FVector2D(150.f, 20.f));
	CastBarWidget->SetVisibility(true);

	CastBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 170.f));
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CachedCameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	// Health bar cache
	if (HealthBarWidget)
	{
		if (UUserWidget* Widget = HealthBarWidget->GetUserWidgetObject())
		{
			CachedHealthBar = Cast<UEnemyHealthBar>(Widget);
			if (!CachedHealthBar)
			{
				UE_LOG(LogEnemyCharacter, Error, TEXT("'%s' Failed to cast health bar widget to UEnemyHealthBar!"), *GetNameSafe(this));
			}
		}
		else
		{
			UE_LOG(LogEnemyCharacter, Error, TEXT("'%s' Failed to get user widget object from health bar widget component!"), *GetNameSafe(this));
		}
	}	

	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &AEnemyCharacter::HandleHealthChanged);
		HealthComponent->OnDeath.AddDynamic(this, &AEnemyCharacter::HandleDeath);
	}
	else
	{
		UE_LOG(LogEnemyCharacter, Error, TEXT("'%s' Failed to find Health Component to bind health changed and death events!"), *GetNameSafe(this));
	}

	if (CachedHealthBar && HealthComponent)
	{
		CachedHealthBar->UpdateHealthBar(HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth());
		CachedHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UE_LOG(LogEnemyCharacter, Error, TEXT("'%s' Failed to cache health bar or health component!"), *GetNameSafe(this));
	}

	// Rage bar cache
	if (RageBarWidget)
	{
		if (UUserWidget* Widget = RageBarWidget->GetUserWidgetObject())
		{
			CachedRageBar = Cast<UEnemyRageBar>(Widget);
			if (!CachedRageBar)
			{
				UE_LOG(LogEnemyCharacter, Error, TEXT("'%s' Failed to cast rage bar widget to UEnemyRageBar!"), *GetNameSafe(this));
			}
		}
		else
		{
			UE_LOG(LogEnemyCharacter, Error, TEXT("'%s' Failed to get user widget object from rage bar widget component!"), *GetNameSafe(this));
		}
	}

	if (CombatComponent)
	{
		CombatComponent->OnRageChanged.AddDynamic(this, &AEnemyCharacter::HandleRageChanged);
	}
	else
	{
		UE_LOG(LogEnemyCharacter, Error, TEXT("'%s' Failed to find Combat Component to bind rage changed event!"), *GetNameSafe(this));
	}

	if (CachedRageBar && CombatComponent)
	{
		CachedRageBar->UpdateRageBar(CombatComponent->GetCurrentRage(), CombatComponent->GetMaxRage());
		CachedRageBar->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UE_LOG(LogEnemyCharacter, Error, TEXT("'%s' Failed to cache rage bar or combat component!"), *GetNameSafe(this));
	}

	// Cast bar cache
	if (CastBarWidget)
	{
		if (UUserWidget* Widget = CastBarWidget->GetUserWidgetObject())
		{
			CachedCastBar = Cast<UEnemyCastBar>(Widget);
			if (!CachedCastBar)
			{
				UE_LOG(LogEnemyCharacter, Error, TEXT("'%s' Failed to cast cast bar widget to UEnemyCastBar!"), *GetNameSafe(this));
			}
		}
		else
		{
			UE_LOG(LogEnemyCharacter, Error, TEXT("'%s' Failed to get user widget object from cast bar widget component!"), *GetNameSafe(this));
		}
	}

	if (CachedCastBar)
	{
		CachedCastBar->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UE_LOG(LogEnemyCharacter, Error, TEXT("'%s' Failed to cache cast bar!"), *GetNameSafe(this));
	}

	if (CombatComponent)
	{
		CombatComponent->OnBeginCast.AddDynamic(this, &AEnemyCharacter::HandleBeginCast);
		CombatComponent->OnFinishCast.AddDynamic(this, &AEnemyCharacter::HandleFinishCast);
		CombatComponent->OnInterruptCast.AddDynamic(this, &AEnemyCharacter::HandleInterruptCast);
	}
	else
	{
		UE_LOG(LogEnemyCharacter, Error, TEXT("'%s' Failed to find Combat Component to bind cast started and finished events!"), *GetNameSafe(this));
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
			UE_LOG(LogEnemyCharacter, Error, TEXT("'%s' Failed to spawn equipped weapon!"), *GetNameSafe(this));
		}

		if (CombatComponent)
		{
			CombatComponent->SetEquippedWeapon(EquippedWeapon);
		}
		else
		{
			UE_LOG(LogEnemyCharacter, Error, TEXT("'%s' Failed to find Combat Component to set equipped weapon!"), *GetNameSafe(this));
		}
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CachedCameraManager) return;

	// Rotate health bar to face camera
	if (HealthBarWidget && CachedCameraManager)
	{
		const FRotator LookAt = (CachedCameraManager->GetCameraLocation() - HealthBarWidget->GetComponentLocation()).Rotation();
		HealthBarWidget->SetWorldRotation(LookAt);
	}

	// Update cast bar percent
	if (CastBarWidget && CachedCastBar && CombatComponent && CombatComponent->IsCasting())
	{
		float CastPercent = CombatComponent->GetCastPercent();
		CachedCastBar->UpdateCastBar(CastPercent);

		// Rotate cast bar to face camera
		const FRotator LookAt = (CachedCameraManager->GetCameraLocation() - CastBarWidget->GetComponentLocation()).Rotation();

		CastBarWidget->SetWorldRotation(LookAt);
	}

	// Allows enemy to attack once they stop chasing player
	bCanAttack = GetVelocity().IsZero();
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::RecieveDamage_Implementation(float DamageAmount)
{
	if (HealthComponent && !bIsDead)
	{
		HealthComponent->ApplyDamage(DamageAmount, CombatComponent->CurrentAbilityData);
		UE_LOG(LogEnemyCharacter, Warning, TEXT("Enemy received %f damage, current health: %f"), DamageAmount, HealthComponent->GetCurrentHealth());
	}
}

void AEnemyCharacter::UpdateWalkSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void AEnemyCharacter::HandleHealthChanged(float Current, float Max)
{
	if (CachedHealthBar)
	{
		CachedHealthBar->UpdateHealthBar(Current, HealthComponent->GetMaxHealth());
		CachedHealthBar->SetVisibility(ESlateVisibility::Visible);
		CachedRageBar->SetVisibility(ESlateVisibility::Visible);

		UE_LOG(LogTemp, Warning, TEXT("Enemy Health Changed: %f / %f"), Current, HealthComponent->GetMaxHealth());
	}

	// Reset health bar hide timer
	GetWorld()->GetTimerManager().ClearTimer(HealthBarHideTimer);
	GetWorld()->GetTimerManager().SetTimer(HealthBarHideTimer, this, &AEnemyCharacter::HideHealthBar, 3.f, false);
}

void AEnemyCharacter::HandleRageChanged(float Current, float Max)
{
	if (CachedRageBar)
	{
		CachedRageBar->UpdateRageBar(Current, CombatComponent->GetMaxRage());
		UE_LOG(LogTemp, Warning, TEXT("Enemy Rage Changed: %f / %f"), Current, CombatComponent->GetMaxRage());
	}
}

void AEnemyCharacter::HandleBeginCast()
{
	if (CachedCastBar)
	{
		CachedCastBar->SetVisibility(ESlateVisibility::Visible);
		CachedCastBar->UpdateCastBar(0.f);
	}
}

void AEnemyCharacter::HandleFinishCast()
{
	if (CachedCastBar)
	{
		CachedCastBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AEnemyCharacter::HandleInterruptCast()
{
	if (CachedCastBar)
	{
		CachedCastBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AEnemyCharacter::HideHealthBar()
{
	if (CachedHealthBar)
	{
		CachedHealthBar->SetVisibility(ESlateVisibility::Hidden);
		CachedRageBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool AEnemyCharacter::CanUseAbility(UAbilityData* AbilityData)
{
	if (CombatComponent && !bIsDead)
	{
		return CombatComponent->CanUseAbility(AbilityData);
	}
	else
	{
		return false;
	}
}

void AEnemyCharacter::HandleAttack(UAbilityData* AbilityData)
{
	if (CombatComponent && !bIsDead && bCanAttack)
	{
		CombatComponent->TryUseAbility_AI(AbilityData);
	}
}

void AEnemyCharacter::HandleDeath()
{
	bIsDead = true;

	// Hide health bar
	if (CachedHealthBar)
	{
		CachedHealthBar->SetVisibility(ESlateVisibility::Hidden);
		CachedRageBar->SetVisibility(ESlateVisibility::Hidden);
	}


	// Disable collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	UE_LOG(LogEnemyCharacter, Warning, TEXT("Enemy died"));

	// Play death montage
	if (DeathMontage)
	{
		UAnimInstance* Anim = GetMesh()->GetAnimInstance();
		if (Anim)
		{
			Anim->Montage_Play(DeathMontage);
		}
	}

	// Despawn after delay
	SetLifeSpan(4.f);

	// Destroy weapon after delay
	if (EquippedWeapon)
	{
		EquippedWeapon->SetLifeSpan(4.f);
	}
}

