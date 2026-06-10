// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnemyRageBar.h"
#include "Components/ProgressBar.h"

void UEnemyRageBar::UpdateRageBar(float Current, float Max)
{
	if (!RageBar)
		return;

	float Percent = Current / Max;
	RageBar->SetPercent(Percent);
}
