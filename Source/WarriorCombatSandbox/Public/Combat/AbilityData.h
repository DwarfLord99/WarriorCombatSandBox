// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilityData.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORCOMBATSANDBOX_API UAbilityData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	float RageCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	float RageGenerated;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	float CooldownTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	bool bIsCasted = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	float CastTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	bool bIsInterruptible = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	float InterruptCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	UTexture2D* IconImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	bool bCanInterrupt = false;
};
