// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PursuePlayer.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORCOMBATSANDBOX_API UPursuePlayer : public UBTTaskNode
{
	GENERATED_BODY()

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}
	
public:

	UPROPERTY(EditAnywhere, Category = "Chase")
	float ChaseSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRange = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FBlackboardKeySelector EnemyActorKey;
};
