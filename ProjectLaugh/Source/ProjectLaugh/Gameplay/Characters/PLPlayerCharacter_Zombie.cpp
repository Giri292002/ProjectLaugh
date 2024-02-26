// Fill out your copyright notice in the Description page of Project Settings.


#include "PLPlayerCharacter_Zombie.h"

#include "EnhancedInputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectLaugh/Core/PLPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "ProjectLaugh/Gameplay/PLGameplayTagComponent.h"




void APLPlayerCharacter_Zombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(PounceAction, ETriggerEvent::Triggered, this, &APLPlayerCharacter_Zombie::Net_Pounce);
	}
}

void APLPlayerCharacter_Zombie::Net_Pounce_Implementation()
{
	//Check if we can pounce
	//Add more blocked tags here
	FGameplayTagContainer BlockedTags;
	BlockedTags.AddTag(SharedGameplayTags::TAG_Character_Status_Stunned);
	BlockedTags.AddTag(SharedGameplayTags::TAG_Character_Status_Pouncing);

	if (PLGameplayTagComponent->GetActiveGameplayTags().HasAny(BlockedTags))
	{
		return;
	}

	//Perform Pounce Trace
	FHitResult HitResult;
	FVector StartLocation;
	FRotator StartRotation;
	PLPlayerController->GetPlayerViewPoint(StartLocation, StartRotation);
	//Add the camera spring arm length to the pounce range
	const FVector EndLocation = StartLocation + (StartRotation.Vector() * (150.f + GetCameraBoom()->TargetArmLength));
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectsToQuery;
	ObjectsToQuery.Add(EObjectTypeQuery::ObjectTypeQuery3);
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), StartLocation, EndLocation, 30.f, 40.f, ObjectsToQuery, false, IgnoredActors, EDrawDebugTrace::Persistent, HitResult, true);

	const FRotator NewActorRotation = FRotator(GetActorRotation().Pitch, StartRotation.Yaw, GetActorRotation().Roll);
	SetActorRotation(NewActorRotation);
	Server_Pounce(NewActorRotation, HitResult);
}

void APLPlayerCharacter_Zombie::Server_Pounce_Implementation(FRotator NewRotation, FHitResult HitResult)
{
	SetActorRotation(NewRotation);
	if (!HitResult.bBlockingHit)
	{
		return;
	}

	if(APLPlayerCharacter* OtherPlayer = Cast<APLPlayerCharacter>(HitResult.GetActor()))
	{
		if (!(OtherPlayer->FindComponentByClass<UPLGameplayTagComponent>()->GetActiveGameplayTags().HasTag(SharedGameplayTags::TAG_Character_Affiliation_Elder)))
		{
			return;
		}
		OtherPlayer->Net_ToggleFreezeCharacter(true);
		OtherPlayer->Multicast_OnPounced();
	}
	//TODO: Add line back when pouncing animation is setup
	//PLGameplayTagComponent->Server_AddTag(SharedGameplayTags::TAG_Character_Status_Pouncing);
	
}


bool APLPlayerCharacter_Zombie::Server_Pounce_Validate(FRotator NewRotation, FHitResult HitResult)
{
	return true;
}