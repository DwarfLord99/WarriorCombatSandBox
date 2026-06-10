// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyCastBar.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORCOMBATSANDBOX_API UEnemyCastBar : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void UpdateCastBar(float Percent);

protected:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* CastBar;
};
