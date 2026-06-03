// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/AbilitySystemComponent.h"
#include "Combat/AbilityData.h"
#include "Combat/CombatComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UAbilitySystemComponent::UAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UAbilitySystemComponent::TryUseAbility(int32 SlotIndex)
{
	if (!CanUseAbility(SlotIndex)) return;

	UE_LOG(LogTemp, Warning, TEXT("TryUseAbility(%d) called"), SlotIndex);

	ActiveAbility(SlotIndex);
}

FAbilitySlotData UAbilitySystemComponent::GetSlotData(int32 SlotIndex) const
{
	FAbilitySlotData SlotData;

	if (!SlottedAbilities.IsValidIndex(SlotIndex)) return SlotData;

	const UAbilityData* Ability = SlottedAbilities[SlotIndex];

	SlotData.Icon = Ability ? Ability->IconImage : nullptr;
	SlotData.Cooldown = Ability ? Ability->CooldownTime : 0.f;
	SlotData.CurrentCooldown = CurrentCooldowns.IsValidIndex(SlotIndex) ? CurrentCooldowns[SlotIndex] : 0.f;
	SlotData.Keybind = FText::FromString(FString::Printf(TEXT("%d"), SlotIndex + 1));
	SlotData.bIsUsable = CanUseAbility(SlotIndex);

	//UE_LOG(LogTemp, Warning, TEXT("GetSlotData called for slot %d"), SlotIndex);

	return SlotData;
}


// Called when the game starts
void UAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	CombatComponent = GetOwner()->FindComponentByClass<UCombatComponent>();

	CurrentCooldowns.SetNum(SlottedAbilities.Num());
	for (float& CD: CurrentCooldowns)
	{
		CD = 0.f;
	}

	// Print out the slotted abilities and their corresponding input mappings for debugging
	for (int32 i = 0; i < SlottedAbilities.Num(); i++)
	{
		FString AbilityName = SlottedAbilities[i] ? SlottedAbilities[i]->GetName() : TEXT("None");
		FString InputName = SlotInputMapping.IsValidIndex(i) ? UEnum::GetValueAsString(SlotInputMapping[i]) : TEXT("None");
		UE_LOG(LogTemp, Warning, TEXT("Slot %d: Ability = %s, Input = %s"), i, *AbilityName, *InputName);
	}
}


void UAbilitySystemComponent::HandleInput(EAbilityInput Input)
{
	int32 CoreIndex = CoreInputMapping.IndexOfByKey(Input);
	if (CoreIndex != INDEX_NONE)
	{
		UAbilityData* CoreAbility = CoreAbilities.IsValidIndex(CoreIndex) ? CoreAbilities[CoreIndex] : nullptr;
		CombatComponent->TryUseAbility(Input, CoreAbility);
		return;
	}

	int32 SlotIndex = SlotInputMapping.IndexOfByKey(Input);
	if (SlotIndex != INDEX_NONE && !CombatComponent->bIsAttacking)
	{
		TryUseAbility(SlotIndex);
		return;
	}
}

bool UAbilitySystemComponent::CanUseAbility(int32 SlotIndex) const
{
	if (!SlottedAbilities.IsValidIndex(SlotIndex)) return false;

	const UAbilityData* Ability = SlottedAbilities[SlotIndex];
	if (!Ability) return false;

	if (CurrentCooldowns[SlotIndex] > 0.f) return false;

	if (CombatComponent->GetCurrentRage() < Ability->RageCost) return false;

	return true;
}

void UAbilitySystemComponent::ActiveAbility(int32 SlotIndex)
{
	if (!SlottedAbilities.IsValidIndex(SlotIndex) || !CombatComponent) return;

	UAbilityData* Ability = SlottedAbilities[SlotIndex];
	if (!Ability) return;

	if (!SlotInputMapping.IsValidIndex(SlotIndex)) return;

	EAbilityInput InputType = SlotInputMapping[SlotIndex];

	CombatComponent->TryUseAbility(InputType, Ability);

	// Start cooldown
	CurrentCooldowns[SlotIndex] = Ability->CooldownTime;
}

// Called every frame
void UAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (int32 i = 0; i < CurrentCooldowns.Num(); i++)
	{
		if (CurrentCooldowns[i] > 0.f)
		{
			CurrentCooldowns[i] -= DeltaTime;
			if (CurrentCooldowns[i] < 0.f)
			{
				CurrentCooldowns[i] = 0.f;
			}
		}
	}
}

int32 UAbilitySystemComponent::GetSlotIndexFromInput(EAbilityInput Input) const
{
	for (int32 i = 0; i < SlotInputMapping.Num(); i++)
	{
		if (SlotInputMapping[i] == Input)
		{
			return i;
		}
	}

	return INDEX_NONE;
}

