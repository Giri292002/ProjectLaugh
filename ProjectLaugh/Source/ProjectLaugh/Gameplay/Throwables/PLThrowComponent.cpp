// Fill out your copyright notice in the Description page of Project Settings.


#include "PLThrowComponent.h"

#include "Engine/StaticMeshActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "ProjectLaugh/Core/PLPlayerController.h"
#include "ProjectLaugh/Gameplay/Interaction/PLInteractionComponent.h"
#include "ProjectLaugh/Gameplay/Interaction/PLInteractionInterface.h"
#include "ProjectLaugh/Gameplay/Throwables/PLThrowableComponent.h"

// Sets default values for this component's properties
UPLThrowComponent::UPLThrowComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	ThrowRange = 2500.f;
	// ...
}


// Called when the game starts
void UPLThrowComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UPLThrowComponent::Net_HoldObject_Implementation(AActor* ObjectToHold)
{
	CurrentlyHoldingObject = ObjectToHold;
	if (!(GetOwner()->HasAuthority()))
	{
		Server_HoldObject(ObjectToHold);
	}
	else
	{
		Multicast_HoldObject(ObjectToHold);
	}
}

void UPLThrowComponent::Server_HoldObject_Implementation(AActor* ObjectToHold)
{
	CurrentlyHoldingObject = ObjectToHold;
	Multicast_HoldObject(ObjectToHold);
}

bool UPLThrowComponent::Server_HoldObject_Validate(AActor* ObjectToHold)
{
	return true;
}

void UPLThrowComponent::Multicast_HoldObject_Implementation(AActor* ObjectToHold)
{	
	Cast<AStaticMeshActor>(ObjectToHold)->GetStaticMeshComponent()->SetSimulatePhysics(false);
	ObjectToHold->SetActorEnableCollision(false);
	ObjectToHold->FindComponentByClass<UPLThrowableComponent>()->Deactivate();
	ObjectToHold->FindComponentByClass<UPLThrowableComponent>()->SetUpdatedComponent(nullptr);
	ObjectToHold->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void UPLThrowComponent::Net_Throw_Implementation(APLPlayerController* PLPlayerController)
{
	if (!IsValid(CurrentlyHoldingObject))
	{
		//TODO: Call drop object here and then pickup. For now just dont do anything
		return;
	}

	//Get Player aim direction
	FVector CameraLocation;
	FRotator CameraRotation;
	PLPlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FHitResult HitResult;
	//Build Line trace directions
	const FVector EndLocation = CameraLocation + (CameraRotation.Vector() * 500000.f);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel10, QueryParams);
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd);
	const FVector LaunchVelocity = LookAtRotation.Vector()* GetThrowRange();

	//Request server to throw
	if (!(GetOwner()->HasAuthority()))
	{
		Server_ThrowObject(CurrentlyHoldingObject, LaunchVelocity);
	}
	else
	{
		Multicast_Throw(CurrentlyHoldingObject, LaunchVelocity);
		CurrentlyHoldingObject = nullptr;
		//If you are server just throw
	}
}

void UPLThrowComponent::Server_ThrowObject_Implementation(AActor* ObjectToThrow, FVector LaunchVelocity)
{
	CurrentlyHoldingObject = nullptr;
	Multicast_Throw(ObjectToThrow, LaunchVelocity);
}

bool UPLThrowComponent::Server_ThrowObject_Validate(AActor* ObjectToThrow, FVector LaunchVelocity)
{
	return true;
}

void UPLThrowComponent::Multicast_Throw_Implementation(AActor* HoldingObject, FVector LaunchVelocity)
{
	HoldingObject->SetActorEnableCollision(true);
	UPLThrowableComponent* Comp = HoldingObject->FindComponentByClass<UPLThrowableComponent>();
	checkf(Comp, TEXT("Comp is invalid"));
	Cast<AStaticMeshActor>(HoldingObject)->GetStaticMeshComponent()->MoveIgnoreActors.Add(GetOwner());
	HoldingObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Comp->SetUpdatedComponent(Cast<AStaticMeshActor>(HoldingObject)->GetStaticMeshComponent());
	Comp->Velocity = LaunchVelocity;
	Comp->Activate(true);
}


void UPLThrowComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{ 
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPLThrowComponent, CurrentlyHoldingObject);
}

