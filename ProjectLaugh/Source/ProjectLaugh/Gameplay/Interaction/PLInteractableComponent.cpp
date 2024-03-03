// Fill out your copyright notice in the Description page of Project Settings.


#include "PLInteractableComponent.h"

// Sets default values for this component's properties
UPLInteractableComponent::UPLInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UPLInteractableComponent::Interact(APLPlayerCharacter* Instigator, UPLInteractionComponent* InteractableComponent)
{
	IPLInteractionInterface::Execute_Interact(GetOwner(), Instigator, InteractableComponent);
}

bool UPLInteractableComponent::CanInteract(APLPlayerCharacter* InInstigator, UPLInteractionComponent* OtherInteractableComponent)
{
	return IPLInteractionInterface::Execute_CanInteract(GetOwner(), InInstigator, OtherInteractableComponent);
}
