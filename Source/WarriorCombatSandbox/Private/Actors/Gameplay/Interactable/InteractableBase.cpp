// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Gameplay/Interactable/InteractableBase.h"
#include "Components/BoxComponent.h"
#include "Character/Player/WarriorCharacter.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create and set up the interaction volume
	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	RootComponent = InteractionVolume;

	InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	InteractionVolume->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);


	// Bind overlap events
	InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AInteractableBase::OnOverlapBegin);
	InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &AInteractableBase::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractableBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AWarriorCharacter::StaticClass()))
	{
		AWarriorCharacter* Player = Cast<AWarriorCharacter>(OtherActor);
		Player->AddNearbyInteractable(this);
	}
}

void AInteractableBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AWarriorCharacter::StaticClass()))
	{
		AWarriorCharacter* Player = Cast<AWarriorCharacter>(OtherActor);
		Player->RemoveNearbyInteractable(this);
	}
}

void AInteractableBase::Interact()
{
	if (CanInteract())
	{
		ExecuteInteraction();
	}
}

bool AInteractableBase::CanInteract_Implementation()
{
	return true;
}

