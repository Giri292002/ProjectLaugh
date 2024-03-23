// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Components/PLActorComponent.h"
#include "PLHideComponent.generated.h"

class APLHidableActor;

UCLASS()
class PROJECTLAUGH_API UPLHideComponent : public UPLActorComponent
{
	GENERATED_BODY()

public:
	UPLHideComponent();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OpenDoor(APLHidableActor* InHidableActor);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ToggleDoor(APLHidableActor* InHidableActor);

	UFUNCTION(Client, Reliable)
	void Net_StartHiding(APLHidableActor* InHideableActor);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartHiding(APLHidableActor* InHideableActor);

	UFUNCTION(Client, Reliable)
	void Net_StopHiding(APLHidableActor* InHideableActor);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopHiding(APLHidableActor* InHideableActor);
	
};
