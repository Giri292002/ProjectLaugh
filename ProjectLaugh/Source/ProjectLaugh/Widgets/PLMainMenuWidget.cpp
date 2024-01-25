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
	PLGameInstance->SearchingForServerDelegate.AddDynamic(this, &UPLMainMenuWidget::OnSearchingForServers);
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
	PLGameInstance->ServerListDelegate.AddDynamic(this, &UPLMainMenuWidget::OnServersRefreshed);

	PLGameInstance->FindSessions();
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
	ServerSlot->PLGameInstance = PLGameInstance;
	ScrollBox->AddChild(ServerSlot);
	UE_LOG(LogTemp, Log, TEXT("Found Server"));
}

void UPLMainMenuWidget::OnSearchingForServers(bool bIsSearching)
{
	FindServersThrobber->SetVisibility(bIsSearching? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	RefreshServerButton->SetIsEnabled(!bIsSearching);
}
