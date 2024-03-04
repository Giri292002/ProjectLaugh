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
	virtual void Net_HoldObject(AActor* ObjectToHold);

	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation)
	void Server_HoldObject(AActor* ObjectToHold);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_HoldObject(AActor* ObjectToHold);

	UFUNCTION(BlueprintCallable, Client, Unreliable)
	virtual void Net_Throw(APLPlayerController* PLPlayerController);

	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation)
	void Server_ThrowObject(AActor* ObjectToThrow, FVector LaunchVelocity);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Throw(AActor* HoldingObject, FVector LaunchVelocity);

	UFUNCTION(BlueprintCallable)
	float GetThrowRange() const { return ThrowRange; }

	//Tries to drop if you are holding an object
	UFUNCTION(Client, Reliable)
	void Net_TryDrop();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Drop(AActor* ObjectToDrop);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Drop(AActor* ObjectToDrop);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated)
	AActor* CurrentlyHoldingObject;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL | Throw")
	float ThrowRange;	


};
