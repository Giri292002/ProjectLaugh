// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PLGameInstance.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPLGameInstance, Log, All);

USTRUCT(BlueprintType)
struct FServerInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FString ServerName;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayers;

	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayers;

	UPROPERTY(BlueprintReadOnly)
	FString PlayerCountString;

	void SetPlayerCount()
	{
		PlayerCountString = FString(FString::FromInt(CurrentPlayers) + "/" + FString::FromInt(MaxPlayers));
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerSignature, FServerInfo, ServersInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerSearchingSignature, bool, bIsSearchingForServer);

UCLASS()
class PROJECTLAUGH_API UPLGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPLGameInstance();

	UPROPERTY(BlueprintAssignable)
	FServerSignature ServerListDelegate;

	UPROPERTY(BlueprintAssignable)
	FServerSearchingSignature SearchingForServerDelegate;

	UFUNCTION(BlueprintCallable)
	void CreateServer(FString ServerName, FString HostName);

	UFUNCTION(BlueprintCallable)
	void FindSessions();

protected:

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	IOnlineSessionPtr SessionInterface;

	virtual void OnCreateSessionComplete(FName SessionName, bool bSucceeded);
	virtual void OnFindSessionComplete(bool bSucceeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	virtual void Init() override;	
};
