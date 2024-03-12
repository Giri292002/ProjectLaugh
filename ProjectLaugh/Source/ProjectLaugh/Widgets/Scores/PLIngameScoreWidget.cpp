// Fill out your copyright notice in the Description page of Project Settings.


#include "PLIngameScoreWidget.h"

#include "ProjectLaugh/Components/PLScoreComponent.h"
#include"Components/VerticalBox.h"
#include "PLScorePopup.h"

void UPLIngameScoreWidget::SetupComponent(UPLActorComponent* PLActorComponent)
{
	PLScoreComponent = Cast<UPLScoreComponent>(PLActorComponent);
	PLScoreComponent->AddPopupWidgetDelegate.AddDynamic(this, &UPLIngameScoreWidget::WidgetAddScorePopup);
}

void UPLIngameScoreWidget::WidgetAddScorePopup(FText ScoreReason, int32 Score)
{
	checkf(ScorePopupClass, TEXT("Invalid score popup class"));
	UPLScorePopup* Popup = CreateWidget<UPLScorePopup>(GetWorld(), ScorePopupClass);
	ScorePopupWidgetContainer->AddChildToVerticalBox(Popup);
	Popup->SetupAndPlay(ScoreReason, Score);
}
