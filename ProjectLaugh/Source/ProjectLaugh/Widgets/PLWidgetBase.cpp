// Fill out your copyright notice in the Description page of Project Settings.


#include "PLWidgetBase.h"

#include "ProjectLaugh/Core/PLEOSGameState.h"

void UPLWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	OnGetGameState.AddDynamic(this, &UPLWidgetBase::OnGetPLGameState);
	GetWorld()->GetTimerManager().SetTimer(GetGameStateTimerHandle, this, &UPLWidgetBase::GetGameState, 0.5f, true);
}

void UPLWidgetBase::GetGameState()
{
	PLGameState = GetWorld()->GetGameState<APLEOSGameState>();
	if (IsValid(PLGameState))
	{
		GetWorld()->GetTimerManager().ClearTimer(GetGameStateTimerHandle);
		OnGetGameState.Broadcast();
	}
}

void UPLWidgetBase::OnGetPLGameState()
{
	PLConstruct();
}

void UPLWidgetBase::PLConstruct()
{
}
