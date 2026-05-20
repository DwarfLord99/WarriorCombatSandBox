// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WarriorController.generated.h"

class UInputMappingContext;
class UUserWidget;

UCLASS(abstract)
class WARRIORCOMBATSANDBOX_API AWarriorController : public APlayerController
{
	GENERATED_BODY()

protected:

	// Input Mapping Contexts
	UPROPERTY(EditDefaultsOnly, Category = "Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	// Gameplay initialization
	virtual void BeginPlay() override;

	// Setup input mapping contexts
	virtual void SetupInputComponent() override;
	
};
