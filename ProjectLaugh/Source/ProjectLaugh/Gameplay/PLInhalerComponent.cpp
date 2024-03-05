// Fill out your copyright notice in the Description page of Project Settings.


#include "PLInhalerComponent.h"

#include "Engine/StaticMeshActor.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"
#include "ProjectLaugh/Data/PLInhalerData.h"
#include "ProjectLaugh/Gameplay/PLGameplayTagComponent.h"
#include "ProjectLaugh/Widgets/PLInhalerWidget.h"

// Sets default values for this component's properties
UPLInhalerComponent::UPLInhalerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	bStopRunningDone = true;
}


void UPLInhalerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPLInhalerComponent, CurrentAirInLungAmount);
	DOREPLIFETIME(UPLInhalerComponent, MaxAirInLungAmount);
	DOREPLIFETIME(UPLInhalerComponent, CurrentInhalerAmount);
	DOREPLIFETIME(UPLInhalerComponent, MaxInhalerAmount);
	DOREPLIFETIME(UPLInhalerComponent, bIsInhaling);
	DOREPLIFETIME(UPLInhalerComponent, bStopRunningDone);
	DOREPLIFETIME(UPLInhalerComponent, InhalerMesh);
}

// Called when the game starts
void UPLInhalerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ensureAlwaysMsgf(PLInhalerData, TEXT("PL Inhaler Data is invalid")))
	{
		MaxAirInLungAmount = PLInhalerData->MaxLung;
		MaxInhalerAmount = CurrentInhalerAmount = PLInhalerData->MaxInhaler;
	}

	PLPlayerCharacter = Cast<APLPlayerCharacter>(GetOwner());
	
	//Attach Mesh to player
	if (PLPlayerCharacter->HasAuthority())
	{
		Server_SpawnInhalerMesh();
		Multicast_ToggleMeshVisibility(false);
	}
}

// Called every frame
void UPLInhalerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsInhaling)
	{
		if (CurrentInhalerAmount > 0.f && CurrentAirInLungAmount <= MaxAirInLungAmount)
		{
			FMath::Clamp(CurrentInhalerAmount -= DeltaTime * PLInhalerData->InhalerUseRate, 0, MaxInhalerAmount);
			FMath::Clamp(CurrentAirInLungAmount += (DeltaTime * PLInhalerData->LungChargeRate), 0, MaxAirInLungAmount);
		}
		else
		{
			Net_StopInhale();
		}
		OnInhalerValueChange.Broadcast(CurrentInhalerAmount, MaxInhalerAmount);
		OnLungValueChange.Broadcast(CurrentAirInLungAmount, MaxAirInLungAmount);
	}
	else
	{
		// Passively Recharge
		if (CurrentInhalerAmount <= MaxInhalerAmount)
		{
			FMath::Clamp(CurrentInhalerAmount += DeltaTime * PLInhalerData->InhalerChargeRate, 0.f, MaxInhalerAmount);
		}
		if (CurrentAirInLungAmount > 0.f)
		{
			FMath::Clamp(CurrentAirInLungAmount -= DeltaTime * PLInhalerData->LungChargeRate, 0.f, MaxInhalerAmount);

			if (CurrentAirInLungAmount <= 0.f)
			{
				Net_StopRunning();
			}
		}
		OnInhalerValueChange.Broadcast(CurrentInhalerAmount, MaxInhalerAmount);
		OnLungValueChange.Broadcast(CurrentAirInLungAmount, MaxAirInLungAmount);
	}
}

void UPLInhalerComponent::Net_StartRunning_Implementation()
{	
	//UE_LOG(LogTemp, Warning, TEXT("Start running. Changing to: %f"), PLPlayerCharacter->GetMaxWalkSpeed() * PLInhalerData->MaxWalkSpeedMultiplier);
	if (bStopRunningDone)
	{
		Net_SetbStopRunningDone(false);
		PLPlayerCharacter->Net_SetMaxWalkSpeed(PLPlayerCharacter->GetMaxWalkSpeed() * PLInhalerData->MaxWalkSpeedMultiplier);
	}
}

