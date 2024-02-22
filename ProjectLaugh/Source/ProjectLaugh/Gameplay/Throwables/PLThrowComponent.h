// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PLThrowComponent.generated.h"


class UPLThrowableComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTLAUGH_API UPLThrowComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPLThrowComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable, Client, Unreliable)
	void Net_HoldObject(AActor* ObjectToHold);

	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation)
	void Server_HoldObject(AActor* ObjectToHold);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_HoldObject(AActor* ObjectToHold);

	UFUNCTION(BlueprintCallable, Client, Unreliable)
	void Net_Throw(APLPlayerController* PLPlayerController);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Throw(AActor* HoldingObject);

	UFUNCTION(BlueprintCallable)
	float GetThrowRange() const { return ThrowRange; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated)
	AActor* CurrentlyHoldingObject;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL | Throw")
	float ThrowRange;	

	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation)
	void Server_ThrowObject(APLPlayerController* PLPlayerController, AActor* ObjectToThrow);

	UFUNCTION()
	void ThrowObject(APLPlayerController* PLPlayerController);
};
