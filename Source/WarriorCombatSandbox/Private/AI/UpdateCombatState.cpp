// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UpdateCombatState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controller/EnemyAIController.h"
#include "Character/Enemy/EnemyCharacter.h"
#include "Character/Player/WarriorCharacter.h"

void UUpdateCombatState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* AIPawn = AIController ? AIController->GetPawn() : nullptr;
	if (!AIPawn) return;

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(AIPawn);

	AWarriorCharacter* EnemyActor = Cast<AWarriorCharacter>(BlackboardComp->GetValueAsObject(EnemyActorKey.SelectedKeyName));

	const float CurrentTime = GetWorld()->GetTimeSeconds();

	// Check if player is dead
	if (EnemyActor && EnemyActor->IsDead())
	{
		BlackboardComp->ClearValue(EnemyActorKey.SelectedKeyName);
		BlackboardComp->SetValueAsBool(IsInCombatKey.SelectedKeyName, false);

		EnemyCharacter->UpdateMovementSpeed(EnemyCharacter->PatrolSpeed);
		return;
	}

	// If player is visible, update last seen time and set combat state to true
	if (EnemyActor)
	{
		BlackboardComp->SetValueAsFloat(PlayerLastSeenTimeKey.SelectedKeyName, CurrentTime);
	}

	// Check distance to player and update combat state
	float DistanceToPlayer = EnemyActor ? FVector::Dist(AIPawn->GetActorLocation(), EnemyActor->GetActorLocation()) : FLT_MAX;

	const float CombatRange = 200.f; // Example combat range, adjust as needed
	const float DisengageRange = 2000.f; // Example disengage range, adjust as needed
	const float LoseSightTime = 3.f; // Time after which the AI will lose sight of the player

	// If player is within combat range, set combat state to true
	if (EnemyActor && DistanceToPlayer >= CombatRange)
	{
		BlackboardComp->SetValueAsBool(IsInCombatKey.SelectedKeyName, true);

		// Speed up to attack speed
		if (EnemyCharacter)
		{
			EnemyCharacter->UpdateMovementSpeed(EnemyCharacter->PursueSpeed);
		}
		return;
	}

	// If chasing but not in combat, use chase speed
	if (DistanceToPlayer > DisengageRange)
	{
		BlackboardComp->ClearValue(EnemyActorKey.SelectedKeyName);
		BlackboardComp->SetValueAsBool(IsInCombatKey.SelectedKeyName, false);
		return;
	}

	// Disengage if player has been out of sight for too long
	float LastSeenTime = BlackboardComp->GetValueAsFloat(PlayerLastSeenTimeKey.SelectedKeyName);
	if (CurrentTime - LastSeenTime > LoseSightTime)
	{
		BlackboardComp->ClearValue(EnemyActorKey.SelectedKeyName);
		BlackboardComp->SetValueAsBool(IsInCombatKey.SelectedKeyName, false);
		return;
	}

	// If player is dead, reset combat state
	if (EnemyActor && EnemyActor->IsDead())
	{
		BlackboardComp->SetValueAsBool(IsInCombatKey.SelectedKeyName, false);
		return;
	}
}
