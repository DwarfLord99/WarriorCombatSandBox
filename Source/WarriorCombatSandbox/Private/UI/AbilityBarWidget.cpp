// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AbilityBarWidget.h"
#include "UI/AbilitySlotWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

void UAbilityBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("AbilityBarWidget Constructed"));
}


void UAbilityBarWidget::InitializeBar(UAbilitySystemComponent* InASC, int32 NumSlots)
{
	AbilitySystem = InASC;

	if (!SlotClass || !SlotContainer)
		return;

	Slots.Empty();

	for (int32 i = 0; i < NumSlots; i++)
	{
		UAbilitySlotWidget* ASlot = CreateWidget<UAbilitySlotWidget>(GetWorld(), SlotClass);

		ASlot->InitializeSlot(i, AbilitySystem);

		SlotContainer->AddChild(ASlot);
		Slots.Add(ASlot);
	}

	UE_LOG(LogTemp, Warning, TEXT("InitializeBar called. ASC = %s"), *GetNameSafe(InASC));
	UE_LOG(LogTemp, Warning, TEXT("NumSlots = %d"), NumSlots);
}

UAbilitySlotWidget* UAbilityBarWidget::GetSlotWidget(int32 SlotIndex) const
{
	return Slots.IsValidIndex(SlotIndex) ? Slots[SlotIndex] : nullptr;
}

void UAbilityBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	for (UAbilitySlotWidget* ASlot : Slots)
	{
		if (ASlot)
		{
			ASlot->UpdateSlot();
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("AbilityBar Tick: %d slots"), Slots.Num());

}
