// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityData.h"
#include "Combat/Damageable.h"
#include "Combat/AbilityInputSystem.h"
#include "Weapons/Weapon.h"
#include "CombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRageChanged, float, NewRage, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginCast);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishCast);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInterruptCast);

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

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ApplyDamage(AActor* Target, float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	AActor* GetHitTarget();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetEquippedWeapon(AWeapon* NewWeapon) { EquippedWeapon = NewWeapon; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UAbilityData* CurrentAbilityData;

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnRageChanged OnRageChanged;

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnBeginCast OnBeginCast;

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnFinishCast OnFinishCast;

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnInterruptCast OnInterruptCast;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	AWeapon* EquippedWeapon;

	UPROPERTY()
	TMap<EAbilityInput, float> LastUsedTime;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void OnAttackHit(AActor* HitActor);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TryUseAbility(EAbilityInput InputType, UAbilityData* AbilityData);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TryUseAbility_AI(UAbilityData* AbilityData);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool CanUseAbility(UAbilityData* AbilityData);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ExecuteAbility(UAbilityData* AbilityData);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetCooldownRemaining(EAbilityInput InputType, const UAbilityData* AbilityData) const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetCurrentRage() const { return CurrentRage; }

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetMaxRage() const { return MaxRage; }

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsAbilityOnCooldown(EAbilityInput InputType, const UAbilityData* AbilityData) const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartCooldown(UAbilityData* AbilityData);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void BeginCast(UAbilityData* AbilityData);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void FinishCast();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void InterruptCast();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsCasting() const { return bIsCasting; }

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetCastPercent() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsAbilityInterruptible() const;

	UPROPERTY()
	TMap<UAbilityData*, float> CooldownTimers;

	bool bIsAttacking = false;

	bool bIsCasting = false;
	UAbilityData* CastingAbilityData = nullptr;
	float CastTimer = 0.f;
};
