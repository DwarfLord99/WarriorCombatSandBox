// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RageBarWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class WARRIORCOMBATSANDBOX_API URageBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Rage")
	void SetRage(float Current, float Max);

protected:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* RageBar;
};
