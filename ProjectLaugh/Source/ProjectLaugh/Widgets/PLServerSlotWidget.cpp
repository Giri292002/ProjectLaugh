// Fill out your copyright notice in the Description page of Project Settings.


#include "PLServerSlotWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ProjectLaugh/Core/PLGameInstance.h"

void UPLServerSlotWidget::NativeConstruct()
{
	ServerNameTextBox->SetText(FText::FromString(ServerInfo.ServerName));
	PlayerCountTextBox->SetText(FText::FromString(ServerInfo.PlayerCountString));
	ServerArrayIndex = ServerInfo.ServerArrayIndex;
	JoinServerButton->OnClicked.AddDynamic(this, &UPLServerSlotWidget::OnJoinServer);

	if (!ensureAlwaysMsgf(PLGameInstance, TEXT("Please set PL Game instance when constructing this widget")))
	{
		return;
	}
}

void UPLServerSlotWidget::OnJoinServer()
{
	PLGameInstance->JoinSession(ServerArrayIndex);
}