// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PLInteractionInterface.generated.h"

class APLPlayerCharacter;


//Enum to determine who can control an object
UENUM(BlueprintType, meta = (Bitflags))
	enum class EInteractorSupport : uint8
{
	None = 0 UMETA(Hidden),
	Elder = 1,
	Zombie = 2
};


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
};
