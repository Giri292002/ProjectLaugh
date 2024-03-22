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
class UInputAction;
class UInputMappingContext;
class APLPlayerCharacter;
class UPLWidgetBase;
struct FInputActionValue;

UCLASS()
class PROJECTLAUGH_API APLHidableActor : public APawn, public IPLInteractionInterface
{
	GENERATED_BODY()

	APLHidableActor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* RootSceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UPLInteractableComponent* PLInteractableComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UPLGameplayTagComponent* PLGameplayTagComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* HidingLocationMarker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* ExitLocationMarker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UBillboardComponent* HidingLocationSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UBillboardComponent* ExitLocationSprite;

	UPROPERTY(EditDefaultsOnly, Category = "PL | Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "PL | Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "PL | Input")
	UInputAction* StopHidingAction;

	UPROPERTY(EditDefaultsOnly, Category = "PL | Input")
	UInputAction* OpenDoorAction;

	UPROPERTY(EditDefaultsOnly, Category = "PL | Animation")
	UAnimMontage* OpenMontage;

	UPROPERTY(EditDefaultsOnly, Category = "PL | Animation")
	UAnimMontage* CloseMontage;

	UPROPERTY(EditDefaultsOnly, Category = "PL | UI")
	TSubclassOf<UPLWidgetBase> PopupWidgetClass;

	UPROPERTY(Replicated)
	APLPlayerCharacter* OccupantCharacter;

	UFUNCTION()
	void HandleOnMontageEnded(UAnimMontage* Montage, bool Interrupted);

	UFUNCTION()
	void OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController);

	UFUNCTION()
	void ToggleDoor();

	//IPLInteractionInterface Implementation Begin
	void Interact_Implementation(APLPlayerCharacter* InInstigator, UPLInteractionComponent* OtherInteractableComponent) override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

public:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OpenDoor();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_CloseDoor();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayMontage(UAnimMontage* InMontage);

	UFUNCTION(Client, Reliable)
	void Net_StopHiding();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopHiding();

	UFUNCTION()
	void SetOccupant(APLPlayerCharacter* InOccupant);

	UPLGameplayTagComponent* GetGameplayTagComponent() const { return PLGameplayTagComponent; }

	USceneComponent* GetHidingLocationMarker() const { return HidingLocationMarker; }

	USceneComponent* GetExitLocationMarker() const { return ExitLocationMarker; }

protected:
	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void NotifyControllerChanged() override;
};
