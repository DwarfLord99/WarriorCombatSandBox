// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UPlayerHUDWidget;
class UHealthBarWidget;
class URageBarWidget;
class UAbilityBarWidget;

/**
 * 
 */
UCLASS()
class WARRIORCOMBATSANDBOX_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	void UpdateHealth(float Current, float Max);
	void UpdateRage(float Current, float Max);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPlayerHUDWidget> PlayerHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UHealthBarWidget> HealthBarClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<URageBarWidget> RageBarClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UAbilityBarWidget> AbilityBarClass;

	UPROPERTY()
	UPlayerHUDWidget* PlayerHUDWidget;

	UPROPERTY()
	UHealthBarWidget* HealthBar;

	UPROPERTY()
	URageBarWidget* RageBar;

	UPROPERTY()
	UAbilityBarWidget* AbilityBar;
	
};
