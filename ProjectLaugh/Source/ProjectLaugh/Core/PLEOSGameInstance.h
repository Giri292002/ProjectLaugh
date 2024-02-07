// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "PLEOSGameInstance.generated.h"

class IOnlineSubsystem;
const FName PLSessionName = FName("Old War Z Session");

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
	void FindSession(FString InRoomCode);

	UFUNCTION()
	void Login();

	UFUNCTION(BlueprintCallable)
	FString GetRoomCode() const { return RoomCode; }

	UFUNCTION(BlueprintCallable)
	int GetMaxPlayersInCurrentLobby() const { return MaxPlayersInCurrentLobby; }

	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	void OnCreateSessionComplete(FName SessionName, bool bIsSuccesful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccesful);
	void JoinEOSSession();
	void OnJoinSessionCompleteCallback(FName InSessionName, EOnJoinSessionCompleteResult::Type Result);

protected:
	IOnlineSubsystem* OnlineSubsystem;

	UPROPERTY()
	bool bIsLoggedIn;

	UPROPERTY()
	FString RoomCode;

	UFUNCTION()
	FString GenerateRoomCode();

	UPROPERTY()
	int MaxPlayersInCurrentLobby;	

private:
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	FOnlineSessionSearchResult* SessionToJoin;
	FString ConnectString;
};
