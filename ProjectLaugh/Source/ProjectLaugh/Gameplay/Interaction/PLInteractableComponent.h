// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PLInteractionInterface.h"
#include "PLInteractableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTLAUGH_API UPLInteractableComponent : public UActorComponent, public IPLInteractionInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPLInteractableComponent();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Project Laugh | Defaults", meta = (BitMask, BitmaskEnum = "EInteractorSupport"))
	uint8 SupportedInteractors;

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintNativeEvent)
	bool IsValidInteraction(uint8 InteractorType);
	virtual bool IsValidInteraction_Implementation(uint8 InteractorType) override;

	UFUNCTION(BlueprintNativeEvent)
	uint8 GetSupportedInteractors();
	virtual uint8 GetSupportedInteractors_Implementation() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
