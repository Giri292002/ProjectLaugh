// Fill out your copyright notice in the Description page of Project Settings.


#include "PLPlayerProfileSlot.h"

#include "Components/Image.h"
#include "ProjectLaugh/Data/CharacterUIProfileData.h"

void UPLPlayerProfileSlot::Setup(UCharacterUIProfileData* UIProfileData)
{
	ProfileImage->SetBrushFromTexture(UIProfileData->CharacterProfileImage);
}
