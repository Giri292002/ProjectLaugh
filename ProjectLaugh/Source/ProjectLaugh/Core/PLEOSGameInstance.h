// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PLEOSGameInstance.generated.h"

class IOnlineSubsystem;

DECLARE_LOG_CATEGORY_EXTERN(LogPLEOS, Log, Log);

UCLASS()
class PROJECTLAUGH_API UPLEOSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPLEOSGameInstance();

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void CreateSession(int32 MaxPlayers);

	UFUNCTION()
	void Login();

	void OnCreateSessionComplete(FName SessionName, bool bIsSuccesful);
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

protected:
	IOnlineSubsystem* OnlineSubsystem;

	UPROPERTY()
	bool bIsLoggedIn;
};
