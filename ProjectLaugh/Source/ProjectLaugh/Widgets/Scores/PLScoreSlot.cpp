// Fill out your copyright notice in the Description page of Project Settings.


#include "PLScoreSlot.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "ProjectLaugh/Data/CharacterUIProfileData.h"

void UPLScoreSlot::Setup(FString Name, int Score, UCharacterUIProfileData* CharacterUIProfileData)
{
	NameTextBlock->SetText(FText::FromString(Name));
	ScoreTextBlock->SetText(FText::FromString(FString::FromInt(Score)));
	ProfileImage->SetBrushFromTexture(CharacterUIProfileData->CharacterProfileImage);
	PlayAnimation(PopInAnimation);
}
