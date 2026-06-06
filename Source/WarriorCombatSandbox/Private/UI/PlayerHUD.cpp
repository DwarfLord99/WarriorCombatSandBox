// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PlayerHUD.h"
#include "UI/PlayerHUDWidget.h"
#include "UI/HealthBarWidget.h"
#include "UI/RageBarWidget.h"
#include "UI/AbilityBarWidget.h"
#include "Character/Player/WarriorCharacter.h"
#include "Components/PanelWidget.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerHUDWidgetClass)
	{
		PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
		PlayerHUDWidget->AddToViewport();
	}

	if (HealthBarClass)
	{
		HealthBar = CreateWidget<UHealthBarWidget>(GetWorld(), HealthBarClass);

		UPanelWidget* Anchor = Cast<UPanelWidget>(PlayerHUDWidget->HealthBarAnchor);
		if (Anchor)
		{
			Anchor->AddChild(HealthBar);
		}
	}

	if (RageBarClass)
	{
		RageBar = CreateWidget<URageBarWidget>(GetWorld(), RageBarClass);

		UPanelWidget* Anchor = Cast<UPanelWidget>(PlayerHUDWidget->RageBarAnchor);
		if (Anchor)
		{
			Anchor->AddChild(RageBar);
		}
	}

	if (AbilityBarClass && PlayerHUDWidget && PlayerHUDWidget->AbilityBarAnchor)
	{
		AbilityBar = CreateWidget<UAbilityBarWidget>(GetWorld(), AbilityBarClass);

		UPanelWidget* Anchor = Cast<UPanelWidget>(PlayerHUDWidget->AbilityBarAnchor);
		if (Anchor)
		{
			Anchor->AddChild(AbilityBar);
		}
	}

	AWarriorCharacter* Warrior = Cast<AWarriorCharacter>(GetOwningPlayerController()->GetPawn());

	if (AbilityBar)
	{
		AbilityBar->InitializeBar(Warrior->AbilitySystem, 3);
	}
}

void APlayerHUD::UpdateHealth(float Current, float Max)
{
	if (HealthBar)
	{
		HealthBar->SetHealth(Current, Max);
	}
}

void APlayerHUD::UpdateRage(float Current, float Max)
{
	if (RageBar)
	{
		RageBar->SetRage(Current, Max);
	}
}
