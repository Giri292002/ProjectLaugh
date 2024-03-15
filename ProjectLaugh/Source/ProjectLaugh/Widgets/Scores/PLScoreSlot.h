// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PLScoreSlot.generated.h"

class UTextBlock;
class UImage;
class UCharacterUIProfileData;

UCLASS()
class PROJECTLAUGH_API UPLScoreSlot : public UUserWidget
{
	GENERATED_BODY()	

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreTextBlock;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* PopInAnimation;

	UPROPERTY(meta = (BindWidget))
	UImage* ProfileImage;

public:
	UFUNCTION()
	void Setup(FString Name, int Score, UCharacterUIProfileData* CharacterUIProfileData);
};
