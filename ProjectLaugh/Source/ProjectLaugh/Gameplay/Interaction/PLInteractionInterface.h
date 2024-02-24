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

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	bool IsValidInteraction(const uint8 InteractorType, const uint8 InteractedType);
	virtual bool IsValidInteraction_Implementation(const uint8 InteractorType, const uint8 InteractedType);

	UFUNCTION(BlueprintNativeEvent)
	void IsLookingAtInteractable(const bool bStartFocus);

	UFUNCTION(BlueprintNativeEvent)
	uint8 GetSupportedInteractors();
	virtual uint8 GetSupportedInteractors_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Interact(APLPlayerCharacter* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetInteractionHitResult(FHitResult& OutHitResult);
};
