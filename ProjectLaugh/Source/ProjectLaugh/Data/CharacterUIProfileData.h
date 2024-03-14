// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"
#include "CharacterUIProfileData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLAUGH_API UCharacterUIProfileData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CharacterProfileImage;
};
