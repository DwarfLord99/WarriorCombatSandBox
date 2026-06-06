// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class WARRIORCOMBATSANDBOX_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UShapeComponent* WeaponCollider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName BaseSocketName = "Weapon_Base";

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TipSocketName = "Weapon_Tip";

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName BladeCenterSocketName = "Weapon_BladeCenter";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Pose")
	FVector CombatOffset = FVector(-24.f, -8.f, -10.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Pose")
	FRotator CombatRotation = FRotator(0.f, 110.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Pose")
	FVector CarryOffset = FVector(-21.f, -8.f, -9.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Pose")
	FRotator CarryRotation = FRotator(110.f, 0.f, 90.f);

	// For spear/halberd/polearm weapons
	FVector PrevBase;
	FVector PrevTip;

	// For axe/mace/hammer style weapons
	FVector PrevCenter;
	FVector CurrentCenter;

	UPROPERTY()
	TArray<AActor*> HitActors;

	void StartTrace();
	void EndTrace();
	AActor* PerformTrace();
};
