// Fill out your copyright notice in the Description page of Project Settings.


#include "PLPlayerCharacter_Zombie.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h" 
#include "ProjectLaugh/Animation/PLAnimationData.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"
#include "ProjectLaugh/Core/PLPlayerState.h"
#include "ProjectLaugh/Core/PLPlayerController.h"
#include "ProjectLaugh/Core/Infection/PLGameMode_Infection.h"
#include "ProjectLaugh/Core/Infection/PLGameState_Infection.h"
#include "ProjectLaugh/Components/PLScoreComponent.h"
#include "ProjectLaugh/Data/PLPlayerAttributesData.h"
#include "ProjectLaugh/Gameplay/Characters/PLPlayerCharacter_Elder.h"
#include "ProjectLaugh/Gameplay/Interaction/PLInteractionInterface.h"
#include "ProjectLaugh/Gameplay/PLGameplayTagComponent.h"
#include "ProjectLaugh/Gameplay/Skillcheck/PLSkillCheckComponent.h"
#include "ProjectLaugh/Gameplay/Throwables/PLThrowableBase.h"
#include "ProjectLaugh/Gameplay/Throwables/PLThrowableArm.h"
#include "ProjectLaugh/Gameplay/Throwables/PLThrowComponent.h"

APLPlayerCharacter_Zombie::APLPlayerCharacter_Zombie(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PounceCooldownTime = 5.f;
}

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
		ArmedMesh = GetMesh()->GetSkeletalMeshAsset();
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
	//GEngine->AddOnScreenDebugMessage((uint64)("Zombie"), 10.0f, FColor::Green, TEXT("FINISHED TIMELINE"));
	if (PLGameplayTagComponent)
	{
		PLGameplayTagComponent->Server_RemoveTag(SharedGameplayTags::TAG_Character_Status_Spawning);
	}
	Net_ToggleFreezeCharacter(false);

	if (GetGameplayTagComponent()->GetActiveGameplayTags().HasTag(SharedGameplayTags::TAG_Character_Affiliation_Zombie_Alpha))
	{
		//Let game state know to start running brain meter
		Server_RunBrainMeter();
	}
}

void APLPlayerCharacter_Zombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(PounceAction, ETriggerEvent::Triggered, this, &APLPlayerCharacter_Zombie::Net_Pounce);
		EnhancedInputComponent->BindAction(DetachArmAction, ETriggerEvent::Triggered, this, &APLPlayerCharacter_Zombie::Net_DetachArm);
	}
}

void APLPlayerCharacter_Zombie::Restart()
{
	Super::Restart();
	//Only freeze character if we are spawning
	if (PLGameplayTagComponent->GetActiveGameplayTags().HasTag(SharedGameplayTags::TAG_Character_Status_Spawning))
	{
		Net_ToggleFreezeCharacter(true);
	}
}

void APLPlayerCharacter_Zombie::Net_ThrowObject()
{
	if (PLGameplayTagComponent->GetActiveGameplayTags().HasTag(SharedGameplayTags::TAG_Character_Status_Holding_Arm))
	{
		//Trying to throw arm skill check
		if (PLSkillCheckComponent->SkillCheck())
		{
			PLGameplayTagComponent->Server_RemoveTag(SharedGameplayTags::TAG_Character_Status_Holding_Arm);
			PLThrowComponent->Net_StartThrow();
		}
		else
		{
			PLThrowComponent->Net_TryDrop();
		}
	}
}

void APLPlayerCharacter_Zombie::Net_Pounce_Implementation()
{
	//Check if we can pounce, Add more blocked tags here
	FGameplayTagContainer BlockedTags;
	BlockedTags.AddTag(SharedGameplayTags::TAG_Character_Status_Stunned);
	BlockedTags.AddTag(SharedGameplayTags::TAG_Character_Status_Spawning);
	BlockedTags.AddTag(SharedGameplayTags::TAG_Character_Status_Pouncing);
	BlockedTags.AddTag(SharedGameplayTags::TAG_Ability_Pounce_Cooldown);
	BlockedTags.AddTag(SharedGameplayTags::TAG_Character_Status_Armless);

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

	Server_PlayAnimation(GetAnimationData()->PounceMontage);
	
	SetActorRotation(NewActorRotation);

	//Check LoS
	bool bHasLoS = false;
	if (!HitResult.bBlockingHit || !IsValid(HitResult.GetActor()))
	{
		bHasLoS = false;
	}
	else
	{
		FHitResult LoSHit;
		ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4);
		const FVector LoSTraceEnd = HitResult.GetActor()->GetActorLocation();
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(this);
		GetWorld()->LineTraceSingleByChannel(LoSHit, StartLocation, LoSTraceEnd, ECollisionChannel::ECC_GameTraceChannel4, CollisionQueryParams);
		bHasLoS = !LoSHit.bBlockingHit;
	}
	Server_Pounce(NewActorRotation, HitResult, bHasLoS);

}

