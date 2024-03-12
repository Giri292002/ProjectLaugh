// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Widgets/PLComponentWidgetBase.h"
#include "PLIngameScoreWidget.generated.h"

class UVerticalBox;
class UPLScorePopup;
class UPLScoreComponent;

UCLASS()
class PROJECTLAUGH_API UPLIngameScoreWidget : public UPLComponentWidgetBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* ScorePopupWidgetContainer;

	UPROPERTY(EditDefaultsOnly, Category = "PL | UI")
	TSubclassOf<UPLScorePopup> ScorePopupClass;

	UPROPERTY()
	UPLScoreComponent* PLScoreComponent;

public:
	virtual void SetupComponent(UPLActorComponent* PLActorComponent) override;

	UFUNCTION()
	void WidgetAddScorePopup(FText ScoreReason, int32 Score);

};
