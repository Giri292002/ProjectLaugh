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

	//Set if the player can interact
	UFUNCTION(BlueprintCallable)
	void SetCanInteract(bool bInCanInteract) { bCanInteract = bInCanInteract; }

	//Get if the player can interact
	UFUNCTION(BlueprintCallable)
	bool GetCanInteract() const { return bCanInteract; }

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Project Laugh | Defaults", meta = (BitMask, BitmaskEnum = "EInteractorSupport"))
	uint8 InteractorType;

	UPROPERTY(EditDefaultsOnly, Category = "Project Laugh | Defaults")
	float TraceRange;

	UPROPERTY(EditDefaultsOnly, Category = "Project Laugh | Defaults")
	TSubclassOf<UPLCrosshairWidget> PLCrosshairWidgetClass;

	UPROPERTY()
	UPLCrosshairWidget* CrosshairWidget;
	
	UPROPERTY()
	bool bCanInteract;

	UPROPERTY()
	APlayerController* PlayerController;

	UFUNCTION()
	APlayerController* GetPlayerController();

	UFUNCTION()
	void OnClientControlPossessed(AController* NewController);

	// Called when the game starts
	virtual void BeginPlay() override;	

private:
	FHitResult HitResult;
	FVector StartLocation;
	FRotator StartRotation;
	FVector EndLocation;
	FCollisionQueryParams QueryParams;
};
