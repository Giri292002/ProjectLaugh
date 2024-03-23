// Fill out your copyright notice in the Description page of Project Settings.


#include "PLHideComponent.h"

#include "ProjectLaugh/Gameplay/PLHidableActor.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"

UPLHideComponent::UPLHideComponent()
{
	SetIsReplicatedByDefault(true);
}

void UPLHideComponent::Server_ToggleDoor_Implementation(APLHidableActor* InHidableActor)
{
	InHidableActor->ToggleDoor();
}

bool UPLHideComponent::Server_ToggleDoor_Validate(APLHidableActor* InHidableActor)
{
	return true;
}

void UPLHideComponent::Server_OpenDoor_Implementation(APLHidableActor* InHidableActor)
{
	InHidableActor->Server_OpenDoor();
}

bool UPLHideComponent::Server_OpenDoor_Validate(APLHidableActor* InHidableActor)
{
	return true;
}

void UPLHideComponent::Net_StartHiding_Implementation(APLHidableActor* InHideableActor)
{
	if (!IsValid(InHideableActor))
	{
		return;
	}

	APLPlayerCharacter* PLPlayerCharacter = Cast<APLPlayerCharacter>(GetOwner());

	if (!IsValid(PLPlayerCharacter))
	{
		return;
	}

	InHideableActor->Server_OpenDoor();
	PLPlayerCharacter->Net_ToggleFreezeCharacter(true);
	//Move character in closet
	PLPlayerCharacter->GetGameplayTagComponent()->Server_AddTag(SharedGameplayTags::TAG_Ability_Hide_Hiding);
	PLPlayerCharacter->SetActorTransform(InHideableActor->GetHidingLocationMarker()->GetComponentTransform());
	PLPlayerCharacter->GetPLPlayerController()->SetControlRotation(InHideableActor->GetActorRotation());
	PLPlayerCharacter->Net_Crouch(true);
	Server_StartHiding(InHideableActor);
}

void UPLHideComponent::Server_StartHiding_Implementation(APLHidableActor* InHideableActor)
{
	if (!IsValid(InHideableActor))
	{
		return;
	}

	APLPlayerCharacter* PLPlayerCharacter = Cast<APLPlayerCharacter>(GetOwner());

	if (!IsValid(PLPlayerCharacter))
	{
		return;
	}

	InHideableActor->SetOccupant(PLPlayerCharacter); 
	InHideableActor->Server_OpenDoor();
	PLPlayerCharacter->SetActorTransform(InHideableActor->GetHidingLocationMarker()->GetComponentTransform());
	PLPlayerCharacter->GetPLPlayerController()->SetViewTarget(InHideableActor);
	PLPlayerCharacter->GetPLPlayerController()->bAutoManageActiveCameraTarget = false;
	PLPlayerCharacter->GetPLPlayerController()->SetControlRotation(InHideableActor->GetActorRotation());
	PLPlayerCharacter->GetPLPlayerController()->Possess(InHideableActor);
}

bool UPLHideComponent::Server_StartHiding_Validate(APLHidableActor* InHideableActor)
{
	return true;
}

void UPLHideComponent::Net_StopHiding_Implementation(APLHidableActor* InHideableActor)
{
	if (!IsValid(InHideableActor))
	{
		return;
	}

	APLPlayerCharacter* PLPlayerCharacter = Cast<APLPlayerCharacter>(GetOwner());

	if (!IsValid(PLPlayerCharacter))
	{
		return;
	}

	PLPlayerCharacter->SetActorLocation(InHideableActor->GetExitLocationMarker()->GetComponentLocation());
	PLPlayerCharacter->Net_Crouch(false);
	PLPlayerCharacter->GetGameplayTagComponent()->Server_RemoveTag(SharedGameplayTags::TAG_Ability_Hide_Hiding);
	PLPlayerCharacter->Net_ToggleFreezeCharacter(false);
	Server_StopHiding(InHideableActor);
}

void UPLHideComponent::Server_StopHiding_Implementation(APLHidableActor* InHideableActor)
{
	if (!IsValid(InHideableActor))
	{
		return;
	}

	APLPlayerCharacter* PLPlayerCharacter = Cast<APLPlayerCharacter>(GetOwner());

	if (!IsValid(PLPlayerCharacter))
	{
		return;
	}

	InHideableActor->SetOccupant(nullptr);
	PLPlayerCharacter->SetActorLocation(InHideableActor->GetExitLocationMarker()->GetComponentLocation());
	PLPlayerCharacter->GetPLPlayerController()->Possess(PLPlayerCharacter);
}

bool UPLHideComponent::Server_StopHiding_Validate(APLHidableActor* InHideableActor)
{
	return true;
}

