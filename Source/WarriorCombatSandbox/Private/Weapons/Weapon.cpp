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

	// Debug sphere at blade center
	FVector BladeCenter = WeaponMesh->GetSocketLocation(BladeCenterSocketName);
	DrawDebugSphere(
		GetWorld(),
		BladeCenter,
		50.f,
		12,
		FColor::Red,
		false,
		-1.f
	);

}

void AWeapon::StartTrace()
{
	HitActors.Empty();

	PrevCenter = WeaponMesh->GetSocketLocation(BladeCenterSocketName);
}

void AWeapon::EndTrace()
{
	HitActors.Empty();
}

AActor* AWeapon::PerformTrace()
{
	CurrentCenter = WeaponMesh->GetSocketLocation(BladeCenterSocketName);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActors(HitActors);

	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		PrevCenter,
		CurrentCenter,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(50.f),
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

	DrawDebugSphere(
		GetWorld(),
		CurrentCenter,
		50.f,
		12,
		FColor::Green,
		false,
		-1.f
	);

	PrevCenter = CurrentCenter;
	return nullptr;
}

