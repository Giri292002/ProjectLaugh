// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PLInteractionInterface.h"
#include "PLInteractionComponent.generated.h"

class UPLCrosshairWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCanInteractSignature, const bool, bCanInteract);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTLAUGH_API UPLInteractionComponent : public UActorComponent, public IPLInteractionInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPLInteractionComponent();

public:
	UPROPERTY(BlueprintAssignable)
	FCanInteractSignature OnCanInteract;

	UPROPERTY(EditDefaultsOnly, Category = "Project Laugh | Defaults")
	TSubclassOf<UPLCrosshairWidget> PLCrosshairWidgetClass;

	//Get if the player can run the interaction trace
	UFUNCTION(BlueprintCallable)
	bool GetCanRunInteract() const { return bCanRunInteract; }

	//Try calling interact interface on a component
	UFUNCTION(BlueprintCallable)
	bool TryInteract();

	UFUNCTION(BlueprintCallable)
	bool RunInteractTrace(APLPlayerController* PLPlayerController);

	virtual uint8 GetSupportedInteractors_Implementation() override;
	virtual bool GetInteractionHitResult_Implementation(FHitResult& OutHitResult) override;


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Project Laugh | Defaults", meta = (BitMask, BitmaskEnum = "EInteractorSupport"))
	uint8 InteractorType;

	UPROPERTY(EditDefaultsOnly, Category = "Project Laugh | Defaults")
	float TraceRange;

	

	UPROPERTY()
	bool bCanRunInteract;

	// Called when the game starts
	virtual void BeginPlay() override;	

private:
	UPROPERTY()
	FHitResult HitResult;
	FVector StartLocation;
	FRotator StartRotation;
	FVector EndLocation;
	FCollisionQueryParams QueryParams;
	UActorComponent* LastInteractedComponent;

	UPROPERTY()
	APLPlayerCharacter* PLPlayerCharacter;
};
