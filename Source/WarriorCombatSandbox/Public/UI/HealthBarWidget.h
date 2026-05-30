// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class WARRIORCOMBATSANDBOX_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHealth(float Current, float Max);

protected:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
};
