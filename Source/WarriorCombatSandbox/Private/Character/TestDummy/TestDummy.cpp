// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TestDummy/TestDummy.h"

// Sets default values
ATestDummy::ATestDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void ATestDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestDummy::RecieveDamage_Implementation(float DamageAmount)
{
	if (HealthComponent)
	{
		HealthComponent->ApplyDamage(DamageAmount);
		UE_LOG(LogTemp, Warning, TEXT("TestDummy received %f damage, current health: %f"), DamageAmount, HealthComponent->GetCurrentHealth());
	}
}

