// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PLCrosshairWidget.generated.h"

/**
 * 
 */
class UPLInteractionComponent;

UCLASS()
class PROJECTLAUGH_API UPLCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPLInteractionComponent(UPLInteractionComponent* InPLInteractionComponent) { PLInteractionComponent = InPLInteractionComponent; }

protected:
	UPROPERTY()
	UPLInteractionComponent* PLInteractionComponent;

	void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnCanInteract(const bool bCanInteract);
	
};
