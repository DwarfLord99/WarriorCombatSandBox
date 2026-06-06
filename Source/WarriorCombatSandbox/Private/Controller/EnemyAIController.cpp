// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	if (PerceptionComponent && SightConfig)
	{
		SightConfig->SightRadius = 1000.0f;
		SightConfig->LoseSightRadius = 1200.0f;
		SightConfig->PeripheralVisionAngleDegrees = 45.0f;

		PerceptionComponent->ConfigureSense(*SightConfig);
		PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
	}
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// Check if actor is valid and if the stimulus is active (i.e., the actor is currently perceived)
	if (Actor && Stimulus.WasSuccessfullySensed())
	{
		// Clear and invalidate timer by handle
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

		// Set the blackboard key to indicate that the actor is perceived
		GetBlackboardComponent()->SetValueAsBool(HasLineofSightKey, true);
		UE_LOG(LogTemp, Log, TEXT("Has line of sight to actor: %s"), *Actor->GetName());
		UE_LOG(LogTemp, Log, TEXT("Setting blackboard key '%s' to true"), *HasLineofSightKey.ToString());
		GetBlackboardComponent()->SetValueAsObject(EnemyActorKey, Actor);
		UE_LOG(LogTemp, Log, TEXT("Perceived actor: %s"), *Actor->GetName());
		UE_LOG(LogTemp, Log, TEXT("Setting blackboard key '%s' to actor: %s"), *EnemyActorKey.ToString(), *Actor->GetName());
	}
	else
	{
		// Create a timer to clear the blackboard key after a 4.f delay to allow for temporary loss of sight (e.g., due to obstacles)
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			UE_LOG(LogTemp, Log, TEXT("Lost sight of actor: %s"), *GetNameSafe(GetBlackboardComponent()->GetValueAsObject(EnemyActorKey)));
			GetBlackboardComponent()->SetValueAsBool(HasLineofSightKey, false);
			GetBlackboardComponent()->ClearValue(EnemyActorKey);
			}, PerceptionLossDelay, false);
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find Behavior Tree Asset!"), *GetNameSafe(this));
	}
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// draw debug cone to visualize sight radius and peripheral vision angle
	if (PerceptionComponent && SightConfig)
	{
		FVector Location = GetPawn()->GetActorLocation();
		FVector ForwardVector = GetPawn()->GetActorForwardVector();
		DrawDebugCone(GetWorld(), Location, ForwardVector, SightConfig->SightRadius, FMath::DegreesToRadians(SightConfig->PeripheralVisionAngleDegrees), FMath::DegreesToRadians(SightConfig->PeripheralVisionAngleDegrees), 12, FColor::Green, false, -1.0f, 0, 2.0f);
	}
}
