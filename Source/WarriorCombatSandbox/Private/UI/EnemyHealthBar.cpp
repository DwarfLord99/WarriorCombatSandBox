// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnemyHealthBar.h"
#include "Components/ProgressBar.h"

void UEnemyHealthBar::UpdateHealthBar(float Current, float Max)
{
	if (!HealthBar)
		return;

	float Percent = Current / Max;
	HealthBar->SetPercent(Percent);
}