void UPLInhalerComponent::Net_StopRunning_Implementation()
{
	if (!bStopRunningDone)
	{
		Net_SetbStopRunningDone(true);
		PLPlayerCharacter->Net_SetMaxWalkSpeed(PLPlayerCharacter->GetMaxWalkSpeed() / PLInhalerData->MaxWalkSpeedMultiplier);
	}
}

void UPLInhalerComponent::Net_StartInhale_Implementation()
{
	bIsInhaling = true;
	PLPlayerCharacter->Net_ToggleFreezeCharacter(true);
	PLPlayerCharacter->GetGameplayTagComponent()->Server_AddTag(SharedGameplayTags::TAG_Ability_Inhale_Inhaling);
	Server_ToggleMeshVisibility(true); 
	if (!GetOwner()->HasAuthority())
	{
		Server_SetInhale(true);
	}
}

void UPLInhalerComponent::Net_StopInhale_Implementation()
{
	bIsInhaling = false;
	PLPlayerCharacter->Net_ToggleFreezeCharacter(false);
	PLPlayerCharacter->GetGameplayTagComponent()->Server_RemoveTag(SharedGameplayTags::TAG_Ability_Inhale_Inhaling);
	Server_ToggleMeshVisibility(false);
	if (CurrentAirInLungAmount > 0.f)
	{
		Net_StartRunning();
	}

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

void UPLInhalerComponent::Net_SetbStopRunningDone_Implementation(const bool bSInStopRunning)
{
	bStopRunningDone = bSInStopRunning;
	if (!GetOwner()->HasAuthority())
	{
		Server_SetbStopRunningDone(bSInStopRunning);
	}
}

void UPLInhalerComponent::Server_SetbStopRunningDone_Implementation(const bool bSInStopRunning)
{
	bStopRunningDone = bSInStopRunning;
}

bool UPLInhalerComponent::Server_SetbStopRunningDone_Validate(const bool bSInStopRunning)
{
	return true;
}

void UPLInhalerComponent::Server_SpawnInhalerMesh_Implementation()
{
	InhalerMesh = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
	InhalerMesh->SetReplicates(true);
	InhalerMesh->SetActorEnableCollision(false);
	UStaticMeshComponent* MeshComponent = InhalerMesh->GetStaticMeshComponent();
	if (IsValid(MeshComponent))
	{
		InhalerMesh->SetMobility(EComponentMobility::Movable);
		MeshComponent->SetStaticMesh(PLInhalerData->InhalerMesh);
	}
}

bool UPLInhalerComponent::Server_SpawnInhalerMesh_Validate()
{
	return true;
}

void UPLInhalerComponent::Server_ToggleMeshVisibility_Implementation(const bool bVisible)
{
	Multicast_ToggleMeshVisibility(bVisible);
}

bool UPLInhalerComponent::Server_ToggleMeshVisibility_Validate(const bool bVisible)
{
	return true;
}

void UPLInhalerComponent::Multicast_ToggleMeshVisibility_Implementation(const bool bVisible)
{
	AttachInhalerToCharacter(InhalerMesh);
	if (IsValid(InhalerMesh))
	{
		InhalerMesh->GetStaticMeshComponent()->SetVisibility(bVisible);
	}
}

void UPLInhalerComponent::AttachInhalerToCharacter(AStaticMeshActor* InInhalerMesh)
{
	if (!IsValid(PLPlayerCharacter))
	{
		return;
	}
	InhalerMesh->SetMobility(EComponentMobility::Movable);
	InInhalerMesh->AttachToComponent(PLPlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("hand_r"));
}

void UPLInhalerComponent::OnRep_InhalerMesh()
{
	InhalerMesh->SetActorEnableCollision(false);

	InhalerMesh->GetStaticMeshComponent()->SetVisibility(false);

	UStaticMeshComponent* MeshComponent = InhalerMesh->GetStaticMeshComponent();
	if (IsValid(MeshComponent))
	{
		InhalerMesh->SetMobility(EComponentMobility::Movable);
		MeshComponent->SetStaticMesh(PLInhalerData->InhalerMesh);
	}
}
