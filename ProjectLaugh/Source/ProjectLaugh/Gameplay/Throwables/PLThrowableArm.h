// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Gameplay/Throwables/PLThrowableBase.h"
#include "PLThrowableArm.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLAUGH_API APLThrowableArm : public APLThrowableBase
{
	GENERATED_BODY()

	APLThrowableArm(); 	

	virtual void Interact_Implementation(APLPlayerCharacter* InInstigator, UPLInteractionComponent* OtherInteractableComponent) override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Destroy();
};
