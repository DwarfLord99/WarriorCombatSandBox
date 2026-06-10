// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Character/Components/HealthComponent.h"
#include "Combat/CombatComponent.h"
#include "Combat/AbilityData.h"
#include "Combat/Damageable.h"
#include "Weapons/Weapon.h"
#include "UI/EnemyHealthBar.h"
#include "UI/EnemyRageBar.h"
#include "UI/EnemyCastBar.h"
#include "EnemyCharacter.generated.h"

class UWidgetComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogEnemyCharacter, Log, All);

UCLASS()
class WARRIORCOMBATSANDBOX_API AEnemyCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()	

	// Health Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

public:

	// Combat Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;

	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* HealthBarWidget;

	UPROPERTY()
	UEnemyHealthBar* CachedHealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* RageBarWidget;

	UPROPERTY()
	UEnemyRageBar* CachedRageBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* CastBarWidget;

	UPROPERTY()
	UEnemyCastBar* CachedCastBar;

	UPROPERTY()
	APlayerCameraManager* CachedCameraManager;

	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	UAnimMontage* DeathMontage;

	UPROPERTY()
	FTimerHandle HideHealthBarTimer;

	// Weapon
	UPROPERTY(EditAnywhere, Category = Equipment)
	TSubclassOf<AWeapon> WeaponClass;

	// Equipped Weapon
	UPROPERTY()
	AWeapon* EquippedWeapon;

	FTimerHandle HealthBarHideTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void RecieveDamage_Implementation(float DamageAmount) override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UAbilityData* BasicAttackData;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UAbilityData* HeavyAttackData;

	UFUNCTION()
	void UpdateWalkSpeed(float NewSpeed);

	UFUNCTION()
	void HandleDeath();

	UFUNCTION()
	void HandleHealthChanged(float Current, float Max);

	UFUNCTION()
	void HandleRageChanged(float Current, float Max);

	UFUNCTION()
	void HandleBeginCast();

	UFUNCTION()
	void HandleFinishCast();

	UFUNCTION()
	void HandleInterruptCast();

	UFUNCTION()
	void HideHealthBar();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool CanUseAbility(UAbilityData* AbilityData);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void HandleAttack(UAbilityData* AbilityData);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsDead = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanAttack = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bPlayerDetected = false;

};
