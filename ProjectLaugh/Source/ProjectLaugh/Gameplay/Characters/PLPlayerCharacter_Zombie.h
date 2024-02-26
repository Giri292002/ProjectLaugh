// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"
#include "PLPlayerCharacter_Zombie.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLAUGH_API APLPlayerCharacter_Zombie : public APLPlayerCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PounceAction;

protected:
	UFUNCTION(BlueprintCallable, Client, Unreliable)
	void Net_Pounce();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_Pounce(FRotator NewRotation, FHitResult HitResult);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
