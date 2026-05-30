// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/Components/HealthComponent.h"
#include "Combat/Damageable.h"
#include "UI/EnemyHealthBar.h"
#include "TestDummy.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class UWidgetComponent;

UCLASS(abstract)
class WARRIORCOMBATSANDBOX_API ATestDummy : public AActor, public IDamageable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestDummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

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


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void RecieveDamage_Implementation(float DamageAmount) override;

	UFUNCTION()
	void HandleDeath();

	UFUNCTION()
	void HandleHealthChanged(float Current, float Max);

	UFUNCTION()
	void HideHealthBar();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsDead = false;
};
