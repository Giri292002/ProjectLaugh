// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "ProjectLaugh/Gameplay/PLGameplayTagComponent.h"
#include "ProjectLaugh/Core/PLPlayerController.h"
#include "ProjectLaugh/Animation/PLAnimationData.h"
#include "ProjectLaugh/ProjectLaughCharacter.h"
#include "PLPlayerCharacter.generated.h"

class AController;
class APLPlayerController;
class UPLPlayerAttributesData;
class UPLInhalerComponent;
class UPLInteractionComponent;
class UPLThrowComponent;
class UPLSkillCheckComponent;
class UPLStunData;
class UPLGameplayTagComponent;
class UPLNameComponent;
class UPLAnimationData;
class UCharacterUIProfileData;
class APLHidableActor;
class UPLHideComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClientControllerPossesSignature, AController*, NewController);

UCLASS()
class PROJECTLAUGH_API APLPlayerCharacter : public AProjectLaughCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APLPlayerCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL | Data")
	UPLPlayerAttributesData* PLPlayerAttributesData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL | Data | Animation")
	UPLAnimationData* PLAnimationData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL | Data | UI")
	UCharacterUIProfileData* CharacterUIProfileData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL | Animation")
	TSubclassOf<UAnimInstance> DefaultAnimationLayer;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPLInteractionComponent* PLInteractionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPLThrowComponent* PLThrowComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPLGameplayTagComponent* PLGameplayTagComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPLSkillCheckComponent* PLSkillCheckComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPLNameComponent* PLNameComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPLHideComponent* PLHideComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PL | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PL | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrowAction; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL | Input")
	UInputMappingContext* DefaultMappingContext;

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

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_UpdateNameWidget(const FString& Name);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_UpdateNameWidget(const FString& Name);

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
	void Server_StunCharacter(UPLStunData* StunData);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_StopStunCharacter();

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_StunCharacter(UPLStunData* StunData);

	UFUNCTION(Client, Reliable)
	void Net_TryInteract();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TryInteract();

	UFUNCTION(Client, Reliable)
	virtual void Net_ThrowObject();

	UFUNCTION(Client, Unreliable)
	void Net_OnPounced();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnPounced();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DisappearCharacter();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DisappearCharacter();

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void Server_Destroy();

	//Play a montage animation, optionally jump to a montage section
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PlayAnimation(UAnimMontage* MontageToPlay, bool bJumpToSection = false, FName SectionName = FName("None"));

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAnimation(UAnimMontage* MontageToPlay, bool bJumpToSection = false, FName SectionName = FName("None"));
	
	//Stop any montage that is currently playing, if MontageToStop is provided it will stop that particular montage
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopAnimation(UAnimMontage* MontageToStop = nullptr); 

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StopAnimation(UAnimMontage* MontageToStop = nullptr);

	UFUNCTION(Client,Reliable)
	void Net_SetMovementMode(EMovementMode InMovementMode);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetMovementMode(EMovementMode InMovementMode);

	UFUNCTION(Client, Reliable)
	void Net_Crouch(const bool bStartCrouch);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Crouch(const bool bStartCrouch = true);

	UFUNCTION(BlueprintCallable)
	UPLInteractionComponent* GetInteractionComponent() const { return PLInteractionComponent; };

	UFUNCTION(BlueprintCallable)
	UPLGameplayTagComponent* GetGameplayTagComponent() const { return PLGameplayTagComponent; };

	UFUNCTION(BlueprintCallable)
	UPLSkillCheckComponent* GetSkillCheckComponent() const { return PLSkillCheckComponent; }

	UFUNCTION(BlueprintCallable)
	UPLThrowComponent* GetThrowComponent() const { return PLThrowComponent; }

	UFUNCTION(BlueprintCallable)
	UPLAnimationData* GetAnimationData() const { return PLAnimationData; }

	UFUNCTION(BlueprintCallable)
	UPLPlayerAttributesData* GetPLPlayerAttributesData() const { return PLPlayerAttributesData; }

	UFUNCTION(BlueprintCallable)
	UCharacterUIProfileData* GetCharacterUIData() const { return CharacterUIProfileData; }

	UFUNCTION(BlueprintCallable)
	APLPlayerController* GetPLPlayerController() const { return PLPlayerController; }

	UFUNCTION(BlueprintCallable)
	UPLNameComponent* GetPLNameComponent() const { return PLNameComponent; }

	UFUNCTION(BlueprintCallable)
	UPLHideComponent* GetPLHideComponent() const { return PLHideComponent; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual void Restart() override;
	virtual void PossessedBy(AController* Possessor) override;
	virtual void OnRep_PlayerState() override;
	virtual void OnPlayerStateChanged(APlayerState* NewPlayerState, APlayerState* OldPlayerState) override;
	virtual void NotifyControllerChanged() override;
};
