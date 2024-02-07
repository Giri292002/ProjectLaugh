// Fill out your copyright notice in the Description page of Project Settings.


#include "PLPlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectLaugh/Data/PLPlayerAttributesData.h"

// Sets default values
APLPlayerCharacter::APLPlayerCharacter()
{
}

// Called when the game starts or when spawned
void APLPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!ensureAlwaysMsgf(PLPlayerAttributesData, TEXT("PLPlayerAttributesData is null")))
	{
		return;
	}

	SetMaxWalkSpeed(PLPlayerAttributesData->MaxWalkSpeed);
	SetPushForce(PLPlayerAttributesData->PushForce);
}

void APLPlayerCharacter::Server_SetMaxWalkSpeed_Implementation(const float InMaxWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = InMaxWalkSpeed;
}

bool APLPlayerCharacter::Server_SetMaxWalkSpeed_Validate(const float InMaxWalkSpeed)
{
	return true;
}

void APLPlayerCharacter::SetMaxWalkSpeed_Implementation(const float InMaxWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = InMaxWalkSpeed;
	if (!HasAuthority())
	{
		Server_SetMaxWalkSpeed(InMaxWalkSpeed);
	}
}

void APLPlayerCharacter::Server_SetPushForce_Implementation(const float InPushForce)
{
	GetCharacterMovement()->InitialPushForceFactor = InPushForce;
}

bool APLPlayerCharacter::Server_SetPushForce_Validate(const float InPushForce)
{
	return true;
}

void APLPlayerCharacter::SetPushForce_Implementation(const float InPushForce)
{
	GetCharacterMovement()->InitialPushForceFactor = InPushForce;
	if (!HasAuthority())
	{
		Server_SetPushForce(InPushForce);
	}
}

// Called every frame
void APLPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APLPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APLPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetCharacterMovement()->bPushForceScaledToMass = true;
}

