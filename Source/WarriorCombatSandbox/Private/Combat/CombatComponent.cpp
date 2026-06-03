// Copyright Epic Games, Inc. All Rights Reserved.

#include "Combat/CombatComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Components/HealthComponent.h"
#include "Animation/AnimInstance.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentRage = 0.f;
	OnRageChanged.Broadcast(CurrentRage, 0.f);	
}

void UCombatComponent::TryUseAbility(EAbilityInput InputType, UAbilityData* AbilityData)
{
	if (!AbilityData)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryUseAbility called with null AbilityData"));
		return;
	}

	// Cooldown check
	if (IsAbilityOnCooldown(InputType, AbilityData))
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability is on cooldown"));
		return;
	}

	// Resource and state checks
	if (!CanUseAbility(AbilityData))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot use ability: %s"), *AbilityData->GetName());
		return;
	}

	// Then call the shared version
	ExecuteAbility(AbilityData);

	// If successful, record cooldown
	LastUsedTime.Add(InputType, GetWorld()->GetTimeSeconds());
}

void UCombatComponent::TryUseAbility_AI(UAbilityData* AbilityData)
{
	if (!AbilityData)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryUseAbility called with null AbilityData"));
		return;
	}

	if (!CanUseAbility(AbilityData))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot use ability: %s"), *AbilityData->GetName());
		return;
	}

	ExecuteAbility(AbilityData);
}

bool UCombatComponent::CanUseAbility(UAbilityData* AbilityData)
{
	if (CurrentRage < AbilityData->RageCost)
		return false;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter && OwnerCharacter->GetCharacterMovement()->IsFalling())
		return false;

	return true;
}

void UCombatComponent::ExecuteAbility(UAbilityData* AbilityData)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter || !AbilityData->AttackMontage || bIsAttacking)
		return;

	bIsAttacking = true;
	CurrentAbilityData = AbilityData;

	// Rage cost
	CurrentRage -= AbilityData->RageCost;
	CurrentRage = FMath::Clamp(CurrentRage, 0.f, MaxRage);
	OnRageChanged.Broadcast(CurrentRage, AbilityData->RageCost);

	// Play montage
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		OwnerCharacter->GetCharacterMovement()->DisableMovement();

		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindUObject(this, &UCombatComponent::OnAttackMontageEnded);

		AnimInstance->Montage_Play(AbilityData->AttackMontage);
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AbilityData->AttackMontage);
	}
}

float UCombatComponent::GetCooldownRemaining(EAbilityInput InputType, const UAbilityData* AbilityData) const
{
	if (!AbilityData)
		return 0.f;

	const float* LastTimePtr = LastUsedTime.Find(InputType);
	if (!LastTimePtr)
		return 0.f;

	float TimeSinceUse = GetWorld()->GetTimeSeconds() - *LastTimePtr;
	float CooldownRemaining = AbilityData->CooldownTime - TimeSinceUse;

	return FMath::Max(CooldownRemaining, 0.f);
}

bool UCombatComponent::IsAbilityOnCooldown(EAbilityInput InputType, const UAbilityData* AbilityData) const
{
	if (!AbilityData)
		return false;

	const float* LastTimePtr = LastUsedTime.Find(InputType);
	if (!LastTimePtr)
		return false;

	float TimeSinceUse = GetWorld()->GetTimeSeconds() - *LastTimePtr;
	return TimeSinceUse < AbilityData->CooldownTime;
}

void UCombatComponent::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter && OwnerCharacter->GetCharacterMovement())
	{
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		bIsAttacking = false;
	}
}

void UCombatComponent::OnAttackHit(AActor* HitActor)
{
	if (!HitActor)
		return;
	CurrentRage += CurrentAbilityData->RageGenerated;

	CurrentRage = FMath::Clamp(CurrentRage, 0.f, MaxRage);

	OnRageChanged.Broadcast(CurrentRage, CurrentAbilityData->RageGenerated);

	ApplyDamage(HitActor, CurrentAbilityData ? CurrentAbilityData->DamageAmount : 10.f);

	UE_LOG(LogTemp, Log, TEXT("Hit target: %s"), *HitActor->GetName());
}

void UCombatComponent::ApplyDamage(AActor* Target, float DamageAmount)
{
	if (!Target) return;

	UHealthComponent* Health = Target->FindComponentByClass<UHealthComponent>();
	if (Health)
	{
		Health->ApplyDamage(DamageAmount);
	}
}

AActor* UCombatComponent::GetHitTarget()
{
	if (!EquippedWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetHitTarget called but no weapon equipped"));
		return nullptr;
	}

	return EquippedWeapon->PerformTrace();
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

