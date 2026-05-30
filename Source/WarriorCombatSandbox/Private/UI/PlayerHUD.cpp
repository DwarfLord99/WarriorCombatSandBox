// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PlayerHUD.h"
#include "UI/PlayerHUDWidget.h"
#include "UI/HealthBarWidget.h"
#include "UI/RageBarWidget.h"
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

	if (PlayerHUDWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerHUDWidgetClass IS VALID"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerHUDWidgetClass IS NULL"));
	}


	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("HUD SPAWNED"));

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
