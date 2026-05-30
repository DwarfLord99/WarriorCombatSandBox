// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HealthBarWidget.h"
#include "Components/ProgressBar.h"

void UHealthBarWidget::SetHealth(float Current, float Max)
{
	if (!HealthBar) return;

	float HealthPercent = Current / Max;
	HealthBar->SetPercent(HealthPercent);
}
