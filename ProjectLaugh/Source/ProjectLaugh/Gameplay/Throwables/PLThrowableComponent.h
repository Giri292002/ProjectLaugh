// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../../Gameplay/Interaction/PLInteractionInterface.h"
#include "PLThrowableComponent.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

//Attach this to any object that needs to be thrown
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTLAUGH_API UPLThrowableComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPLThrowableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnProjectileStopped(const FHitResult& ImpactResult);
};
