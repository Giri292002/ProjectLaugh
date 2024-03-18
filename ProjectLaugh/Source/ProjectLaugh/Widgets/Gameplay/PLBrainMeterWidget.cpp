// Fill out your copyright notice in the Description page of Project Settings.


#include "PLBrainMeterWidget.h"

#include "Components/ProgressBar.h" 
#include "Kismet/GameplayStatics.h"
#include "ProjectLaugh/Core/Infection/PLGameState_Infection.h"

void UPLBrainMeterWidget::PLConstruct()
{
	PLInfectionGameState = Cast<APLGameState_Infection>(UGameplayStatics::GetGameState(GetWorld()));	
	PLInfectionGameState->OnBrainMeterUpdateDelegate.AddDynamic(this, &UPLBrainMeterWidget::OnBrainMeterUpdated);
	SetVisibility(ESlateVisibility::Hidden);
}

void UPLBrainMeterWidget::OnBrainMeterUpdated(float CurrentMeter, float MaxMeter)
{
	SetVisibility(ESlateVisibility::Visible);
	BrainMeter->SetPercent(CurrentMeter / MaxMeter);
}
