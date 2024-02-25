// Fill out your copyright notice in the Description page of Project Settings.


#include "PLInteractionComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Rotator.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"
#include "ProjectLaugh/Core/PLPlayerController.h"
#include "ProjectLaugh/Widgets/PLCrosshairWidget.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPLInteractionComponent::UPLInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	TraceRange = 500.f;
	bCanRunInteract = true;
}

// Called when the game starts
void UPLInteractionComponent::BeginPlay()
{
	Super::BeginPlay();	

	PLPlayerCharacter = Cast<APLPlayerCharacter>(GetOwner());
}

bool UPLInteractionComponent::TryInteract()
{
	if (!IsValid(LastInteractedComponent))
	{
		return false;
	}

	if(IPLInteractionInterface::Execute_IsValidInteraction(LastInteractedComponent, InteractorType, IPLInteractionInterface::Execute_GetSupportedInteractors(LastInteractedComponent)))
	{
		IPLInteractionInterface::Execute_Interact(LastInteractedComponent, Cast<APLPlayerCharacter>(GetOwner()));
		return true;
	}
	return false;
}

bool UPLInteractionComponent::RunInteractTrace(APLPlayerController* PLPlayerController)
{
	if (!GetCanRunInteract() || !IsValid(PLPlayerController))
	{
		OnCanInteract.Broadcast(false);
		return false;
	}

	//Build Line trace directions
	PLPlayerController->GetPlayerViewPoint(StartLocation, StartRotation);
	EndLocation = StartLocation + (StartRotation.Vector() * TraceRange);
	QueryParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel10, QueryParams);

	/*DrawDebugLine(GetWorld(),
		HitResult.TraceStart,
		HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd,
		HitResult.bBlockingHit ? FColor::Green : FColor::Red,
		true);*/

	//If we cant interact
	if (!HitResult.bBlockingHit || !IsValid(HitResult.GetActor()))
	{
		LastInteractedComponent = nullptr;
		OnCanInteract.Broadcast(false);
		return false;
	}

	LastInteractedComponent = HitResult.GetActor()->FindComponentByInterface(UPLInteractionInterface::StaticClass());
	if (IsValid(LastInteractedComponent))
	{
		if (IPLInteractionInterface::Execute_IsValidInteraction(LastInteractedComponent, InteractorType, IPLInteractionInterface::Execute_GetSupportedInteractors(LastInteractedComponent)))
		{
			OnCanInteract.Broadcast(true);
			IPLInteractionInterface::Execute_IsLookingAtInteractable(LastInteractedComponent, true);
			return true;
		}
	}
	else
	{
		LastInteractedComponent = nullptr;
	}
	OnCanInteract.Broadcast(false);
	return false;
}
uint8 UPLInteractionComponent::GetSupportedInteractors_Implementation()
{
	//Current players cant interact with other players
	return uint8(EInteractorSupport::None);
}

bool UPLInteractionComponent::GetInteractionHitResult_Implementation(FHitResult& OutHitResult)
{
	OutHitResult = HitResult;
	return HitResult.bBlockingHit;
}

