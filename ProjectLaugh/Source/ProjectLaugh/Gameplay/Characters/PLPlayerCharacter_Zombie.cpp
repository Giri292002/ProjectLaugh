// Fill out your copyright notice in the Description page of Project Settings.


#include "PLPlayerCharacter_Zombie.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h" 
#include "ProjectLaugh/Core/PLPlayerController.h"
#include "ProjectLaugh/Core/Infection/PLGameMode_Infection.h"
#include "ProjectLaugh/Gameplay/Characters/PLPlayerCharacter_Elder.h"
#include "ProjectLaugh/Data/PLPlayerAttributesData.h"
#include "ProjectLaugh/Gameplay/PLGameplayTagComponent.h"

void APLPlayerCharacter_Zombie::BeginPlay()
{
	GetMesh()->SetScalarParameterValueOnMaterials(FName("Appearance"), 0.f);

	if (PLGameplayTagComponent)
	{
		PLGameplayTagComponent->Server_AddTag(SharedGameplayTags::TAG_Character_Status_Spawning);
	}

	Super::BeginPlay();	

	if (HasAuthority())
	{
		Multicast_SpawnZombie();
	}
	else
	{
		Server_SpawnZombie();
	}
}

void APLPlayerCharacter_Zombie::Server_SpawnZombie_Implementation()
{
	Multicast_SpawnZombie();
}

bool APLPlayerCharacter_Zombie::Server_SpawnZombie_Validate()
{
	return true;
}

void APLPlayerCharacter_Zombie::Multicast_SpawnZombie_Implementation()
{
	PlayAppearanceTimeline(PLPlayerAttributesData->AppearanceTime);
}

void APLPlayerCharacter_Zombie::AppearanceTimelineFinishedCallback()
{
	Super::AppearanceTimelineFinishedCallback();
	GEngine->AddOnScreenDebugMessage((uint64)("Zombie"), 10.0f, FColor::Green, TEXT("FINISHED TIMELINE"));
	if (PLGameplayTagComponent)
	{
		PLGameplayTagComponent->Server_RemoveTag(SharedGameplayTags::TAG_Character_Status_Spawning);
	}
	Net_ToggleFreezeCharacter(false);
}


void APLPlayerCharacter_Zombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(PounceAction, ETriggerEvent::Triggered, this, &APLPlayerCharacter_Zombie::Net_Pounce);
	}
}

void APLPlayerCharacter_Zombie::Restart()
{
	Super::Restart();
	Net_ToggleFreezeCharacter(true);
}

void APLPlayerCharacter_Zombie::Net_Pounce_Implementation()
{
	//Check if we can pounce, Add more blocked tags here
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

	UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), StartLocation, EndLocation, 30.f, 40.f, ObjectsToQuery, false, IgnoredActors, EDrawDebugTrace::None, HitResult, true);

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

	if(APLPlayerCharacter_Elder* OtherPlayer = Cast<APLPlayerCharacter_Elder>(HitResult.GetActor()))
	{
		if (!(OtherPlayer->FindComponentByClass<UPLGameplayTagComponent>()->GetActiveGameplayTags().HasTag(SharedGameplayTags::TAG_Character_Affiliation_Elder)))
		{
			return;
		}

		APLGameMode_Infection* PLGameMode = Cast<APLGameMode_Infection>(UGameplayStatics::GetGameMode(GetWorld()));
		checkf(PLGameMode, TEXT("PLGamemode infection is invalid"));
		PLGameMode->SpawnConvertedZombie(OtherPlayer);
	}
	//TODO: Add line back when pouncing animation is setup
	//PLGameplayTagComponent->Server_AddTag(SharedGameplayTags::TAG_Character_Status_Pouncing);
	
}


bool APLPlayerCharacter_Zombie::Server_Pounce_Validate(FRotator NewRotation, FHitResult HitResult)
{
	return true;
}