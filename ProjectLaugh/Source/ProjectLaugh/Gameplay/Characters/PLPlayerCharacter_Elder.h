// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"
#include "PLPlayerCharacter_Elder.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLAUGH_API APLPlayerCharacter_Elder : public APLPlayerCharacter
{
	GENERATED_BODY()
	
protected:
	APLPlayerCharacter_Elder(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPLInhalerComponent* PLInhalerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InhaleAction;

	UFUNCTION()
	void Inhale(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	UPLInhalerComponent* GetInhalerComponent() const { return PLInhalerComponent; }

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Server_Destroy_Implementation() override;
};
