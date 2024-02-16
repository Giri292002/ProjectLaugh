// Fill out your copyright notice in the Description page of Project Settings.


#include "PLInteractionComponent.h"

#include "GameFramework/Character.h"
#include "Math/Rotator.h"
#include "ProjectLaugh/Widgets/PLCrosshairWidget.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"

// Sets default values for this component's properties
UPLInteractionComponent::UPLInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	TraceRange = 500.f;
	bCanInteract = true;

	// ...
}


APlayerController* UPLInteractionComponent::GetPlayerController()
{
	if (IsValid(PlayerController))
	{
		return PlayerController;
	}

	return Cast<APlayerController>((Cast<ACharacter>(GetOwner())->GetController()));
}

void UPLInteractionComponent::OnClientControlPossessed(AController* NewController)
{
	PlayerController = GetPlayerController();
}

// Called when the game starts
void UPLInteractionComponent::BeginPlay()
{
	Super::BeginPlay();	

	APLPlayerCharacter* PLPlayerCharacter = Cast<APLPlayerCharacter>(GetOwner());
	PLPlayerCharacter->OnClientControlPossess.AddDynamic(this, &UPLInteractionComponent::OnClientControlPossessed);
	if ((PLPlayerCharacter->GetLocalRole() == ROLE_Authority || PLPlayerCharacter->GetLocalRole() == ROLE_AutonomousProxy)
		&& ensureAlwaysMsgf(PLCrosshairWidgetClass, TEXT("PLCrosshairWidgetClass is invalid")))
	{
		CrosshairWidget = CreateWidget<UPLCrosshairWidget>(GetWorld(), PLCrosshairWidgetClass);
		CrosshairWidget->SetPLInteractionComponent(this);
		CrosshairWidget->AddToViewport();
	}
}

// Called every frame
void UPLInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetCanInteract() && IsValid(GetPlayerController()))
	{		
		GetPlayerController()->GetPlayerViewPoint(StartLocation, StartRotation);
		EndLocation = StartLocation + (StartRotation.Vector() * TraceRange);
		QueryParams.AddIgnoredActor(GetOwner());

		GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel10, QueryParams);
		//DrawDebugLine(GetWorld(),
		//	HitResult.TraceStart,
		//	HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd,
		//	HitResult.bBlockingHit ? FColor::Green : FColor::Red,
		//	true);

		if (!HitResult.bBlockingHit || !IsValid(HitResult.GetActor()))
		{
			OnCanInteract.Broadcast(false);
			return;
		}

		GEngine->AddOnScreenDebugMessage(11, 0.5f, FColor::Purple, FString::Printf(TEXT("Hit Actor: %s"), *GetNameSafe(HitResult.GetActor())));
		if (auto FoundComponent = HitResult.GetActor()->FindComponentByInterface(UPLInteractionInterface::StaticClass()))
		{
			if (IPLInteractionInterface::Execute_IsValidInteraction(FoundComponent, InteractorType))
			{
				OnCanInteract.Broadcast(true);
				return;
			}
		}
		OnCanInteract.Broadcast(false);
	}
}

