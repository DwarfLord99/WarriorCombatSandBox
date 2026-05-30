// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthBar.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class WARRIORCOMBATSANDBOX_API UEnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar(float Current, float Max);

protected:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
};
