#pragma once

#include "CoreMinimal.h"
#include "AbilityInputSystem.generated.h"

UENUM(BlueprintType)
enum class EAbilityInput : uint8
{
    BasicAttack UMETA(DisplayName = "Basic Attack"),
    HeavyAttack UMETA(DisplayName = "Heavy Attack"),
	DefenseSkill UMETA(DisplayName = "Defense Skill"),
	InterruptSkill UMETA(DisplayName = "Interrupt Skill"),
	UltimateSkill UMETA(DisplayName = "Ultimate Skill")
};
