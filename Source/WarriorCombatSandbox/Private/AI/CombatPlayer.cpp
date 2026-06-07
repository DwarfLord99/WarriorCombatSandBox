// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CombatPlayer.h"
#include "AIController.h"
#include "Character/Enemy/EnemyCharacter.h"

EBTNodeResult::Type UCombatPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacter* Self = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (Self)
	{
		UE_LOG(LogTemp, Warning, TEXT("'%s' is attacking Player."), *OwnerComp.GetAIOwner()->GetName());
		Self->BasicAttack();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
