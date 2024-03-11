// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "PLNameComponent.generated.h"

class UWidgetComponent;

/*
* Component that handles showing names of everyone in view
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTLAUGH_API UPLNameComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPLNameComponent();

protected:
	UPROPERTY()
	APLPlayerCharacter* PLOwnerCharacter;

	UPROPERTY()
	FTimerHandle FocusTimerHandle;

	UPROPERTY()
	APLGameState_Infection* InfectionGameState;

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void RunTrace();

	UFUNCTION()
	APLPlayerCharacter* GetPLOwnerCharacter();

	UFUNCTION()
	APLGameState_Infection* GetInfectionGameState();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupName(const FString& Name);

	UFUNCTION()
	void InFocus();

	UFUNCTION()
	void OutOfFocus();

	UFUNCTION()
	bool HasLineOfSightTo(const FVector& StartLocation, const FVector& EndLocation, TArray<AActor*> ActorsToIgnore);
};
