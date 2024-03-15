// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Widgets/PLComponentWidgetBase.h"
#include "PLCrosshairWidget.generated.h"


class UPLInteractionComponent;
class UPLActorComponent;

UCLASS()
class PROJECTLAUGH_API UPLCrosshairWidget : public UPLComponentWidgetBase
{
	GENERATED_BODY()

public:
	virtual void SetupComponent(UPLActorComponent* PLActorComponent) override;

protected:
	UPROPERTY()
	UPLInteractionComponent* PLInteractionComponent; 

	UFUNCTION(BlueprintImplementableEvent)
	void OnCanInteract(const bool bCanInteract);
	
};
