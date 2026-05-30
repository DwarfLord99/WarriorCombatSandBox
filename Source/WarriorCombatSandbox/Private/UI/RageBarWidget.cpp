// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RageBarWidget.h"
#include "Components/ProgressBar.h"

void URageBarWidget::SetRage(float Current, float Max)
{
	if (!RageBar) return;

	float RagePercent = Current / Max;
	RageBar->SetPercent(RagePercent);
}
