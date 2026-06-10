// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TestDummy/TestDummy.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"

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

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATestDummy::BeginPlay()
{
	Super::BeginPlay();

	CachedCameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	if (HealthBarWidget)
	{
		UEnemyHealthBar* HealthBar = Cast<UEnemyHealthBar>(HealthBarWidget->GetUserWidgetObject());
		CachedHealthBar = HealthBar;
	}

	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &ATestDummy::HandleHealthChanged);
		HealthComponent->OnDeath.AddDynamic(this, &ATestDummy::HandleDeath);
	}

	if (CachedHealthBar && HealthComponent)
	{
		CachedHealthBar->UpdateHealthBar(HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth());
	}

	CachedHealthBar->SetVisibility(ESlateVisibility::Hidden);
	
}

// Called every frame
void ATestDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HealthBarWidget) return;

	if (!CachedCameraManager) return;

	HealthBarWidget->SetWorldRotation((CachedCameraManager->GetCameraLocation() - HealthBarWidget->GetComponentLocation()).Rotation());
}

void ATestDummy::RecieveDamage_Implementation(float DamageAmount)
{
	if (HealthComponent && !bIsDead)
	{
		//HealthComponent->ApplyDamage(DamageAmount);S
		UE_LOG(LogTemp, Warning, TEXT("TestDummy received %f damage, current health: %f"), DamageAmount, HealthComponent->GetCurrentHealth());
	}
}

void ATestDummy::HandleHealthChanged(float Current, float Max)
{
	if (CachedHealthBar)
	{
		CachedHealthBar->SetVisibility(ESlateVisibility::Visible);
		CachedHealthBar->UpdateHealthBar(Current, HealthComponent->GetMaxHealth());

		GetWorldTimerManager().SetTimer(HideHealthBarTimer, this, &ATestDummy::HideHealthBar, 3.5f, false);
	}
}

void ATestDummy::HideHealthBar()
{
	CachedHealthBar->SetVisibility(ESlateVisibility::Hidden);
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

