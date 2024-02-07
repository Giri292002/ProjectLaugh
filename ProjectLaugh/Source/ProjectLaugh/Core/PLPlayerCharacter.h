// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/ProjectLaughCharacter.h"
#include "PLPlayerCharacter.generated.h"

class UPLPlayerAttributesData;

UCLASS()
class PROJECTLAUGH_API APLPlayerCharacter : public AProjectLaughCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APLPlayerCharacter();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Project Laugh | Data")
	UPLPlayerAttributesData* PLPlayerAttributesData;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetMaxWalkSpeed(const float InMaxWalkSpeed);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void SetMaxWalkSpeed(const float InMaxWalkSpeed);

	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation)
	void Server_SetPushForce(const float InPushForce);

	UFUNCTION(BlueprintCallable, Client, Unreliable)
	void SetPushForce(const float InPushForce);
	
		// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

};
