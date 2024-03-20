// Fill out your copyright notice in the Description page of Project Settings.


#include "PLHidableActor.h"

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BillboardComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ProjectLaugh/Gameplay/Interaction/PLInteractableComponent.h"
#include "ProjectLaugh/Gameplay/PLGameplayTagComponent.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"

APLHidableActor::APLHidableActor()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName(TEXT("Mesh")));
	SetRootComponent(SkeletalMeshComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(FName(TEXT("Spring Arm")));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(FName(TEXT("Camera")));
	CameraComponent->SetupAttachment(SpringArmComponent);

	PLInteractableComponent = CreateDefaultSubobject<UPLInteractableComponent>(FName(TEXT("Interactable Component")));

	PLGameplayTagComponent = CreateDefaultSubobject<UPLGameplayTagComponent>(FName(TEXT("Gameplaytag Component")));

	HidingLocationMarker = CreateDefaultSubobject<USceneComponent>(FName(TEXT("Hiding Location Marker")));
	HidingLocationMarker->SetupAttachment(SkeletalMeshComponent);
	HidingLocationSprite = CreateDefaultSubobject<UBillboardComponent>(FName(TEXT("Hiding Location Sprite")));
	HidingLocationSprite->SetupAttachment(HidingLocationMarker);

	SetReplicates(true);
}

void APLHidableActor::Interact_Implementation(APLPlayerCharacter* InInstigator, UPLInteractionComponent* OtherInteractableComponent)
{
	if (!IsValid(InInstigator))
	{
		return;
	}
	InInstigator->Server_StartHiding(this);
}

void APLHidableActor::BeginPlay()
{
	Super::BeginPlay();
	HidingLocationTransform = HidingLocationMarker->GetComponentTransform();
}

void APLHidableActor::Server_StartHiding_Implementation(APLPlayerCharacter* InPLPlayerCharacter)
{

}

bool APLHidableActor::Server_StartHiding_Validate(APLPlayerCharacter* InPLPlayerCharacter)
{
	return true;
}

void APLHidableActor::Server_OpenDoor_Implementation()
{
	//Open Doors
	PLGameplayTagComponent->GetActiveGameplayTags().AddTag(SharedGameplayTags::TAG_Interactable_Status_Open);
}

bool APLHidableActor::Server_OpenDoor_Validate()
{
	return true;
}

void APLHidableActor::Server_CloseDoor_Implementation()
{
	//Close Doors
	PLGameplayTagComponent->GetActiveGameplayTags().AddTag(SharedGameplayTags::TAG_Interactable_Status_Close);
}

bool APLHidableActor::Server_CloseDoor_Validate()
{
	return true;
}