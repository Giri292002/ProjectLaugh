// Fill out your copyright notice in the Description page of Project Settings.


#include "PLInteractionInterface.h"

// Add default functionality here for any IPLInteractionInterface functions that are not pure virtual.

bool IPLInteractionInterface::IsValidInteraction_Implementation(const uint8 InteractorType, const uint8 InteractedType)
{
	return InteractorType & InteractedType;
}

uint8 IPLInteractionInterface::GetSupportedInteractors_Implementation()
{
	check(0 && "You must override this");
	return -1;
}
