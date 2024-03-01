// Fill out your copyright notice in the Description page of Project Settings.


#include "PLThrowableArm.h"

#include "ProjectLaugh/Core/PLPlayerCharacter.h"
#include "ProjectLaugh/Gameplay/Characters/PLPlayerCharacter_Zombie.h"
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

void APLThrowableArm::Server_Destroy_Implementation()
{
	Destroy();
}

bool APLThrowableArm::Server_Destroy_Validate()
{
	return true;
}


