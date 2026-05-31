#pragma once

#include "CoreMinimal.h"
#include "AbilityInputSystem.generated.h"

UENUM(BlueprintType)
enum class EAbilityInput : uint8
{
    BasicAttack UMETA(DisplayName = "Basic Attack"),
    HeavyAttack UMETA(DisplayName = "Heavy Attack"),
	Slot1 UMETA(DisplayName = "Slot 1"),
	Slot2 UMETA(DisplayName = "Slot 2"),
	Slot3 UMETA(DisplayName = "Slot 3")
};
