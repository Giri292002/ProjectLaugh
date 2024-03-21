// Fill out your copyright notice in the Description page of Project Settings.


#include "SharedGameplayTags.h"

namespace SharedGameplayTags
{
	//Ability
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Pounce_Cooldown, "Ability.Pounce.Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Throw_Thrown, "Ability.Throw.Thrown");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Inhale_Inhaling, "Ability.Inhale.Inhaling");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Hide_Hiding, "Ability.Hide.Hiding");

	//Character
	UE_DEFINE_GAMEPLAY_TAG(TAG_Character_Affiliation_Elder, "Character.Affiliation.Elder");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Character_Affiliation_Zombie, "Character.Affiliation.Zombie");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Character_Affiliation_Zombie_Alpha, "Character.Affiliation.Zombie.Alpha");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Character_Affiliation_Zombie_Beta, "Character.Affiliation.Zombie.Beta");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Character_Status_Armless, "Character.Status.Armless");	
	UE_DEFINE_GAMEPLAY_TAG(TAG_Character_Status_Pouncing, "Character.Status.Pouncing");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Character_Status_Holding, "Character.Status.Holding");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Character_Status_Holding_Arm, "Character.Status.Holding.Arm");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Character_Status_Spawning, "Character.Status.Spawning");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Character_Status_Stunned, "Character.Status.Stunned");

	//Results
	UE_DEFINE_GAMEPLAY_TAG(TAG_Result_Position_01, "Result.Position.One");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Result_Position_02, "Result.Position.Two");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Result_Position_03, "Result.Position.Three");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Interactable_Status_Open, "Interactable.Status.Open");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Interactable_Status_Close, "Interactable.Status.Close");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Interactable_Hidable_Status_HasOccupant, "Interactable.Hidable.Status.HasOccupant");
}
