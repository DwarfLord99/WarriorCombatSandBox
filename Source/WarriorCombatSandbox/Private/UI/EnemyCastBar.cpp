// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnemyCastBar.h"
#include "Components/ProgressBar.h"

void UEnemyCastBar::UpdateCastBar(float Percent)
{
	if (!CastBar)
		return;

	CastBar->SetPercent(Percent);
}
