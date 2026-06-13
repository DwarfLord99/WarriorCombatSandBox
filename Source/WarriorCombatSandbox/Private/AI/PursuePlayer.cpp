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
	if (!AIController) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(EnemyActorKey.SelectedKeyName));
	if (!TargetActor) return EBTNodeResult::Failed;

	AIController->MoveToActor(TargetActor, AttackRange, true);

	return EBTNodeResult::Succeeded;
}
