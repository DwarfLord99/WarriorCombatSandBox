// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"
#include <Components/BoxComponent.h>

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(Root);

	WeaponCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollider"));
	WeaponCollider->SetupAttachment(WeaponMesh);
	WeaponCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::StartTrace()
{
	HitActors.Empty();

	PrevBase = WeaponMesh->GetSocketLocation(BaseSocketName);
	PrevTip = WeaponMesh->GetSocketLocation(TipSocketName);
}

void AWeapon::EndTrace()
{
	HitActors.Empty();
}

AActor* AWeapon::PerformTrace()
{
	FVector CurrentBase = WeaponMesh->GetSocketLocation(BaseSocketName);
	FVector CurrentTip = WeaponMesh->GetSocketLocation(TipSocketName);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActors(HitActors);

	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		PrevTip,
		CurrentTip,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(10.f),
		Params);

	if (bHit)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			HitActors.Add(HitActor);
			return HitActor;
		}
	}

	// Debug: draw the tip movement
	DrawDebugLine(
		GetWorld(),
		PrevTip,
		CurrentTip,
		FColor::Red,
		false,
		0.05f,
		0,
		1.5f
	);

	// Debug: draw the base movement
	DrawDebugLine(
		GetWorld(),
		PrevBase,
		CurrentBase,
		FColor::Blue,
		false,
		0.05f,
		0,
		1.5f
	);

	// Debug: draw the blade segment
	DrawDebugLine(
		GetWorld(),
		CurrentBase,
		CurrentTip,
		FColor::Yellow,
		false,
		0.05f,
		0,
		1.5f
	);

	// Debug: draw hit point
	if (bHit)
	{
		DrawDebugSphere(
			GetWorld(),
			Hit.ImpactPoint,
			8.f,
			12,
			FColor::Green,
			false,
			0.1f
		);
	}


	return nullptr;
}

