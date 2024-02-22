// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/ProjectLaughCharacter.h"
#include "PLPlayerCharacter.generated.h"

class UPLPlayerAttributesData;
class UPLInhalerComponent;
class UPLInteractionComponent;
class UPLThrowComponent;
class APLPlayerController;
class AController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClientControllerPossesSignature, AController*, NewController);

UCLASS()
class PROJECTLAUGH_API APLPlayerCharacter : public AProjectLaughCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APLPlayerCharacter();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Project Laugh | Data")
	UPLPlayerAttributesData* PLPlayerAttributesData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPLInhalerComponent* PLInhalerComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPLInteractionComponent* PLInteractionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPLThrowComponent* PLThrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InhaleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrowAction;

	UPROPERTY(BlueprintReadOnly, Replicated)
	APLPlayerController* PLPlayerController; 

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void Inhale(const FInputActionValue& Value);

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

	UFUNCTION(BlueprintCallable, Client, Unreliable)
	void Net_ToggleFreezeCharacter(const bool bFreeze);

	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation)
	void Server_ToggleFreezeCharacter(const bool bFreeze);

	UFUNCTION(Client, Reliable)
	void Net_TryInteract();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TryInteract();

	UFUNCTION(Client, Reliable)
	void Net_ThrowObject();

	UFUNCTION(BlueprintCallable)
	UPLInhalerComponent* GetInhalerComponent() const { return PLInhalerComponent; }

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
