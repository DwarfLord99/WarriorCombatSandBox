// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySlotData.generated.h"

USTRUCT(BlueprintType)
struct FAbilitySlotData
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Cooldown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentCooldown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Keybind;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsUsable;
};

