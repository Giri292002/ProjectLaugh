// Fill out your copyright notice in the Description page of Project Settings.


#include "PLInteractableComponent.h"

// Sets default values for this component's properties
UPLInteractableComponent::UPLInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPLInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...	
}


bool UPLInteractableComponent::IsValidInteraction_Implementation(uint8 InteractorType)
{
	return InteractorType & IPLInteractionInterface::Execute_GetSupportedInteractors(this);
}

uint8 UPLInteractableComponent::GetSupportedInteractors_Implementation()
{
	return SupportedInteractors;
}

// Called every frame
void UPLInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

