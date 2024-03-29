// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PLInteractionInterface.generated.h"

class APLPlayerCharacter;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPLInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTLAUGH_API IPLInteractionInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void Interact(APLPlayerCharacter* InInstigator, UPLInteractionComponent* OtherInteractableComponent);

	//Inherit this in your specific classes to add extra Interaction checks 
	UFUNCTION(BlueprintNativeEvent)
	bool CanInteract(APLPlayerCharacter* InInstigator, UPLInteractionComponent* OtherInteractableComponent);
	virtual bool CanInteract_Implementation(APLPlayerCharacter* InInstigator, UPLInteractionComponent* OtherInteractableComponent);
};
