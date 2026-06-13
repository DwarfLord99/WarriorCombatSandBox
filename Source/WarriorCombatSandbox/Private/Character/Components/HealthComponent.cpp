// Copyright Epic Games, Inc. All Rights Reserved.


#include "Character/Components/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	OnHealthChanged.Broadcast(CurrentHealth, 0.f);

	OwnerCombatComponent = GetOwner()->FindComponentByClass<UCombatComponent>();
}

void UHealthComponent::ApplyDamage(float DamageAmount, UAbilityData* AbilityData)
{
	if (DamageAmount <= 0.f || CurrentHealth <= 0.f)
		return;

	float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, MaxHealth);
	UE_LOG(LogTemp, Log, TEXT("Health changed: %f -> %f"), OldHealth, CurrentHealth);

	if (!AbilityData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyDamage called with NULL AbilityData"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyDamage called with ability: %s (CanInterrupt=%s)"),
			*AbilityData->GetName(),
			AbilityData->bCanInterrupt ? TEXT("true") : TEXT("false"));
	}



	if (AbilityData && AbilityData->bCanInterrupt)
	{
		if (OwnerCombatComponent && OwnerCombatComponent->IsAbilityInterruptible())
		{
			OwnerCombatComponent->InterruptCast();
			UE_LOG(LogTemp, Log, TEXT("Cast interrupted"));
		}
	}

	if (!bIsDead)
	{
		// Get anim instance of owner actor and play hit react montage if it exists
		if (HitReactMontage)
		{
			AActor* Owner = GetOwner();
			if (Owner)
			{
				UAnimInstance* AnimInstance = Owner->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();
				if (AnimInstance)
				{
					AnimInstance->Montage_Play(HitReactMontage);
				}
			}
		}
	}

	OnHealthChanged.Broadcast(CurrentHealth, CurrentHealth - OldHealth);

	if (CurrentHealth <= 0.f)
	{
		HandleDeath();
	}
}

void UHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.f || CurrentHealth <= 0.f)
		return;

	float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.f, MaxHealth);

	OnHealthChanged.Broadcast(CurrentHealth, CurrentHealth - OldHealth);
}

void UHealthComponent::HandleDeath()
{
	if (bIsDead)
		return;

	bIsDead = true;

	OnDeath.Broadcast();
}

void UHealthComponent::ResetHealth()
{
	bIsDead = false;
	CurrentHealth = MaxHealth;
	OnHealthChanged.Broadcast(CurrentHealth, 0.f);
}


