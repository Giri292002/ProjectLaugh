// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PLWaitingForPlayersWidget.generated.h"

class UTextBlock;

UCLASS()
class PROJECTLAUGH_API UPLWaitingForPlayersWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* RoomCodeText;

	void NativeConstruct() override;
};
