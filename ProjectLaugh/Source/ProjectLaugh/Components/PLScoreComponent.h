// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Components/PLActorComponent.h"
#include "ProjectLaugh/Widgets/PLIngameScoreWidget.h"
#include "PLScoreComponent.generated.h"

/**
 * Keeps track of player score
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPopupWidgetSignature, FText, ScoreReason, int32 , Score);

class UPLScoreData;
class UPLIngameScoreWidget;

UCLASS()
class PROJECTLAUGH_API UPLScoreComponent : public UPLActorComponent
{
	GENERATED_BODY()
	
public:
	UPLScoreComponent();

	UFUNCTION(BlueprintCallable)
	void AddScoreToTotal(const int32 ScoreToAdd, FString Reason = "");

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void Server_AddScoreFromTimeSurvived();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void Server_AddScoreFromPositionSurvived();

	UFUNCTION(Client, Unreliable)
	void Net_AddScorePopup(const FText& ScoreReason, int32 Score);

	UFUNCTION(BlueprintCallable)
	UPLScoreData* GetScoreData() const { return ScoreData; }

	UPROPERTY(BlueprintAssignable)
	FPopupWidgetSignature AddPopupWidgetDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "PL | Data | Score")
	UPLScoreData* ScoreData;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "PL | Score")
	int32 TotalScore;

	UPROPERTY()
	TArray<FString> OrdinalSuffix = { "st", "nd", "rd" };
};
