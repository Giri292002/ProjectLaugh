// Fill out your copyright notice in the Description page of Project Settings.


#include "PLThrowableArm.h"

#include "Kismet/GameplayStatics.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"
#include "ProjectLaugh/Core/Infection/PLGameMode_Infection.h"
#include "ProjectLaugh/Gameplay/Characters/PLPlayerCharacter_Zombie.h"
#include "ProjectLaugh/Gameplay/Characters/PLPlayerCharacter_Elder.h"
#include "ProjectLaugh/Gameplay/PLGameplayTagComponent.h"

APLThrowableArm::APLThrowableArm()
{
}

void APLThrowableArm::Interact_Implementation(APLPlayerCharacter* InInstigator, UPLInteractionComponent* OtherInteractableComponent)
{
	InInstigator->GetGameplayTagComponent()->Server_RemoveTag(SharedGameplayTags::TAG_Character_Status_Armless);
	APLPlayerCharacter_Zombie* Zombie = Cast<APLPlayerCharacter_Zombie>(InInstigator);
	Zombie->Server_AttachArm();
	checkf(Zombie, TEXT("Cant cast InInstigator to zombie."));
	Server_Destroy();
}

void APLThrowableArm::OnActorHitWithObject(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority() && GameplayTagComponent->GetActiveGameplayTags().HasTag(SharedGameplayTags::TAG_Ability_Throw_Thrown))
	{
		if (OtherActor)
		{
			if (APLPlayerCharacter_Elder* Elder = Cast<APLPlayerCharacter_Elder>(OtherActor))
			{
				APLGameMode_Infection* PLGameMode = Cast<APLGameMode_Infection>(UGameplayStatics::GetGameMode(GetWorld()));
				checkf(PLGameMode, TEXT("PLGamemode infection is invalid"));
				PLGameMode->SpawnConvertedZombie(Elder);
			}
			if (OtherActor != PreviouslyHitActor)
			{
				Multicast_SpawnHitFX(Hit.ImpactPoint);
				PreviouslyHitActor = OtherActor;
			}
		}

	}
}

void APLThrowableArm::Server_Destroy_Implementation()
{
	Destroy();
}

bool APLThrowableArm::Server_Destroy_Validate()
{
	return true;
}


