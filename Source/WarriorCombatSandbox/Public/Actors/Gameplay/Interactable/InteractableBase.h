// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableBase.generated.h"

class UBoxComponent;

UCLASS()
class WARRIORCOMBATSANDBOX_API AInteractableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Collision volume for interaction range
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	UBoxComponent* InteractionVolume;

	// Check if player is in range to interact
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bPlayerInRange = false;

	// Player reference for interaction
	UPROPERTY()
	AActor* CachedPlayer;

public:

	// Function to handle interaction logic
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void Interact();

	// Whether this interactable can be interacted with
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	bool CanInteract();
	virtual bool CanInteract_Implementation();

	// Blueprint event for child classes to override
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void ExecuteInteraction();

	// Focus events for UI feedback
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnFocus();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnUnfocus();

protected:

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
