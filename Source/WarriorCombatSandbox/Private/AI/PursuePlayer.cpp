// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PursuePlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"
#include "Character/Enemy/EnemyCharacter.h"

EBTNodeResult::Type UPursuePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	UObject* TargetObject = BlackboardComp->GetValueAsObject(EnemyActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	AEnemyCharacter* Self = Cast<AEnemyCharacter>(AIController->GetPawn());

	EPathFollowingRequestResult::Type MoveToResult = AIController->MoveToActor(TargetActor, AttackRange, true, true, true, 0, true);

	if (MoveToResult)
	{
		Self->UpdateWalkSpeed(ChaseSpeed);
		Self->bPlayerDetected = true;
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
