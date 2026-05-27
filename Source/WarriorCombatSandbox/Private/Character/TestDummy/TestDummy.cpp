// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TestDummy/TestDummy.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ATestDummy::ATestDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void ATestDummy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ATestDummy::HandleDeath);
	}
	
}

// Called every frame
void ATestDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestDummy::RecieveDamage_Implementation(float DamageAmount)
{
	if (HealthComponent && !bIsDead)
	{
		HealthComponent->ApplyDamage(DamageAmount);
		UE_LOG(LogTemp, Warning, TEXT("TestDummy received %f damage, current health: %f"), DamageAmount, HealthComponent->GetCurrentHealth());
	}
}

void ATestDummy::HandleDeath()
{
	bIsDead = true;

	// Disable collision
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UE_LOG(LogTemp, Warning, TEXT("TestDummy died"));

	// Play death montage
	if (DeathMontage)
	{
		UAnimInstance* Anim = MeshComponent->GetAnimInstance();
		if (Anim)
		{
			Anim->Montage_Play(DeathMontage);
		}
	}

	// Despawn after delay
	SetLifeSpan(4.f);
}

