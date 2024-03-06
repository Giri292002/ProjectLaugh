// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PLWidgetBase.generated.h"

class APLEOSGameState;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGetGameStateSignature);

UCLASS()
class PROJECTLAUGH_API UPLWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	APLEOSGameState* PLGameState;

	UPROPERTY()
	FTimerHandle GetGameStateTimerHandle;

	UFUNCTION()
	void GetGameState();

	UFUNCTION()
	virtual void OnGetPLGameState();

	//Custom Native construct that runs after we get game state
	UFUNCTION()
	virtual void PLConstruct();
	
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnGetGameStateSignature OnGetGameState;
};
