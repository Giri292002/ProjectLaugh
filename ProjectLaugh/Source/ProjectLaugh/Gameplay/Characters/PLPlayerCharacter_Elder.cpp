// Fill out your copyright notice in the Description page of Project Settings.


#include "PLPlayerCharacter_Elder.h"
#include "EnhancedInputComponent.h"
#include "ProjectLaugh/Gameplay/PLInhalerComponent.h"

APLPlayerCharacter_Elder::APLPlayerCharacter_Elder(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PLInhalerComponent = CreateDefaultSubobject<UPLInhalerComponent>(FName(TEXT("Inhaler Component")));
}

void APLPlayerCharacter_Elder::Inhale(const FInputActionValue& Value)
{
	if (ensure(PLInhalerComponent))
	{
		const bool bIsInhaling = Value.Get<bool>();
		if (bIsInhaling)
		{
			PLInhalerComponent->Net_StartInhale();
		}
		else
		{
			PLInhalerComponent->Net_StopInhale();
		}
	}
}

void APLPlayerCharacter_Elder::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(InhaleAction, ETriggerEvent::Triggered, this, &APLPlayerCharacter_Elder::Inhale);
	}
}