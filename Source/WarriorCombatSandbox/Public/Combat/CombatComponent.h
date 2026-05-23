// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityData.h"
#include "CombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRageChanged, float, NewRage, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARRIORCOMBATSANDBOX_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float MaxRage = 50.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	float CurrentRage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UAbilityData* CurrentAbilityData;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void OnAttackHit();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ApplyDamage(AActor* Target, float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	AActor* GetHitTarget();

	bool bIsAttacking = false;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnRageChanged OnRageChanged;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TryUseAbility(UAbilityData* AbilityData);
};
