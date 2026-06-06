// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include <Perception/AISenseConfig_Sight.h>
#include "BehaviorTree/BTTaskNode.h"
#include "EnemyAIController.generated.h"

class UBehaviorTree;

/**
 * 
 */
UCLASS()
class WARRIORCOMBATSANDBOX_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	AEnemyAIController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName HasLineofSightKey = "HasLineofSight";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName EnemyActorKey = "EnemyActor";

	UPROPERTY()
	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float PerceptionLossDelay = 4.f;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
};
