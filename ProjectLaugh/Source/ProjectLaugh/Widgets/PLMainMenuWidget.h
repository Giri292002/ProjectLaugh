// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PLMainMenuWidget.generated.h"

class UButton;
class UPLEOSGameInstance;
class UWidgetSwitcher;
class UScrollBox;
class UPLServerSlotWidget;
class UCircularThrobber;
class UEditableTextBox;
class USlider;
class UTextBlock;

UCLASS()
class PROJECTLAUGH_API UPLMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// --- Create Room Dialog ---
	UPROPERTY(meta = (BindWidget))
	UButton* OpenCreateRoomDialogButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CreateRoomButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BackToMainFromCreateRoomButton;

	// --- Join Room Dialog ---
	UPROPERTY(meta = (BindWidget))
	UButton* OpenJoinRoomDialogButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinRoomButton;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* RoomCodeInputTextBox;

	UPROPERTY(meta = (BindWidget))
	UButton* BackToMainFromJoinRoomButton;

	// --- Main Menu Dialog ---

	UPROPERTY(meta=(BindWidget))
	UButton* QuitButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher* MenuWidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	UCircularThrobber* FindServersThrobber;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* PlayerCountSlider;


	UFUNCTION()
	void OnOpenCreateRoomDialogButtonClicked();

	UFUNCTION()
	void OnCreateRoomButtonClicked();

	UFUNCTION()
	void OnJoinRoomButtonClicked();

	UFUNCTION()
	void OnBackToMainButtonClicked();

	UFUNCTION()
	void OnOpenJoinRoomButtonClicked();

	UFUNCTION()
	void OnQuitButtonClicked();

	UFUNCTION()
	void ExecuteQuitGame();

	UFUNCTION()
	void OnRoomCodeInputTextBoxChanged(const FText& NewText);

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	UPLEOSGameInstance* PLEOSGameInstance;
	
};
