// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PLMainMenuWidget.generated.h"

class UButton;
class UPLGameInstance;
class UWidgetSwitcher;
class UScrollBox;
class UPLServerSlotWidget;
class UCircularThrobber;
class UEditableTextBox;
class USlider;

UCLASS()
class PROJECTLAUGH_API UPLMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* OpenCreateServerDialogButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CreateServerButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ViewServersButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BackToMainFromViewServerButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BackToMainFromCreateServerButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RefreshServerButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher* MenuWidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBox;

	UPROPERTY(meta = (BindWidget))
	UCircularThrobber* FindServersThrobber;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* HostNameEditableTextBox;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ServerNameEditableTextBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* PlayerCountSlider;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPLServerSlotWidget> ServerSlotWidgetClass;


	UFUNCTION()
	void OnOpenCreateServerDialogButtonClicked();

	UFUNCTION()
	void OnCreateServerButtonClicked();

	UFUNCTION()
	void OnRefreshServerButtonClicked();

	UFUNCTION()
	void OnBackToMainButtonClicked();

	UFUNCTION()
	void OnViewServerButtonClicked();

	UFUNCTION()
	void OnServerAdd(FServerInfo ServerListDelegates);

	UFUNCTION()
	void OnSearchingForServers(bool bIsSearching);

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	UPLGameInstance* PLGameInstance;
	
};
