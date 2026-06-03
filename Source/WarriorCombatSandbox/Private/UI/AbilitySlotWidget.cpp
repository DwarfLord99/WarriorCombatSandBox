// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AbilitySlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"

void UAbilitySlotWidget::InitializeSlot(int32 inIndex, UAbilitySystemComponent* InASC)
{
	SlotIndex = inIndex;
	AbilitySystem = InASC;
}

void UAbilitySlotWidget::UpdateSlot()
{
	if (!AbilitySystem) return;

	FAbilitySlotData SlotData = AbilitySystem->GetSlotData(SlotIndex);

	SetSlotData(SlotData);

	//UE_LOG(LogTemp, Warning, TEXT("Updating slot %d"), SlotIndex);

}

void UAbilitySlotWidget::SetSlotData(const FAbilitySlotData& Data)
{
	if (IconImage)
	{
		IconImage->SetBrushFromTexture(Data.Icon);
	}

	if (KeybindText)
	{
		KeybindText->SetText(Data.Keybind);
	}

	if (CooldownBar)
	{
		float Percent = (Data.Cooldown > 0) ? (Data.CurrentCooldown / Data.Cooldown) : 0.f;

		CooldownBar->SetPercent(Percent);
	}

	if (CooldownOverlay)
	{
		CooldownOverlay->SetVisibility((Data.CurrentCooldown > 0) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}
