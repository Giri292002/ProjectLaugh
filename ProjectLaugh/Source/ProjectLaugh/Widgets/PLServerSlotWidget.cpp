// Fill out your copyright notice in the Description page of Project Settings.


#include "PLServerSlotWidget.h"
#include "Components/TextBlock.h"

void UPLServerSlotWidget::NativeConstruct()
{
	ServerNameTextBox->SetText(FText::FromString(ServerInfo.ServerName));
	PlayerCountTextBox->SetText(FText::FromString(ServerInfo.PlayerCountString));
}
