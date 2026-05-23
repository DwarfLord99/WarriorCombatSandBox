// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PlayerHUD.h"
#include "Components/ProgressBar.h"

void UPlayerHUD::UpdateHealth(float Current, float Max)
{
	if (HealthBar)
	{
		float HealthPercent = Current / Max;
		UE_LOG(LogTemp, Log, TEXT("Updating Health: Current = %f, Max = %f, Percent = %f"), Current, Max, HealthPercent);
		HealthBar->SetPercent(HealthPercent);
	}
}

void UPlayerHUD::UpdateRage(float Current, float Max)
{
	if (RageBar)
	{
		float RagePercent = Current / Max;
		RageBar->SetPercent(RagePercent);
	}
}
