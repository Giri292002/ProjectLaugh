// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Widgets/PLWidgetBase.h"
#include "PLWaitingForPlayersWidget.generated.h"

class UTextBlock;

UCLASS()
class PROJECTLAUGH_API UPLWaitingForPlayersWidget : public UPLWidgetBase 
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* RoomCodeText;

	void NativeConstruct() override;
};
