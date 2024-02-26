// Fill out your copyright notice in the Description page of Project Settings.


#include "PLGameplayTagComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPLGameplayTagComponent::UPLGameplayTagComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void UPLGameplayTagComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPLGameplayTagComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(UPLGameplayTagComponent, ActiveGameplayTags);
}

void UPLGameplayTagComponent::Server_AddTag_Implementation(const FGameplayTag TagToAdd)
{
	ActiveGameplayTags.AddTag(TagToAdd);
}

bool UPLGameplayTagComponent::Server_AddTag_Validate(const FGameplayTag TagToAdd)
{
	return true;
}

void UPLGameplayTagComponent::Server_RemoveTag_Implementation(const FGameplayTag TagToRemove)
{
	ActiveGameplayTags.RemoveTag(TagToRemove);
}

bool UPLGameplayTagComponent::Server_RemoveTag_Validate(const FGameplayTag TagToRemove)
{
	return true;
}

void UPLGameplayTagComponent::Server_HasOrAddTag_Implementation(const FGameplayTag TagToAdd)
{
	if (!ActiveGameplayTags.HasTag(TagToAdd))
	{
		Server_AddTag(TagToAdd);
	}
}

bool UPLGameplayTagComponent::Server_HasOrAddTag_Validate(const FGameplayTag TagToAdd)
{
	return true;
}

