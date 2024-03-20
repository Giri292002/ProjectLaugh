// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ProjectLaugh/Gameplay/Interaction/PLInteractionInterface.h"
#include "PLHidableActor.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class UPLInteractableComponent;
class UPLGameplayTagComponent;
class USceneComponent;
class UBillboardComponent;

UCLASS()
class PROJECTLAUGH_API APLHidableActor : public APawn, public IPLInteractionInterface
{
	GENERATED_BODY()

	APLHidableActor();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UPLInteractableComponent* PLInteractableComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UPLGameplayTagComponent* PLGameplayTagComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* HidingLocationMarker;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBillboardComponent* HidingLocationSprite;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartHiding(APLPlayerCharacter* InPLPlayerCharacter);

	UPROPERTY()
	FTransform HidingLocationTransform;

	//IPLInteractionInterface Implementation Begin
	void Interact_Implementation(APLPlayerCharacter* InInstigator, UPLInteractionComponent* OtherInteractableComponent) override;

	virtual void BeginPlay() override;

public:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OpenDoor();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_CloseDoor();

	UPLGameplayTagComponent* GetGameplayTagComponent() const { return PLGameplayTagComponent; }

	USceneComponent* GetHidingLocationMarker() const { return HidingLocationMarker; }
};
