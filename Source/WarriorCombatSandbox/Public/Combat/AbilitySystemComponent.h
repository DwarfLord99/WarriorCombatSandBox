// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat/AbilityInputSystem.h"
#include "UI/AbilitySlotData.h"
#include "AbilitySystemComponent.generated.h"

class UAbilityData;
class UCombatComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARRIORCOMBATSANDBOX_API UAbilitySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilitySystemComponent();

	void TryUseAbility(int32 SlotIndex);

	FAbilitySlotData GetSlotData(int32 SlotIndex) const;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void HandleInput(EAbilityInput Input);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<float> CurrentCooldowns;

	UPROPERTY()
	UCombatComponent* CombatComponent;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<UAbilityData*> CoreAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<EAbilityInput> CoreInputMapping;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<UAbilityData*> SlottedAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<EAbilityInput> SlotInputMapping;

	bool CanUseAbility(int32 SlotIndex) const;
	void ActiveAbility(int32 SlotIndex);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	int32 GetSlotIndexFromInput(EAbilityInput Input) const;
};
