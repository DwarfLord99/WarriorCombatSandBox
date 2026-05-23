// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class WARRIORCOMBATSANDBOX_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* RageBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UFUNCTION(BlueprintCallable, Category = "PlayerHUD")
	void UpdateHealth(float Current, float Max);

	UFUNCTION(BlueprintCallable, Category = "PlayerHUD")
	void UpdateRage(float Current, float Max);
};
