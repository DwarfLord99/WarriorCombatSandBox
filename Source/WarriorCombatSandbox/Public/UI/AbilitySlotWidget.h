// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySlotData.h"
#include "AbilitySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORCOMBATSANDBOX_API UAbilitySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void SetSlotData(const FAbilitySlotData& Data);

protected:

	UPROPERTY(meta = (BindWidget))
	class UImage* IconImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KeybindText;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* CooldownOverlay;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* CooldownBar;
};
