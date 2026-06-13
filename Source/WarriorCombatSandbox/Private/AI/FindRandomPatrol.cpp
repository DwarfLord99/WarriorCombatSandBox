// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/FindRandomPatrol.h"
#include "Character/Enemy/EnemyCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Engine/World.h"

EBTNodeResult::Type UFindRandomPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (EnemyCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("'%s' is executing FindRandomPatrol task."), *EnemyCharacter->GetName());

		EnemyCharacter->UpdateMovementSpeed(EnemyCharacter->PatrolSpeed);

		FVector EnemyLocation = EnemyCharacter->GetActorLocation();

		UWorld* World = EnemyCharacter->GetWorld();
		UE_LOG(LogTemp, Warning, TEXT("'%s' is trying to find a random patrol location. Enemy Location: %s"), *EnemyCharacter->GetName(), *EnemyLocation.ToString());
		if (!World) return EBTNodeResult::Failed;

		UNavigationSystemBase* NavSystemBase = World->GetNavigationSystem();
		UE_LOG(LogTemp, Warning, TEXT("'%s' got Navigation System: %s"), *EnemyCharacter->GetName(), *GetNameSafe(NavSystemBase));
		if (!NavSystemBase) return EBTNodeResult::Failed;

		UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(NavSystemBase);
		UE_LOG(LogTemp, Warning, TEXT("'%s' cast Navigation System to UNavigationSystemV1: %s"), *EnemyCharacter->GetName(), *GetNameSafe(NavSystem));
		if (!NavSystem) return EBTNodeResult::Failed;

		FNavLocation RandomPatrolLocation;
		bool bFoundPatrolLocation = NavSystem->GetRandomReachablePointInRadius(EnemyLocation, PatrolRadius, RandomPatrolLocation);
		UE_LOG(LogTemp, Warning, TEXT("'%s' found random patrol location: %s, Location: %s"), *EnemyCharacter->GetName(), bFoundPatrolLocation ? TEXT("True") : TEXT("False"), *RandomPatrolLocation.Location.ToString());

		if (bFoundPatrolLocation)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(PatrolLocationKey.SelectedKeyName, RandomPatrolLocation.Location);
			// print name of patrol location key
			UE_LOG(LogTemp, Warning, TEXT("'%s' set patrol location in blackboard key: %s"), *EnemyCharacter->GetName(), *PatrolLocationKey.SelectedKeyName.ToString());

			UE_LOG(LogTemp, Warning, TEXT("'%s' set patrol location in blackboard: %s"), *EnemyCharacter->GetName(), *RandomPatrolLocation.Location.ToString());
			return EBTNodeResult::Succeeded;
		}
		else
		{
			// If no valid patrol location found, set the blackboard key to the enemy's current location as a fallback
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(PatrolLocationKey.SelectedKeyName, EnemyLocation);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