void APLPlayerCharacter_Zombie::Server_Pounce_Implementation(FRotator NewRotation, FHitResult HitResult, const bool bHasLOS)
{
	SetActorRotation(NewRotation);

	//Start Cooldown
	PLGameplayTagComponent->Server_AddTag(SharedGameplayTags::TAG_Ability_Pounce_Cooldown);
	FText Message = FText::FromString("POUNCE COOLDOWN");
	PLPlayerController->Client_AddTimer(PounceCooldownTime, Message, false);
	GetWorldTimerManager().SetTimer(PounceCooldownTimer, this, &APLPlayerCharacter_Zombie::Server_OnPounceCooldownFinished, PounceCooldownTime);

	if (!HitResult.bBlockingHit || !IsValid(HitResult.GetActor()) || !bHasLOS)
	{
		return;
	}

	if (APLPlayerCharacter_Elder* OtherPlayer = Cast<APLPlayerCharacter_Elder>(HitResult.GetActor()))
	{
		if (!(OtherPlayer->FindComponentByClass<UPLGameplayTagComponent>()->GetActiveGameplayTags().HasTag(SharedGameplayTags::TAG_Character_Affiliation_Elder)))
		{
			return;
		}

		APLPlayerState* PLPlayerState = PLPlayerController->GetPlayerState<APLPlayerState>();
		APLGameMode_Infection* PLGameMode = Cast<APLGameMode_Infection>(UGameplayStatics::GetGameMode(GetWorld()));
		checkf(PLGameMode, TEXT("PLGamemode infection is invalid"));
		APLGameState_Infection* PLGameState = Cast<APLGameState_Infection>(UGameplayStatics::GetGameState(GetWorld()));
		checkf(PLGameState, TEXT("PLGamestate infection is invalid"));
		PLGameMode->SpawnConvertedZombie(OtherPlayer);

		bool bIsAlphaZombie = GetGameplayTagComponent()->GetActiveGameplayTags().HasTagExact(SharedGameplayTags::TAG_Character_Affiliation_Zombie_Alpha);
		PLPlayerState->Server_IncreaseConversion();
		//Add Score
		PLPlayerState->GetPLScoreComponent()->Server_AddScoreFromConversionStreak(PLPlayerState->GetConversionsThisRound(), bIsAlphaZombie);
		if (!bIsAlphaZombie)
		{
			PLGameState->GiveAlphaZombieAssist();
		}
	}
}

bool APLPlayerCharacter_Zombie::Server_Pounce_Validate(FRotator NewRotation, FHitResult HitResult, const bool bHasLOS)
{
	return true;
}


void APLPlayerCharacter_Zombie::Server_OnPounceCooldownFinished()
{
	PLGameplayTagComponent->Server_RemoveTag(SharedGameplayTags::TAG_Ability_Pounce_Cooldown);
}

void APLPlayerCharacter_Zombie::Net_DetachArm_Implementation()
{
	FGameplayTagContainer BlockedTags;
	BlockedTags.AddTag(SharedGameplayTags::TAG_Character_Status_Armless);
	BlockedTags.AddTag(SharedGameplayTags::TAG_Character_Status_Spawning);

	if (GetGameplayTagComponent()->GetActiveGameplayTags().HasAny(BlockedTags))
	{
		return;
	}
	Server_DetachArm();
	GetSkillCheckComponent()->StartSkillCheck();
}

void APLPlayerCharacter_Zombie::Server_DetachArm_Implementation()
{
	FGameplayTagContainer BlockedTags;
	BlockedTags.AddTag(SharedGameplayTags::TAG_Character_Status_Armless);

	if (GetGameplayTagComponent()->GetActiveGameplayTags().HasAny(BlockedTags))
	{
		return;
	}
	FTransform ArmTransform;
	GetMesh()->GetSocketLocation(FName("upperarm_l"));
	GetGameplayTagComponent()->Server_AddTag(SharedGameplayTags::TAG_Character_Status_Armless);
	Multicast_DetachArm();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParameters.Owner = this;
	checkf(ThrowableArmClass, TEXT("Throwable Arm Class is invalid. Please set it up in class defaults"));
	ThrowableArm = GetWorld()->SpawnActor<APLThrowableBase>(ThrowableArmClass, GetActorTransform(), SpawnParameters);
	if (HasAuthority())
	{
		PLThrowComponent->Server_HoldObject(ThrowableArm);
		PLGameplayTagComponent->Server_AddTag(SharedGameplayTags::TAG_Character_Status_Holding_Arm);
	}
}

void APLPlayerCharacter_Zombie::Multicast_DetachArm_Implementation()
{
	GetMesh()->SetSkeletalMesh(ArmlessMesh, false);
}

void APLPlayerCharacter_Zombie::Server_AttachArm_Implementation(APLThrowableArm* PLThrowableArm)
{
	PLThrowableArm->Destroy();
	Multicast_AttachArm();
}

bool APLPlayerCharacter_Zombie::Server_AttachArm_Validate(APLThrowableArm* PLThrowableArm)
{
	return true;
}

void APLPlayerCharacter_Zombie::Multicast_AttachArm_Implementation()
{
	GetMesh()->SetSkeletalMesh(ArmedMesh, false);
}

void APLPlayerCharacter_Zombie::Server_RunBrainMeter_Implementation()
{
	APLGameState_Infection* PLInfectionGameState = GetWorld()->GetGameState<APLGameState_Infection>();
	if (!IsValid(PLInfectionGameState))
	{
		return;
	}

	PLInfectionGameState->RunBrainMeter();
}

bool APLPlayerCharacter_Zombie::Server_RunBrainMeter_Validate()
{
	return true;
}

void APLPlayerCharacter_Zombie::OnRep_ThrowableArm()
{
	if (IsValid(ThrowableArm))
	{
		PLThrowComponent->Net_HoldObject(ThrowableArm);
	}
}

bool APLPlayerCharacter_Zombie::Server_DetachArm_Validate()
{
	return true;
}
void APLPlayerCharacter_Zombie::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APLPlayerCharacter_Zombie, ThrowableArm);
	DOREPLIFETIME(APLPlayerCharacter_Zombie, ArmedMesh);
}