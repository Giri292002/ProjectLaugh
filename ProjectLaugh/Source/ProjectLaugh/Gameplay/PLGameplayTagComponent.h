// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "../SharedGameplayTags.h"
#include "PLGameplayTagComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTLAUGH_API UPLGameplayTagComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPLGameplayTagComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/*
	* Adds a gameplay tag
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "PL | Tags")
	void Server_AddTag(const FGameplayTag TagToAdd);

	//Removes a tag
	UFUNCTION(Server,Reliable, WithValidation, BlueprintCallable, Category = "PL | Tags")
	void Server_RemoveTag(const FGameplayTag TagToRemove);

	UFUNCTION(Client, Reliable, BlueprintCallable,Category = "PL | Tags")
	void Net_AddTag(const FGameplayTag TagToAdd);

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "PL | Tags")
	void Net_RemoveTag(const FGameplayTag TagToRemove);

	/**
	* Tries to find a tag, if tag doesn't exist add one
	* @return bool Returns if it found a tag or not
	* @param FGameplayTag TagToAdd The tag to add
	* 
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "PL | Tags")
	void Server_HasOrAddTag(const FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer& GetActiveGameplayTags() { return ActiveGameplayTags; }

protected:
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "PL | Tags")
	FGameplayTagContainer ActiveGameplayTags;
};
