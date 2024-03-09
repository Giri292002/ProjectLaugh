// Fill out your copyright notice in the Description page of Project Settings.


#include "PLScoreComponent.h"

#include "Curves/CurveFloat.h"
#include "Net/UnrealNetwork.h"
#include "ProjectLaugh/Core/PLPlayerState.h"
#include "ProjectLaugh/Core/Infection/PLGameState_Infection.h"
#include "ProjectLaugh/Data/PLScoreData.h"

UPLScoreComponent::UPLScoreComponent()
{
	SetIsReplicatedByDefault(true);
}

void UPLScoreComponent::AddScoreToTotal(const int32 ScoreToAdd, FString Reason)
{
	TotalScore += ScoreToAdd;
	UE_LOG(LogTemp, Log, TEXT("Added Score: %d, Total Score is: %d"), ScoreToAdd, TotalScore);
	Net_AddScorePopup(FText::FromString(Reason), ScoreToAdd);
}



void UPLScoreComponent::Server_AddScoreFromPositionSurvived_Implementation()
{
	int Position = GetInfectionGameState()->GetNumberOfElders();
	int Score =	FMath::TruncToInt(GetScoreData()->PositionScoreCurve->GetFloatValue(Position));
	if (Score > 0)
	{
		AddScoreToTotal(Score, FString::Printf(TEXT("Placed %i%s place. Bonus"), Position, *(OrdinalSuffix[Position - 1])));
	}
}

bool UPLScoreComponent::Server_AddScoreFromPositionSurvived_Validate()
{
	return true;
}

void UPLScoreComponent::Net_AddScorePopup_Implementation(const FText& ScoreReason, int32 Score)
{
	AddPopupWidgetDelegate.Broadcast(ScoreReason, Score);
}

void UPLScoreComponent::Server_AddScoreFromTimeSurvived_Implementation()
{
	const int TimeSurvivedInSeconds = FMath::TruncToInt(GetInfectionGameState()->GetTimeSurvived());
	const int TimeSurvivedInMinute = FMath::TruncToInt(TimeSurvivedInSeconds / 60.f);
	const bool bUseMinutes = TimeSurvivedInSeconds > 60;

	FString ReasonText = FString::Printf(TEXT("%i %s survived "), bUseMinutes ? TimeSurvivedInMinute : TimeSurvivedInSeconds, bUseMinutes ? TEXT("minutes") : TEXT("seconds"));
	const int32 Score = FMath::TruncToInt(GetScoreData()->TimeSurvivedScoreCurve->GetFloatValue(TimeSurvivedInSeconds));
	AddScoreToTotal(Score, ReasonText);
}

bool UPLScoreComponent::Server_AddScoreFromTimeSurvived_Validate()
{
	return true;
}


void UPLScoreComponent::Server_AddScoreFromConversionStreak_Implementation(const int32 ConversionStreak, bool bIsAlphaZombie)
{
	int Score = FMath::TruncToInt(bIsAlphaZombie? GetScoreData()->AlphaZombieConversionScoreCurve->GetFloatValue(ConversionStreak)
												: GetScoreData()->BetaZombieConversionScoreCurve->GetFloatValue(ConversionStreak));
	FString ReasonText = FString::Printf(TEXT("x%i %s"), ConversionStreak, ConversionStreak > 1 ? TEXT("Conversion Streak") : TEXT("Conversion"));
	AddScoreToTotal(Score, ReasonText);
}

bool UPLScoreComponent::Server_AddScoreFromConversionStreak_Validate(const int32 ConversionStreak, bool bIsAlphaZombie)
{
	return true;
}

APLGameState_Infection* UPLScoreComponent::GetInfectionGameState()
{
	if (IsValid(InfectionGameState))
	{
		return InfectionGameState;
	}
	InfectionGameState = GetWorld()->GetGameState<APLGameState_Infection>();
	check(InfectionGameState);
	return InfectionGameState;
}



void UPLScoreComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPLScoreComponent, TotalScore);
}

void UPLScoreComponent::Server_AddScoreFromConversionAssist_Implementation()
{
	AddScoreToTotal(GetScoreData()->ConversionAssistScore, FString("Conversion assist"));
}

bool UPLScoreComponent::Server_AddScoreFromConversionAssist_Validate()
{
	return true;
}