// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyRageBar.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORCOMBATSANDBOX_API UEnemyRageBar : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void UpdateRageBar(float Current, float Max);

protected:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* RageBar;
};
