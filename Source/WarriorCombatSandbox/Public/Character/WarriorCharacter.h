// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
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

public:

	// Sets default values for this character's properties
	AWarriorCharacter();

protected:

	// Initialize input bindings
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// Movement input
	void Move(const FInputActionValue& Value);

	// Look input
	void Look(const FInputActionValue& Value);

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

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
