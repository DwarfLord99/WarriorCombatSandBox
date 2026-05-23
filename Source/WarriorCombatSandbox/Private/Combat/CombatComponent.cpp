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

		if (EquippedWeapon)
			EquippedWeapon->StartTrace();

		CurrentRage -= AbilityData->RageCost;
		CurrentRage += AbilityData->RageGenerated;

		CurrentRage = FMath::Clamp(CurrentRage, 0.f, MaxRage);

		OnRageChanged.Broadcast(CurrentRage, AbilityData->RageGenerated);

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

		if (EquippedWeapon)
			EquippedWeapon->EndTrace();
	}
}

void UCombatComponent::OnAttackHit()
{
	AActor* HitTarget = GetHitTarget();
	if (!HitTarget)
		return;

	ApplyDamage(HitTarget, CurrentAbilityData ? CurrentAbilityData->DamageAmount : 10.f);

	UE_LOG(LogTemp, Log, TEXT("Hit target: %s"), *HitTarget->GetName());
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
	//ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	//if (!OwnerCharacter)
	//	return nullptr;

	//FVector Start = OwnerCharacter->GetActorLocation();
	//FVector Forward = OwnerCharacter->GetActorForwardVector();
	//FVector End = Start + Forward * 150.f; // attack range

	//FHitResult Hit;
	//FCollisionQueryParams Params;
	//Params.AddIgnoredActor(OwnerCharacter);

	//bool bHit = GetWorld()->LineTraceSingleByChannel(
	//	Hit,
	//	Start,
	//	End,
	//	ECC_Pawn,
	//	Params
	//);

	//if (bHit)
	//{
	//	return Hit.GetActor();
	//}

	//return nullptr;

	if (!EquippedWeapon)
		return nullptr;

	return EquippedWeapon->PerformTrace();
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

