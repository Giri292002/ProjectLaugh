// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Components/PLActorComponent.h"
#include "PLInhalerComponent.generated.h"

class UPLInhalerData;
class UPLInhalerWidget;
class APawn;
class AController;
class APLPlayerCharacter;
class AStaticMeshActor;
class UStaticMesh;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInhalerValueChange, const float, CurrentInhalerAmount, const float, MaxInhalerAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLungValueChange, const float, CurrentLungAmount, const float, MaxLungAmount);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTLAUGH_API UPLInhalerComponent : public UPLActorComponent
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
	int MaxAirInLungAmount;

	UPROPERTY(Replicated)
	float CurrentInhalerAmount;

	UPROPERTY(Replicated)
	int MaxInhalerAmount;

	UPROPERTY(Replicated)
	bool bIsInhaling;

	UPROPERTY()
	FTimerHandle InhaleTimerHandle;

	UPROPERTY(EditDefaultsOnly)
	UPLInhalerData* PLInhalerData;

public:	
	UPROPERTY(BlueprintAssignable)
	FOnInhalerValueChange OnInhalerValueChange;

	UPROPERTY(BlueprintAssignable)
	FOnLungValueChange OnLungValueChange;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Reduce value from inhaler and increase AirInLungAmount it to lungs
	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation)
	void Server_SetInhale(const bool bInInhale);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Net_StartInhale();

	UFUNCTION(BlueprintCallable, Client, Unreliable)
	void Net_StopInhale();

	UFUNCTION(BlueprintCallable, Client, Unreliable)
	void Net_StartRunning();

	UFUNCTION(BlueprintCallable, Client, Unreliable)
	void Net_StopRunning();

	UFUNCTION(BlueprintCallable, Client, Unreliable)
	void Net_SetbStopRunningDone(const bool bSInStopRunning);

	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation)
	void Server_SetbStopRunningDone(const bool bSInStopRunning);

	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation)
	void Server_ToggleMeshVisibility(const bool bVisible);

	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation)
	void Server_SpawnInhalerMesh();

	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable)
	void Multicast_ToggleMeshVisibility(const bool bVisible);

	UFUNCTION()
	void AttachInhalerToCharacter(AStaticMeshActor* InInhalerMesh);

	UFUNCTION()
	void OnRep_InhalerMesh();

	//UFUNCTION(BlueprintCallable, Client, Unreliable)
	//void Net_FreezeCharacter();

private:
	UPROPERTY()
	float PreviousWalkSpeed = 0.f;

	UPROPERTY()
	float PreviousPushSpeed = 100.f;

	UPROPERTY()
	APLPlayerCharacter* PLPlayerCharacter;

	UPROPERTY(Replicated)
	bool bStopRunningDone;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_InhalerMesh)
	AStaticMeshActor* InhalerMesh;
};
