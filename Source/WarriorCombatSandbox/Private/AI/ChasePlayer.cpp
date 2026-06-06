// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ChasePlayer.h"
#include "Character/Enemy/EnemyCharacter.h"
#include "AIController.h"

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (EnemyCharacter)
	{
		EnemyCharacter->UpdateWalkSpeed(ChaseSpeed);
		UE_LOG(LogTemp, Warning, TEXT("'%s' is executing ChasePlayer task. Updated walk speed to: %f"), *EnemyCharacter->GetName(), ChaseSpeed);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
