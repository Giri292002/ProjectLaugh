// Fill out your copyright notice in the Description page of Project Settings.


#include "PLMainMenuWidget.h"

#include "Components/Button.h"
#include "Components/CircularThrobber.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/Slider.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectLaugh/Alphabets.h"
#include "ProjectLaugh/Core/PLEOSGameInstance.h"
#include "ProjectLaugh/Widgets/PLServerSlotWidget.h"

void UPLMainMenuWidget::NativePreConstruct()
{
}

void UPLMainMenuWidget::NativeConstruct()
{
	OpenCreateRoomDialogButton->OnClicked.AddDynamic(this, &UPLMainMenuWidget::OnOpenCreateRoomDialogButtonClicked);
	CreateRoomButton->OnClicked.AddDynamic(this, &UPLMainMenuWidget::OnCreateRoomButtonClicked);
	OpenJoinRoomDialogButton->OnClicked.AddDynamic(this, &UPLMainMenuWidget::OnOpenJoinRoomButtonClicked);
	JoinRoomButton->OnClicked.AddDynamic(this, &UPLMainMenuWidget::OnJoinRoomButtonClicked);
	BackToMainFromJoinRoomButton->OnClicked.AddDynamic(this, &UPLMainMenuWidget::OnBackToMainButtonClicked);
	BackToMainFromCreateRoomButton->OnClicked.AddDynamic(this, &UPLMainMenuWidget::OnBackToMainButtonClicked);
	QuitButton->OnClicked.AddDynamic(this, &UPLMainMenuWidget::OnQuitButtonClicked);
	RoomCodeInputTextBox->OnTextChanged.AddDynamic(this, &UPLMainMenuWidget::OnRoomCodeInputTextBoxChanged);

	PLEOSGameInstance = Cast<UPLEOSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!ensureAlwaysMsgf(PLEOSGameInstance, TEXT("PL Game instance is invalid")))
	{
		return;
	}
}

void UPLMainMenuWidget::OnOpenCreateRoomDialogButtonClicked()
{
	MenuWidgetSwitcher->SetActiveWidgetIndex(2);
}

void UPLMainMenuWidget::OnCreateRoomButtonClicked()
{
	PLEOSGameInstance->CreateSession(FMath::TruncToInt32(PlayerCountSlider->GetValue()));
}

void UPLMainMenuWidget::OnOpenJoinRoomButtonClicked()
{
	MenuWidgetSwitcher->SetActiveWidgetIndex(1);
	RoomCodeInputTextBox->SetKeyboardFocus();
}

void UPLMainMenuWidget::OnQuitButtonClicked()
{
	if (ensureAlwaysMsgf(PLEOSGameInstance, TEXT("PLEOSGameInstance is invalid")))
	{
		PLEOSGameInstance->DestroySession();
		FTimerHandle QuitGameHandle;
		GetWorld()->GetTimerManager().SetTimer(QuitGameHandle, this, &UPLMainMenuWidget::ExecuteQuitGame, 0.5f);
	}
}

void UPLMainMenuWidget::ExecuteQuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(),0), EQuitPreference::Quit, false);
}

void UPLMainMenuWidget::OnRoomCodeInputTextBoxChanged(const FText& NewText)
{
	if (!ensureAlwaysMsgf(PLEOSGameInstance, TEXT("Invalid PLEOSGameInstance")))
	{
		return;
	}
	auto CharacterArray = NewText.ToString().ToUpper().GetCharArray();	

	//If unsupported characters have been inputted, remove them
	if (!Alphabets::SupportedAlphabets.Contains(CharacterArray.Last(1)))
	{
		FString ChoppedString = NewText.ToString().LeftChop(1);
		RoomCodeInputTextBox->SetText(FText::FromString(ChoppedString).ToUpper());
	}
	
	//Room character has exceeded, remove the last input letters
	if (NewText.ToString().Len() > 5)
	{
		FString ChoppedString = NewText.ToString().LeftChop(1);
		RoomCodeInputTextBox->SetText(FText::FromString(ChoppedString).ToUpper());
	}

	RoomCodeInputTextBox->SetText(RoomCodeInputTextBox->GetText().ToUpper());
}


void UPLMainMenuWidget::OnBackToMainButtonClicked()
{
	MenuWidgetSwitcher->SetActiveWidgetIndex(0);
}

void UPLMainMenuWidget::OnJoinRoomButtonClicked()
{
	FString RoomCode = RoomCodeInputTextBox->GetText().ToString();
	if (RoomCode.Len() < 5)
	{
		UE_LOG(LogTemp, Error, TEXT("Too short room code."))
		return;
	}
	PLEOSGameInstance->FindSession(RoomCode);
}
