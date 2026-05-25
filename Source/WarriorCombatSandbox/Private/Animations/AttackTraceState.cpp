// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AttackTraceState.h"
#include "Combat/CombatComponent.h"

void UAttackTraceState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    HitActors.Empty();

    if (auto* Owner = MeshComp->GetOwner())
    {
        if (auto* Combat = Owner->FindComponentByClass<UCombatComponent>())
        {
            if (Combat->EquippedWeapon)
            {
                Combat->EquippedWeapon->StartTrace();
				UE_LOG(LogTemp, Log, TEXT("AttackTraceState: Trace started"));
            }
        }
    }
}

void UAttackTraceState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	UE_LOG(LogTemp, Log, TEXT("AttackTraceState: NotifyTick called"));

    if (auto* Owner = MeshComp->GetOwner())
    {
        if (auto* Combat = Owner->FindComponentByClass<UCombatComponent>())
        {
            if (Combat->EquippedWeapon)
            {
                AActor* HitActor = Combat->EquippedWeapon->PerformTrace();

                // Ignore player
                if (HitActor == Owner)
                {
                    HitActor = nullptr;
				}

                if (HitActor && !HitActors.Contains(HitActor))
                {
                    HitActors.Add(HitActor);
                    Combat->OnAttackHit(HitActor); // applies damage once
                }
            }
        }
    }
}

void UAttackTraceState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (auto* Owner = MeshComp->GetOwner())
    {
        if (auto* Combat = Owner->FindComponentByClass<UCombatComponent>())
        {
            if (Combat->EquippedWeapon)
            {
                Combat->EquippedWeapon->EndTrace();
				UE_LOG(LogTemp, Log, TEXT("AttackTraceState: Trace ended"));
            }
        }
    }

    HitActors.Empty();
}
