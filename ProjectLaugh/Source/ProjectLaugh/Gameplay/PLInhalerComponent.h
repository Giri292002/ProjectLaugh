// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PLInhalerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTLAUGH_API UPLInhalerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPLInhalerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	float CurrentAirInLungAmount;

	UPROPERTY(Replicated)
	float MaxAirInLungAmount;

	UPROPERTY(Replicated)
	float CurrentInhalerAmount;

	UPROPERTY(Replicated)
	float MaxInhalerAmount;

	UPROPERTY(Replicated)
	bool bIsInhaling;

	UPROPERTY()
	FTimerHandle InhaleTimerHandle;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Reduce value from inhaler and increase AirInLungAmount it to lungs
	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation)
	void Server_SetInhale(const bool bInInhale);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Net_StartInhale();

	UFUNCTION(BlueprintCallable, Client, Unreliable)
	void Net_StopInhale();
};
