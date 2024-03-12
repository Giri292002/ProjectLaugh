// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectLaugh/Core/PLGameInstance.h"
#include "PLServerSlotWidget.generated.h"

class UTextBlock;
class UButton;
class UPLGameInstance;

UCLASS()
class PROJECTLAUGH_API UPLServerSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FServerInfo ServerInfo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerNameTextBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerCountTextBox;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinServerButton;

	UPROPERTY()
	UPLGameInstance* PLGameInstance;

	UFUNCTION()
	void OnJoinServer();

	virtual void NativeConstruct() override;
};
