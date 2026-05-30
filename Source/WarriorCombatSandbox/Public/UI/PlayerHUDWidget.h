// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORCOMBATSANDBOX_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	UWidget* HealthBarAnchor;

	UPROPERTY(meta = (BindWidget))
	UWidget* RageBarAnchor;

	UPROPERTY(meta = (BindWidget))
	UWidget* AbilityBarAnchor;
};
