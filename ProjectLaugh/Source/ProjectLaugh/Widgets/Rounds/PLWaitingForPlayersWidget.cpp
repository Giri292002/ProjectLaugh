// Fill out your copyright notice in the Description page of Project Settings.


#include "PLWaitingForPlayersWidget.h"
#include "Components/TextBlock.h"
#include "ProjectLaugh/Core/PLEOSGameInstance.h"

void UPLWaitingForPlayersWidget::NativeConstruct()
{
	UPLEOSGameInstance* PLEOSGameInstance = Cast<UPLEOSGameInstance>(GetWorld()->GetGameInstance());
	if (!ensureAlwaysMsgf(PLEOSGameInstance, TEXT("Invalid PLEOS Game instance")))
	{
		return;
	}

	RoomCodeText->SetText(FText::FromString("ROOM CODE: " + PLEOSGameInstance->GetRoomCode()));
}
