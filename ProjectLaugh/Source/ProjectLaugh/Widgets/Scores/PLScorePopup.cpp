// Fill out your copyright notice in the Description page of Project Settings.


#include "PLScorePopup.h"

#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

void UPLScorePopup::AnimationFinished()
{
	RemoveFromParent();
}

void UPLScorePopup::SetupAndPlay(FText ScoreReason, int32 Score)
{
	ScoreReasonText->SetText(ScoreReason);
	ScoreText->SetText(FText::FromString("+" + FString::FromInt(Score)));
	FWidgetAnimationDynamicEvent AnimationDelegate;
	AnimationDelegate.BindUFunction(this, FName("AnimationFinished"));
	BindToAnimationFinished(PopupAndFadeAnimation, AnimationDelegate);
	PlayAnimation(PopupAndFadeAnimation);
}
