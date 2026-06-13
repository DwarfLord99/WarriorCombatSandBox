// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FindRandomPatrol.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORCOMBATSANDBOX_API UFindRandomPatrol : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float PatrolRadius = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	FBlackboardKeySelector PatrolLocationKey;
};
