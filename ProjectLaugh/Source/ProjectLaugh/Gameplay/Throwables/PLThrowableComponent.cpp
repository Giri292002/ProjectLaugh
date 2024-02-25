// Fill out your copyright notice in the Description page of Project Settings.


#include "PLThrowableComponent.h"
#include "ProjectLaugh/Gameplay/Throwables/PLThrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"

// Sets default values for this component's properties
UPLThrowableComponent::UPLThrowableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bShouldBounce = true;
	Bounciness = 0.2f;
	bInterpMovement = true;
	bAutoActivate = false;
	bThrottleInterpolation = true;
}


// Called when the game starts
void UPLThrowableComponent::BeginPlay()
{
	Super::BeginPlay();
	OnProjectileStop.AddDynamic(this, &UPLThrowableComponent::OnProjectileStopped);
	OnProjectileStopped(FHitResult());
}

void UPLThrowableComponent::OnProjectileStopped(const FHitResult& ImpactResult)
{
	Cast<AStaticMeshActor>(GetOwner())->GetStaticMeshComponent()->SetSimulatePhysics(true);
}

void UPLThrowableComponent::DisableInteractionOutline()
{
	Execute_IsLookingAtInteractable(this, false);
}

void UPLThrowableComponent::Interact_Implementation(APLPlayerCharacter* Instigator)
{
	UPLThrowComponent* ThrowComponent = Instigator->FindComponentByClass<UPLThrowComponent>();
	if (!IsValid(ThrowComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Throw component is invalid on %s"), *GetNameSafe(Instigator));
		return;
	}
	ThrowComponent->Net_HoldObject(GetOwner());
}

uint8 UPLThrowableComponent::GetSupportedInteractors_Implementation()
{
	return SupportedInteractors;
}

void UPLThrowableComponent::IsLookingAtInteractable_Implementation(const bool bStartFocus)
{
	if (InteractionLookAtTimeHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractionLookAtTimeHandle);
	}
	Cast<AStaticMeshActor>(GetOwner())->GetStaticMeshComponent()->SetRenderCustomDepth(bStartFocus);
	if (bStartFocus)
	{
		GetWorld()->GetTimerManager().SetTimer(InteractionLookAtTimeHandle, this, &UPLThrowableComponent::DisableInteractionOutline, 0.05f);
	}
}


// Called every frame
void UPLThrowableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

