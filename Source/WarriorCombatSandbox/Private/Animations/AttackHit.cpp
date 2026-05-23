// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AttackHit.h"
#include "Combat/CombatComponent.h"
#include "GameFramework/Actor.h"

void UAttackHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
		return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
		return;

	UCombatComponent* CombatComponent = Owner->FindComponentByClass<UCombatComponent>();
	if (!CombatComponent)
		return;

	CombatComponent->OnAttackHit();
}
