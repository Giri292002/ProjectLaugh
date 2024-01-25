// Fill out your copyright notice in the Description page of Project Settings.


#include "PLMainMenuWidget.h"

#include "Components/Button.h"
#include "Components/CircularThrobber.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "ProjectLaugh/Core/PLGameInstance.h"
#include "ProjectLaugh/Widgets/PLServerSlotWidget.h"
#include "Kismet/GameplayStatics.h"

void UPLMainMenuWidget::NativeConstruct()
{
	OpenCreateServerDialogButton->OnClicked.AddDynamic(this, &UPLMainMenuWidget::OnOpenCreateServerDialogButtonClicked);
	CreateServerButton->OnClicked.AddDynamic(this, &UPLMainMenuWidget::OnCreateServerButtonClicked);
	ViewServersButton->OnClicked.AddDynamic(this, &UPLMainMenuWidget::OnViewServerButtonClicked);
	BackToMainFromViewServerButton->OnClicked.AddDynamic(this, &UPLMainMenuWidget::OnBackToMainButtonClicked);
	BackToMainFromCreateServerButton->OnClicked.AddDynamic(this, &UPLMainMenuWidget::OnBackToMainButtonClicked);
	RefreshServerButton->OnClicked.AddDynamic(this, &UPLMainMenuWidget::OnRefreshServerButtonClicked);

	PLGameInstance = Cast<UPLGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!ensureAlwaysMsgf(PLGameInstance, TEXT("PL Game instance is invalid")))
	{
		return;
	}
}

void UPLMainMenuWidget::OnOpenCreateServerDialogButtonClicked()
{
	MenuWidgetSwitcher->SetActiveWidgetIndex(2);
}

void UPLMainMenuWidget::OnCreateServerButtonClicked()
{	
	FString ServerName = ServerNameEditableTextBox->GetText().ToString();
	FString HostName = HostNameEditableTextBox->GetText().ToString();
	PLGameInstance->CreateServer(ServerName, HostName);
}

void UPLMainMenuWidget::OnViewServerButtonClicked()
{
	MenuWidgetSwitcher->SetActiveWidgetIndex(1);
	OnRefreshServerButtonClicked();
}


void UPLMainMenuWidget::OnBackToMainButtonClicked()
{
	MenuWidgetSwitcher->SetActiveWidgetIndex(0);
}

void UPLMainMenuWidget::OnRefreshServerButtonClicked()
{
	if (ScrollBox->HasAnyChildren())
	{
		ScrollBox->ClearChildren();
	}
	FindServersThrobber->SetVisibility(ESlateVisibility::Visible);

	PLGameInstance->FindSessions();
	if (!ensureAlwaysMsgf(PLGameInstance, TEXT("Game Instance is invalid")))
	{
		return;
	}

	PLGameInstance->ServerListDelegate.AddDynamic(this, &UPLMainMenuWidget::OnServersRefreshed);
}


void UPLMainMenuWidget::OnServersRefreshed(FServerInfo ServerListDelegates)
{
	if (!ensureAlwaysMsgf(ServerSlotWidgetClass, TEXT("Server Slot Widget Class is invalid")))
	{
		return;
	}

	FindServersThrobber->SetVisibility(ESlateVisibility::Hidden);
	UPLServerSlotWidget* ServerSlot = CreateWidget<UPLServerSlotWidget>(GetWorld(), ServerSlotWidgetClass);
	ServerSlot->ServerInfo = ServerListDelegates;
	ScrollBox->AddChild(ServerSlot);
	UE_LOG(LogTemp, Log, TEXT("Found Server"));
}