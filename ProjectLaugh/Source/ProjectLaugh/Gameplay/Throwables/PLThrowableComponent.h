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
class PROJECTLAUGH_API UPLThrowableComponent : public UProjectileMovementComponent, public IPLInteractionInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPLThrowableComponent();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Project Laugh | Defaults", meta = (BitMask, BitmaskEnum = "EInteractorSupport"))
	uint8 SupportedInteractors;

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnProjectileStopped(const FHitResult& ImpactResult);

	UFUNCTION()
	void DisableInteractionOutline();

	FTimerHandle InteractionLookAtTimeHandle; 
public:	
	virtual void Interact_Implementation(APLPlayerCharacter* Instigator) override;
	virtual uint8 GetSupportedInteractors_Implementation() override;
	virtual void IsLookingAtInteractable_Implementation(const bool bStartFocus) override;	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;		
};
