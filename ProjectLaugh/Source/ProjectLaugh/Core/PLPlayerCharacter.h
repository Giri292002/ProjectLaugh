// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "ProjectLaugh/ProjectLaughCharacter.h"
#include "PLPlayerCharacter.generated.h"

class UPLPlayerAttributesData;
class UPLInhalerComponent;
class UPLInteractionComponent;
class UPLThrowComponent;
class UPLStunData;
class APLPlayerController;
class AController;
class UPLGameplayTagComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClientControllerPossesSignature, AController*, NewController);

UCLASS()
class PROJECTLAUGH_API APLPlayerCharacter : public AProjectLaughCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APLPlayerCharacter();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL | Data")
	UPLPlayerAttributesData* PLPlayerAttributesData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL | Data")
	UPLStunData* PLStunData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPLInteractionComponent* PLInteractionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPLThrowComponent* PLThrowComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPLGameplayTagComponent* PLGameplayTagComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrowAction;

	UPROPERTY(BlueprintReadOnly, Replicated)
	APLPlayerController* PLPlayerController; 

	UPROPERTY()
	FTimerHandle StunTimerHandle;

	//Is the character currently frozen, meaning they can't move but can still move the camera around and take in input
	UPROPERTY()
	bool bFrozen;

	//Shared timeline for both appearing and disappearing
	UPROPERTY(Replicated)
	FTimeline AppearanceTimeline;
	/*
	* Plays the appearance timeline
	* @param TimelinePlayLength how long the timeline should play
	*/
	UFUNCTION()
	void PlayAppearanceTimeline(const float TimelinePlayLength);

	/*
	* Plays the appeareance timeline in reverse 
	* @param TimelinePlayLength how long the timeline should play
	*/
	UFUNCTION()
	void PlayDisappearanceTimeline(const float TimelinePlayLength);

	UFUNCTION()
	virtual void AppearanceTimelineCallback(float Value);

	UFUNCTION()
	virtual void AppearanceTimelineFinishedCallback();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(BlueprintAssignable)
	FClientControllerPossesSignature OnClientControlPossess;

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetMaxWalkSpeed(const float InMaxWalkSpeed);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Net_SetMaxWalkSpeed(const float InMaxWalkSpeed);

	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation)
	void Server_SetPushForce(const float InPushForce);

	UFUNCTION(BlueprintCallable, Client, Unreliable)
	void Net_SetPushForce(const float InPushForce);

	UFUNCTION(BlueprintCallable)
	float GetMaxWalkSpeed();

	/*
	* Prevent the character from moving
	* @param bool bFreeze New Freeze state
	* @param bool bOverride Force (Un)Freeze character ignoring the previos freeze state
	*/
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Net_ToggleFreezeCharacter(const bool bFreeze);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_ToggleFreezeCharacter(const bool bFreeze);

	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation)
	void Server_StunCharacter();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_StopStunCharacter();

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_StunCharacter();

	UFUNCTION(Client, Reliable)
	void Net_TryInteract();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TryInteract();

	UFUNCTION(Client, Reliable)
	void Net_ThrowObject();

	UFUNCTION(Client, Unreliable)
	void Net_OnPounced();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnPounced();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DisappearCharacter();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DisappearCharacter();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Destroy();

	UFUNCTION(BlueprintCallable)
	UPLInteractionComponent* GetPLInteractionComponent() const { return PLInteractionComponent; };

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual void Restart() override;
	virtual void PossessedBy(AController* Possessor) override;
};
