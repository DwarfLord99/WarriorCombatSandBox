// Copyright Epic Games, Inc. All Rights Reserved.


#include "Controller/WarriorController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "WarriorCombatSandbox/WarriorCombatSandbox.h"
#include <Character/Player/WarriorCharacter.h>

void AWarriorController::BeginPlay()
{
	Super::BeginPlay();

}

void AWarriorController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			// add default mapping contexts
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
				UE_LOG(LogTemp, Warning, TEXT("Adding IMC: %s"), *GetNameSafe(CurrentContext));

			}
		}
	}
}
