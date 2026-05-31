// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AbilityBarWidget.h"
#include "UI/AbilitySlotWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

void UAbilityBarWidget::InitializeSlots(int32 NumSlots)
{
	if (!SlotClass || !SlotContainer)
	{
		return;
	}

	Slots.Empty();

	for (int32 i = 0; i < NumSlots; i++)
	{
		UAbilitySlotWidget* AbilitySlot = CreateWidget<UAbilitySlotWidget>(GetWorld(), SlotClass);
		SlotContainer->AddChild(AbilitySlot);
		Slots.Add(AbilitySlot);
	}
}

UAbilitySlotWidget* UAbilityBarWidget::GetSlotWidget(int32 SlotIndex) const
{
	return Slots.IsValidIndex(SlotIndex) ? Slots[SlotIndex] : nullptr;
}
