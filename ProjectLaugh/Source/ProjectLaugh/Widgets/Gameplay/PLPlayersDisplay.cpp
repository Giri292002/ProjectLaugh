// Fill out your copyright notice in the Description page of Project Settings.


#include "PLPlayersDisplay.h"

#include "Components/HorizontalBox.h" 
#include "ProjectLaugh/Core/Infection/PLGameState_Infection.h"
#include "ProjectLaugh/Data/CharacterUIProfileData.h"
#include "ProjectLaugh/Widgets/Gameplay/PLPlayerProfileSlot.h"

void UPLPlayersDisplay::PLConstruct()
{
	Super::PLConstruct();
	
	APLGameState_Infection* InfectionGameState = Cast<APLGameState_Infection>(PLGameState);
	InfectionGameState->OnCharacterAddOrRemoveSignature.AddDynamic(this, &UPLPlayersDisplay::AddOrRemoveCharacter);
}

void UPLPlayersDisplay::AddOrRemoveCharacter(FGameplayTag AffiliationTag, UCharacterUIProfileData* CharacterUIData)
{
	AddCharacter(AffiliationTag, CharacterUIData);
}

void UPLPlayersDisplay::AddCharacter(FGameplayTag AffiliationTag, UCharacterUIProfileData* CharacterUIData)
{
	UPLPlayerProfileSlot* PlayerProfile = CreateWidget<UPLPlayerProfileSlot>(this, PlayerProfileWidgetClass);
	PlayerProfile->Setup(CharacterUIData);
	if (AffiliationTag == SharedGameplayTags::TAG_Character_Affiliation_Elder)
	{
		ElderPlayersDisplay->AddChildToHorizontalBox(PlayerProfile);
	}
}
