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
