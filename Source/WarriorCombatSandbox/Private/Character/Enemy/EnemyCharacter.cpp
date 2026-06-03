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

	// Enemy health bar widget
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CachedCameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	if (HealthBarWidget)
	{
		UEnemyHealthBar* HealthBar = Cast<UEnemyHealthBar>(HealthBarWidget->GetUserWidgetObject());
		CachedHealthBar = HealthBar;
	}

	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &AEnemyCharacter::HandleHealthChanged);
		HealthComponent->OnDeath.AddDynamic(this, &AEnemyCharacter::HandleDeath);
	}

	if (CachedHealthBar && HealthComponent)
	{
		CachedHealthBar->UpdateHealthBar(HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth());
	}

	if (CachedHealthBar)
	{
		CachedHealthBar->SetVisibility(ESlateVisibility::Hidden);
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

	if (!HealthBarWidget) return;

	if (!CachedCameraManager) return;

	HealthBarWidget->SetWorldRotation((CachedCameraManager->GetCameraLocation() - HealthBarWidget->GetComponentLocation()).Rotation());
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
		HealthComponent->ApplyDamage(DamageAmount);
		UE_LOG(LogEnemyCharacter, Warning, TEXT("Enemy received %f damage, current health: %f"), DamageAmount, HealthComponent->GetCurrentHealth());
	}
}

void AEnemyCharacter::HandleHealthChanged(float Current, float Max)
{
	if (CachedHealthBar)
	{
		CachedHealthBar->SetVisibility(ESlateVisibility::Visible);
		CachedHealthBar->UpdateHealthBar(Current, HealthComponent->GetMaxHealth());

		GetWorldTimerManager().SetTimer(HideHealthBarTimer, this, &AEnemyCharacter::HideHealthBar, 3.5f, false);
	}
}

void AEnemyCharacter::HideHealthBar()
{
	CachedHealthBar->SetVisibility(ESlateVisibility::Hidden);
}

void AEnemyCharacter::BasicAttack()
{
	if (CombatComponent && !bIsDead)
	{
		CombatComponent->TryUseAbility_AI(BasicAttackData);
	}
}

void AEnemyCharacter::HandleDeath()
{
	bIsDead = true;

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

