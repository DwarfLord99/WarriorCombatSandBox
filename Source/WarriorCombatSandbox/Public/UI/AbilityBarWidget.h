// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORCOMBATSANDBOX_API UAbilityBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void InitializeSlots(int32 NumSlots);

	class UAbilitySlotWidget* GetSlotWidget(int32 SlotIndex) const;

protected:

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* SlotContainer;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UAbilitySlotWidget> SlotClass;

	UPROPERTY()
	TArray<UAbilitySlotWidget*> Slots;

};
