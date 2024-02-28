// Fill out your copyright notice in the Description page of Project Settings.


#include "PLInteractionComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/Rotator.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"
#include "ProjectLaugh/Core/PLPlayerController.h"
#include "ProjectLaugh/Gameplay/Interaction/PLInteractableComponent.h"
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

	if (IsValidInteractionWith(LastInteractedComponent))
	{
		LastInteractedComponent->Interact(PLPlayerCharacter, this);
		return true;
	}
	return false;
}

bool UPLInteractionComponent::RunInteractTrace(APLPlayerController* PLPlayerController)
{
	if (!GetCanRunInteract() || !IsValid(PLPlayerController))
	{
		UnassignInteractableComponent();
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
	if (!HitResult.bBlockingHit || !IsValid(HitResult.GetActor()) || !UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UPLInteractionInterface::StaticClass()))
	{
		LastInteractedComponent = nullptr;
		OnCanInteract.Broadcast(false);
		return false;
	}

	UPLInteractableComponent* InteractableComponent = HitResult.GetActor()->FindComponentByClass<UPLInteractableComponent>();

	if (!IsValid(InteractableComponent))
	{
		UnassignInteractableComponent();
		return false;
	}
	else
	{
		UnassignInteractableComponent();
	}

	AssignInteractableComponent(InteractableComponent);

	if (IsValidInteractionWith(LastInteractedComponent))
	{
		OnCanInteract.Broadcast(true);
		return true;
	}	
	UnassignInteractableComponent();
	OnCanInteract.Broadcast(false);
	return false;
}

void UPLInteractionComponent::AssignInteractableComponent(UPLInteractableComponent* InteractableComponent)
{
	//Call to draw outline
	LastInteractedComponent = InteractableComponent;
	UStaticMeshComponent* MeshComponent = LastInteractedComponent->GetOwner()->GetComponentByClass<UStaticMeshComponent>();
	if (IsValid(MeshComponent))
	{
		MeshComponent->SetRenderCustomDepth(true);
	}
}

void UPLInteractionComponent::UnassignInteractableComponent()
{
	if (IsValid(LastInteractedComponent))
	{
		UStaticMeshComponent* MeshComponent = LastInteractedComponent->GetOwner()->GetComponentByClass<UStaticMeshComponent>();
		if (IsValid(MeshComponent))
		{
			MeshComponent->SetRenderCustomDepth(false);
		}
		LastInteractedComponent = nullptr;
	}
}

bool UPLInteractionComponent::IsValidInteractionWith(UPLInteractableComponent* InteractableComponent)
{
	return InteractorType & InteractableComponent->GetSupportedInteractors();
}

