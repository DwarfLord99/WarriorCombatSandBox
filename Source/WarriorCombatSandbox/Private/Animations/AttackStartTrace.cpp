// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AttackStartTrace.h"
#include "Combat/CombatComponent.h"

void UAttackStartTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
		return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
		return;

	UCombatComponent* Combat = Owner->FindComponentByClass<UCombatComponent>();
	if (!Combat)
		return;

	if (Combat->EquippedWeapon)
	{
		Combat->EquippedWeapon->StartTrace();
	}
}
