// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CombatPlayer.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORCOMBATSANDBOX_API UCombatPlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:

	UPROPERTY(EditAnywhere, Category = "Combat")
	FBlackboardKeySelector EnemyActorKey;
};
