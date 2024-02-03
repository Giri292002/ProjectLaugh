// Fill out your copyright notice in the Description page of Project Settings.


#include "PLMainMenuWidget.h"

#include "Components/Button.h"
#include "Components/CircularThrobber.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "ProjectLaugh/Core/PLEOSGameInstance.h"
#include "ProjectLaugh/Widgets/PLServerSlotWidget.h"
#include "Kismet/GameplayStatics.h"

void UPLMainMenuWidget::NativePreConstruct()
{
}

void UPLMainMenuWidget::NativeConstruct()
{
	OpenCreateServerDialogButton->OnClicked.AddDynamic(this, &UPLMainMenuWidget::OnOpenCreateServerDialogButtonClicked);
	CreateServerButton->OnClicked.AddDynamic(this, &UPLMainMenuWidget::OnCreateServerButtonClicked);
	ViewServersButton->OnClicked.AddDynamic(this, &UPLMainMenuWidget::OnViewServerButtonClicked);
	BackToMainFromViewServerButton->OnClicked.AddDynamic(this, &UPLMainMenuWidget::OnBackToMainButtonClicked);
	BackToMainFromCreateServerButton->OnClicked.AddDynamic(this, &UPLMainMenuWidget::OnBackToMainButtonClicked);
	RefreshServerButton->OnClicked.AddDynamic(this, &UPLMainMenuWidget::OnRefreshServerButtonClicked);
	QuitButton->OnClicked.AddDynamic(this, &UPLMainMenuWidget::OnQuitButtonClicked);

	PLGameInstance = Cast<UPLEOSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!ensureAlwaysMsgf(PLGameInstance, TEXT("PL Game instance is invalid")))
	{
		return;
	}
	//PLGameInstance->SearchingForServerDelegate.AddDynamic(this, &UPLMainMenuWidget::OnSearchingForServers);
}

void UPLMainMenuWidget::OnOpenCreateServerDialogButtonClicked()
{
	MenuWidgetSwitcher->SetActiveWidgetIndex(2);
}

void UPLMainMenuWidget::OnCreateServerButtonClicked()
{	
	FString ServerName = ServerNameEditableTextBox->GetText().ToString();
	FString HostName = HostNameEditableTextBox->GetText().ToString();
	PLGameInstance->CreateSession(FMath::TruncToInt32(PlayerCountSlider->GetValue()));
}

void UPLMainMenuWidget::OnViewServerButtonClicked()
{
	MenuWidgetSwitcher->SetActiveWidgetIndex(1);
	OnRefreshServerButtonClicked();
}

void UPLMainMenuWidget::OnQuitButtonClicked()
{
	if (ensureAlwaysMsgf(PLGameInstance, TEXT("PLGameInstance is invalid")))
	{
		PLGameInstance->DestroySession();
		FTimerHandle QuitGameHandle;
		GetWorld()->GetTimerManager().SetTimer(QuitGameHandle, this, &UPLMainMenuWidget::ExecuteQuitGame, 0.5f);
	}
}

void UPLMainMenuWidget::ExecuteQuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(),0), EQuitPreference::Quit, false);
}


void UPLMainMenuWidget::OnBackToMainButtonClicked()
{
	MenuWidgetSwitcher->SetActiveWidgetIndex(0);
}

void UPLMainMenuWidget::OnRefreshServerButtonClicked()
{
	/*if (!PLGameInstance->ServerListDelegate.Contains(this, FName("OnServerAdd")))
	{
		UE_LOG(LogTemp, Log, TEXT("BINDING TO PLGameInstance"));
		PLGameInstance->ServerListDelegate.AddDynamic(this, &UPLMainMenuWidget::OnServerAdd);
	}

	PLGameInstance->FindSessions();*/
}


void UPLMainMenuWidget::OnServerAdd(FServerInfo ServerListDelegates)
{
	if (!ensureAlwaysMsgf(ServerSlotWidgetClass, TEXT("Server Slot Widget Class is invalid")))
	{
		return;
	}

	FindServersThrobber->SetVisibility(ESlateVisibility::Hidden);
	UPLServerSlotWidget* ServerSlot = CreateWidget<UPLServerSlotWidget>(GetWorld(), ServerSlotWidgetClass);
	ServerSlot->ServerInfo = ServerListDelegates;
	//ServerSlot->PLGameInstance = PLGameInstance;
	ScrollBox->AddChild(ServerSlot);
	UE_LOG(LogTemp, Log, TEXT("Found Server"));
}

void UPLMainMenuWidget::OnSearchingForServers(bool bIsSearching)
{
	FindServersThrobber->SetVisibility(bIsSearching? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	RefreshServerButton->SetIsEnabled(!bIsSearching);
}
