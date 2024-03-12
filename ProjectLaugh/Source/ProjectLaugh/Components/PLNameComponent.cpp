// Fill out your copyright notice in the Description page of Project Settings.


#include "PLNameComponent.h"

#include "ProjectLaugh/Core/PLPlayerState.h"
#include "Camera/CameraComponent.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"
#include "ProjectLaugh/Core/Infection/PLGameState_Infection.h"
#include "ProjectLaugh/Widgets/Components/PLNameWidget.h"
#include <Kismet/KismetSystemLibrary.h>

// Sets default values for this component's properties
UPLNameComponent::UPLNameComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPLNameComponent::BeginPlay()
{
	Super::BeginPlay();

	OutOfFocus();
}

// Called every frame
void UPLNameComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetPLOwnerCharacter()->IsLocallyControlled())
	{
		RunTrace();
	}
}

void UPLNameComponent::SetupName(const FString& Name)
{
	UPLNameWidget* NameWidget = Cast<UPLNameWidget>(GetWidget());
	if (!IsValid(NameWidget))
	{
		return;
	}
	NameWidget->SetName(Name);
}

void UPLNameComponent::InFocus()
{
	UPLNameWidget* NameWidget = Cast<UPLNameWidget>(GetWidget());
	if (!IsValid(NameWidget))
	{
		return;
	}
	NameWidget->FadeIn();
	if (GetWorld()->GetTimerManager().IsTimerActive(FocusTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(FocusTimerHandle);
	}
	GetWorld()->GetTimerManager().SetTimer(FocusTimerHandle, this, &UPLNameComponent::OutOfFocus, 3.f, false);
}

void UPLNameComponent::OutOfFocus()
{
	UPLNameWidget* NameWidget = Cast<UPLNameWidget>(GetWidget());
	if (!IsValid(NameWidget))
	{
		return;
	}
	NameWidget->FadeOut();
}

bool UPLNameComponent::HasLineOfSightTo(const FVector& StartLocation,const FVector& EndLocation, TArray<AActor*> ActorsToIgnore)
{
	FHitResult OutHit;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLocation, EndLocation, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera), false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, OutHit, true);
	return OutHit.bBlockingHit;
}


void UPLNameComponent::RunTrace()
{
	if (!IsValid(GetPLOwnerCharacter()->GetPLPlayerController()))
	{
		return;
	}

	if (!IsValid(GetInfectionGameState()))
	{
		return; 
	}
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetPLOwnerCharacter());

	for (APLPlayerCharacter* PLCharacter : GetInfectionGameState()->GetCharactersInGame())
	{
		//No need to trace against self
		if (PLCharacter == GetPLOwnerCharacter())
		{
			continue;
		}
		UPLNameComponent* OtherNameComp = PLCharacter->GetPLNameComponent();
		if (IsValid(OtherNameComp))
		{
			ActorsToIgnore.Add(PLCharacter);
			if (IsValid(OtherNameComp) && HasLineOfSightTo(GetPLOwnerCharacter()->GetFollowCamera()->GetComponentLocation(), PLCharacter->GetActorLocation(), ActorsToIgnore))
			{
				OtherNameComp->InFocus();
			}
		}
	}
}

APLPlayerCharacter* UPLNameComponent::GetPLOwnerCharacter()
{
	if (IsValid(PLOwnerCharacter))
	{
		return PLOwnerCharacter;
	}
	PLOwnerCharacter = Cast<APLPlayerCharacter>(GetOwner());
	return PLOwnerCharacter;
}

APLGameState_Infection* UPLNameComponent::GetInfectionGameState()
{
	if (IsValid(InfectionGameState))
	{
		return InfectionGameState;
	}
	InfectionGameState = GetWorld()->GetGameState<APLGameState_Infection>();
	return InfectionGameState;
}

