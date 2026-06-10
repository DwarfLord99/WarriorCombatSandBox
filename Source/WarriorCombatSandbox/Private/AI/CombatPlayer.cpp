// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CombatPlayer.h"
#include "AIController.h"
#include "Character/Enemy/EnemyCharacter.h"

EBTNodeResult::Type UCombatPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacter* Self = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (!Self)
		return EBTNodeResult::Failed;

	// Select ability
	UAbilityData* SelectedAbility = SelectAbility(Self);

	if (SelectedAbility == nullptr)
	{
		return EBTNodeResult::Succeeded;
	}

	// Use ability
	Self->HandleAttack(SelectedAbility);

	return EBTNodeResult::Succeeded;
}

UAbilityData* UCombatPlayer::SelectAbility(AEnemyCharacter* Enemy)
{
	if (Enemy->CanUseAbility(Enemy->HeavyAttackData))
	{
		return Enemy->HeavyAttackData;
	}
	
	if (Enemy->CanUseAbility(Enemy->BasicAttackData))
	{
		return Enemy->BasicAttackData;
	}

	return nullptr;
}
