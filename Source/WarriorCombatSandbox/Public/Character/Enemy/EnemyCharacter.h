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
#include "EnemyCharacter.generated.h"

class UWidgetComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogEnemyCharacter, Log, All);

UCLASS()
class WARRIORCOMBATSANDBOX_API AEnemyCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()

	// Combat Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;

	// Health Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* HealthBarWidget;

	UPROPERTY()
	APlayerCameraManager* CachedCameraManager;

	UPROPERTY()
	UEnemyHealthBar* CachedHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	UAnimMontage* DeathMontage;

	UPROPERTY()
	FTimerHandle HideHealthBarTimer;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UAbilityData* BasicAttackData;

	// Weapon
	UPROPERTY(EditAnywhere, Category = Equipment)
	TSubclassOf<AWeapon> WeaponClass;

	// Equipped Weapon
	UPROPERTY()
	AWeapon* EquippedWeapon;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void RecieveDamage_Implementation(float DamageAmount) override;

	UFUNCTION()
	void UpdateWalkSpeed(float NewSpeed);

	UFUNCTION()
	void HandleDeath();

	UFUNCTION()
	void HandleHealthChanged(float Current, float Max);

	UFUNCTION()
	void HideHealthBar();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void BasicAttack();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsDead = false;

};
