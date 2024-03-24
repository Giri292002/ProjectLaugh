// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Components/PLActorComponent.h"
#include "ProjectLaugh/SharedGameplayTags.h"
#include "PLInteractionInterface.h"
#include "PLInteractionComponent.generated.h"

USTRUCT(BlueprintType)
struct FInteractionInformation
{
	GENERATED_BODY()

	// {Key} to {Interaction Prompt}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText InteractionPrompt = FText::FromString(TEXT("INTERACT"));

	//This is used to set color, 0 is default, 1 is yellow etc.
	//TODO: Need to implement color change based on priority
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int InteractionPriority = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCanInteractSignature, const bool, bCanInteract, FInteractionInformation, InteractionInformation);

class UPLInteractableComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTLAUGH_API UPLInteractionComponent : public UPLActorComponent, public IPLInteractionInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPLInteractionComponent();

public:
	UPROPERTY(BlueprintAssignable)
	FCanInteractSignature OnCanInteract;

	//Get if the player can run the interaction trace
	UFUNCTION(BlueprintCallable)
	bool GetCanRunInteract();

	//Try calling interact interface on a component
	UFUNCTION(BlueprintCallable)
	bool TryInteract();

	UFUNCTION(BlueprintCallable)
	bool RunInteractTrace(APLPlayerController* PLPlayerController);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Project Laugh | Defaults", meta = (BitMask, BitmaskEnum = "EInteractorSupport"))
	FGameplayTagContainer InteractorType;

	UPROPERTY(EditDefaultsOnly, Category = "Project Laugh | Defaults")
	float TraceRange;	

	UPROPERTY()
	bool bCanRunInteract;

	UFUNCTION()
	void AssignInteractableComponent(UPLInteractableComponent* InteractableComponent);

	UFUNCTION()
	void UnassignInteractableComponent();

	UFUNCTION()
	bool IsValidInteractionWith(UPLInteractableComponent* InteractableComponent);

	// Called when the game starts
	virtual void BeginPlay() override;	

private:
	UPROPERTY()
	FHitResult HitResult;
	FVector StartLocation;
	FRotator StartRotation;
	FVector EndLocation;
	FCollisionQueryParams QueryParams;
	UPLInteractableComponent* LastInteractedComponent;

	UPROPERTY()
	APLPlayerCharacter* PLPlayerCharacter;
};
