// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PLPlayerProfileSlot.generated.h"

class UImage;
class UCharacterUIProfileData;
UCLASS()
class PROJECTLAUGH_API UPLPlayerProfileSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* ProfileImage;

public:
	void Setup(UCharacterUIProfileData* UIProfileData);
	
};
