// Fill out your copyright notice in the Description page of Project Settings.


#include "PLScoreSlot.h"

#include "Components/TextBlock.h"

void UPLScoreSlot::Setup(FString Name, int Score)
{
	NameTextBlock->SetText(FText::FromString(Name));
	ScoreTextBlock->SetText(FText::FromString(FString::FromInt(Score)));
	PlayAnimation(PopInAnimation);
}
