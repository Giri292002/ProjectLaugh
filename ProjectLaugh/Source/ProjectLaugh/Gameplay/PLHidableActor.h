// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ProjectLaugh/Gameplay/Interaction/PLInteractionInterface.h"
#include "ProjectLaugh/Core/System/PLResetInterface.h"
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
class UPLStunData;
struct FInputActionValue;

UCLASS()
class PROJECTLAUGH_API APLHidableActor : public APawn, public IPLInteractionInterface, public IPLResetInterface
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

	UPROPERTY(EditDefaultsOnly, Category = "PL | UI")
	TSubclassOf<UPLWidgetBase> PopupWidgetClass;

	//We are going to be using the physics asset to register hit. This array has the bones we need to be registering hits for
	UPROPERTY(EditDefaultsOnly, Category = "PL | Gameplay")
	TArray<FName> AcceptedCollisionBoneNames;

	UPROPERTY(EditDefaultsOnly, Category = "PL | Gameplay")
	UPLStunData* PLStunData;

	UPROPERTY(Replicated)
	APLPlayerCharacter* OccupantCharacter;

	UFUNCTION()
	void HandleOnMontageEnded(UAnimMontage* Montage, bool Interrupted);

	UFUNCTION()
	void OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController);



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

	UFUNCTION()
	void ToggleDoor();

	UFUNCTION(Client, Reliable)
	void Net_StopHiding();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopHiding();

	UFUNCTION()
	void SetOccupant(APLPlayerCharacter* InOccupant);

	UFUNCTION()
	void OnSkeletalMeshComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPLGameplayTagComponent* GetGameplayTagComponent() const { return PLGameplayTagComponent; }

	USceneComponent* GetHidingLocationMarker() const { return HidingLocationMarker; }

	USceneComponent* GetExitLocationMarker() const { return ExitLocationMarker; }

	// --- PLResetInterface Implementation begin ---
	virtual void PLReset_Implementation() override;
	// --- PLResetInterface Implementation End ---

protected:
	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void NotifyControllerChanged() override;
};
