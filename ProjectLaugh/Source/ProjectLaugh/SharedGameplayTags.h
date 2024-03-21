// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace SharedGameplayTags
{	
	// Ability
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Pounce_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Throw_Thrown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Inhale_Inhaling);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Hide_Hiding);

	// Character
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Affiliation_Elder);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Affiliation_Zombie);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Affiliation_Zombie_Alpha);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Affiliation_Zombie_Beta);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Status_Stunned);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Status_Pouncing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Status_Spawning);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Status_Armless);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Status_Holding);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Status_Holding_Arm);

	//Results
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Result_Position_01);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Result_Position_02);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Result_Position_03);

	//Interactables
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Interactable_Status_Open);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Interactable_Status_Close);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Interactable_Hidable_Status_HasOccupant);
}