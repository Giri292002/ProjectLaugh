// Fill out your copyright notice in the Description page of Project Settings.


#include "PLInhalerComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPLInhalerComponent::UPLInhalerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}


void UPLInhalerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(UPLInhalerComponent, CurrentAirInLungAmount);
	DOREPLIFETIME(UPLInhalerComponent, MaxAirInLungAmount);
	DOREPLIFETIME(UPLInhalerComponent, CurrentInhalerAmount);
	DOREPLIFETIME(UPLInhalerComponent, MaxInhalerAmount);
	DOREPLIFETIME(UPLInhalerComponent, bIsInhaling);
}

// Called when the game starts
void UPLInhalerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPLInhalerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsInhaling)
	{
		/*CurrentInhalerAmount -= DeltaTime;
		CurrentAirInLungAmount += DeltaTime;*/
		GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Green, TEXT("Is Inhaling"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Green, TEXT("Is NOT Inhaling"));
	}
}



void UPLInhalerComponent::Net_StartInhale_Implementation()
{
	bIsInhaling = true;
	if (!GetOwner()->HasAuthority())
	{
		Server_SetInhale(true);
	}
}

void UPLInhalerComponent::Net_StopInhale_Implementation()
{
	bIsInhaling = false;
	if (!GetOwner()->HasAuthority())
	{
		Server_SetInhale(false);
	}
}

bool UPLInhalerComponent::Server_SetInhale_Validate(const bool InInhale)
{
	return true;
}

void UPLInhalerComponent::Server_SetInhale_Implementation(const bool InInhale)
{
	bIsInhaling = InInhale;
}


