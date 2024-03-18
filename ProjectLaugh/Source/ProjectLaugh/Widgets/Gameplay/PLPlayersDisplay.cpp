// Fill out your copyright notice in the Description page of Project Settings.


#include "PLPlayersDisplay.h"

#include "Components/HorizontalBox.h" 
#include "ProjectLaugh/Core/PLPlayerCharacter.h"
#include "ProjectLaugh/Data/PLPlayerAttributesData.h"
#include "ProjectLaugh/Core/Infection/PLGameState_Infection.h"
#include "ProjectLaugh/Data/CharacterUIProfileData.h"
#include "ProjectLaugh/Widgets/Gameplay/PLPlayerProfileSlot.h"

void UPLPlayersDisplay::PLConstruct()
{
	InfectionGameState = Cast<APLGameState_Infection>(PLGameState);
	InfectionGameState->OnCharacterAddOrRemoveSignature.AddDynamic(this, &UPLPlayersDisplay::RefreshCharacterDisplay);
	RefreshCharacterDisplay();
}

void UPLPlayersDisplay::RefreshCharacterDisplay()
{
	//Clear previous display
	TArray<UWidget*> ElderWidgetChildren = ElderPlayersDisplay->GetAllChildren();
	TArray<UWidget*> ZombieWidgetChildren = ZombiePlayersDisplay->GetAllChildren();

	for (UWidget* Widget : ElderWidgetChildren)
	{
		ElderPlayersDisplay->RemoveChild(Widget);
	}

	for (UWidget* Widget : ZombieWidgetChildren)
	{
		ZombiePlayersDisplay->RemoveChild(Widget);
	}

	for (APLPlayerCharacter* PLPlayerCharacter : InfectionGameState->GetCharactersInGame())
	{
		if (!IsValid(PLPlayerCharacter))
		{
			continue;
		}

		UPLPlayerProfileSlot* PlayerProfile = CreateWidget<UPLPlayerProfileSlot>(this, PlayerProfileWidgetClass);
		if (!IsValid(PlayerProfile))
		{
			continue;
		}

		if (!IsValid(PLPlayerCharacter->GetCharacterUIData()))
		{
			continue;
		}

		PlayerProfile->Setup(PLPlayerCharacter->GetCharacterUIData());
		if (PLPlayerCharacter->GetGameplayTagComponent()->GetActiveGameplayTags().HasTag(SharedGameplayTags::TAG_Character_Affiliation_Elder))
		{
			ElderPlayersDisplay->AddChildToHorizontalBox(PlayerProfile);
		}
		else if (PLPlayerCharacter->GetGameplayTagComponent()->GetActiveGameplayTags().HasTag(SharedGameplayTags::TAG_Character_Affiliation_Zombie))
		{
			ZombiePlayersDisplay->AddChildToHorizontalBox(PlayerProfile);
		}
	}
}
