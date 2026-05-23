// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Character/Components/HealthComponent.h"
#include "Combat/CombatComponent.h"
#include "Combat/AbilityData.h"
#include "Weapons/Weapon.h"
#include "UI/PlayerHUD.h"
#include "WarriorCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogWarriorCharacter, Log, All);

UCLASS(abstract)
class WARRIORCOMBATSANDBOX_API AWarriorCharacter : public ACharacter
{
	GENERATED_BODY()

	// Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	// Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	// Combat Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;

	// Health Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

protected:

	// Jump Input Action
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	// Move Input Action
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction;

	// Look Input Action
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	// Mouse Look Input Action
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MouseLookAction;

	// Sprint Input Action
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* SprintAction;

	// Basic Attack Input Action
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* BasicAttackAction;

	// Heavy Attack Input Action
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* HeavyAttackAction;

	// Defense Skill Input Action
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DefenseSkillAction;

	// Interrupt Input Action
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* InterruptAction;

	// Ultimate Input Action
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* UltimateAction;

	// Basic Attack Data
	UPROPERTY(EditAnywhere, Category = Abilities)
	UAbilityData* BasicAttackData;

	// Heavy Attack Data
	UPROPERTY(EditAnywhere, Category = Abilities)
	UAbilityData* HeavyAttackData;

	// Defense Skill Data
	UPROPERTY(EditAnywhere, Category = Abilities)
	UAbilityData* DefenseSkillData;

	// Interrupt Data
	UPROPERTY(EditAnywhere, Category = Abilities)
	UAbilityData* InterruptData;

	// Ultimate Data
	UPROPERTY(EditAnywhere, Category = Abilities)
	UAbilityData* UltimateData;

	// Weapon
	UPROPERTY(EditAnywhere, Category = Equipment)
	TSubclassOf<AWeapon> WeaponClass;

	// Equipped Weapon
	UPROPERTY()
	AWeapon* EquippedWeapon;

	// Health Data
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UPlayerHUD> HUDClass;

	UPROPERTY()
	UPlayerHUD* PlayerHUD;

public:

	// Sets default values for this character's properties
	AWarriorCharacter();

protected:

	// Initialize input bindings
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	// Movement input
	void Move(const FInputActionValue& Value);

	// Look input
	void Look(const FInputActionValue& Value);

	// Sprint input
	void Sprint(const FInputActionValue& Value);

	// Basic Attack
	void HandleBasicAttack();

	// Heavy Attack
	void HandleHeavyAttack();

	// Defense Skill
	void HandleDefenseSkill();

	// Interrupt Skill
	void HandleInterrupt();

	// Ultimate Skill
	void HandleUltimate();

	// Health Changed
	UFUNCTION()
	void HandleHealthChanged(float Current, float Max);

	// Rage Changed
	UFUNCTION()
	void HandleRageChanged(float Current, float Max);

public:	

	// Handle Movement input
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	// Handle Look input
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoLook(float Yaw, float Pitch);

	// Handle Jump input
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpStart();

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpEnd();

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoAttack();

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
