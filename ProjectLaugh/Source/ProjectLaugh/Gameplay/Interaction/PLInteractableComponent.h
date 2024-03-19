// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectLaugh/SharedGameplayTags.h"
#include "PLInteractionInterface.h"
#include "PLInteractableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTLAUGH_API UPLInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPLInteractableComponent();

	UFUNCTION(BlueprintCallable, Category = "PL | Interaction")
	FGameplayTagContainer GetSupportedInteractors() const { return SupportedInteractors; }

	UFUNCTION(BlueprintCallable, Category = "PL | Interaction")
	void Interact(APLPlayerCharacter* Instigator, UPLInteractionComponent* InteractableComponent);

	//Extra interaction checks that are added to the attached actor
	UFUNCTION(BlueprintCallable, Category = "PL | Interaction")
	bool CanInteract(APLPlayerCharacter* InInstigator, UPLInteractionComponent* OtherInteractableComponent);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "PL | Interaction", meta = (BitMask, BitmaskEnum = "EInteractorSupport"))
	FGameplayTagContainer SupportedInteractors;
};
