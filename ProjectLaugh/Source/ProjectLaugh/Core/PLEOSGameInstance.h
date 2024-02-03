// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PLEOSGameInstance.generated.h"

class IOnlineSubsystem;

const FName SessionName = FName("Old War Z Session");

DECLARE_LOG_CATEGORY_EXTERN(LogPLEOS, Verbose, All);

UCLASS()
class PROJECTLAUGH_API UPLEOSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPLEOSGameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable)
	void CreateSession(int32 MaxPlayers);

	UFUNCTION(BlueprintCallable)
	void DestroySession();

	UFUNCTION()
	void Login();

	void OnCreateSessionComplete(FName SessionName, bool bIsSuccesful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

protected:
	IOnlineSubsystem* OnlineSubsystem;

	UPROPERTY()
	bool bIsLoggedIn;
};
