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

void UCombatComponent::TryUseAbility(UAbilityData* AbilityData)
{
	if (!AbilityData)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryUseAbility called with null AbilityData"));
		return;
	}

	if (CurrentRage < AbilityData->RageCost)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough rage to use ability"));
		return;
	}

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter && OwnerCharacter->GetCharacterMovement()->IsFalling())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot use ability while in the air"));
		return;
	}

	CurrentAbilityData = AbilityData;	

	if (OwnerCharacter && AbilityData->AttackMontage && !bIsAttacking)
	{
		bIsAttacking = true;

		CurrentRage -= AbilityData->RageCost;

		CurrentRage = FMath::Clamp(CurrentRage, 0.f, MaxRage);

		OnRageChanged.Broadcast(CurrentRage, CurrentAbilityData->RageGenerated);

		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();

		if (AnimInstance)
		{
			// Stop player movement while attacking
			OwnerCharacter->GetCharacterMovement()->DisableMovement();
		}

		// Bind to montage end event
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindUObject(this, &UCombatComponent::OnAttackMontageEnded);
		AnimInstance->Montage_Play(AbilityData->AttackMontage);
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AbilityData->AttackMontage);
	}
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

