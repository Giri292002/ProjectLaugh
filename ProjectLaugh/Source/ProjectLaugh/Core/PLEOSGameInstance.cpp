// Fill out your copyright notice in the Description page of Project Settings.


#include "PLEOSGameInstance.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

DEFINE_LOG_CATEGORY(LogPLEOS);

UPLEOSGameInstance::UPLEOSGameInstance()
{
	bIsLoggedIn = false;
}

void UPLEOSGameInstance::Init()
{
	Super::Init();

	OnlineSubsystem = IOnlineSubsystem::Get();
	Login();
}

void UPLEOSGameInstance::Login()
{
	if (OnlineSubsystem)
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			//If with editor using DevAuth Credentials
#if WITH_EDITOR
			FOnlineAccountCredentials Credentials;
			Credentials.Id = FString("127.0.0.1:8081");
			Credentials.Token = FString("OldWarzGiriCred");
			Credentials.Type = FString("developer");
#endif

			//If its a build, tell user to login
#if !WITH_EDITOR
			FOnlineAccountCredentials Credentials;
			Credentials.Id = FString();
			Credentials.Token = FString();
			Credentials.Type = FString("accountportal");
#endif
			Identity->OnLoginCompleteDelegates->AddUObject(this, &UPLEOSGameInstance::OnLoginComplete);
			Identity->Login(0, Credentials);
		}
	}

}

void UPLEOSGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	UE_LOG(LogPLEOS, Log, TEXT("Log In: %d"), bWasSuccessful);

	bIsLoggedIn = bWasSuccessful;

	if (ensure(OnlineSubsystem))
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			Identity->ClearOnLoginCompleteDelegates(0, this);
		}
	}
}

void UPLEOSGameInstance::CreateSession(int32 MaxPlayers)
{
	if (!bIsLoggedIn)
	{
		UE_LOG(LogPLEOS, Error, TEXT("Cannot Create Session. User is not logged in"));
		return;
	}
	if (ensure(OnlineSubsystem))
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			FOnlineSessionSettings SessionSettings;
			SessionSettings.bIsDedicated = false;
			SessionSettings.bShouldAdvertise = true;
			SessionSettings.bIsLANMatch = true;
			SessionSettings.NumPublicConnections = MaxPlayers;
			SessionSettings.bAllowJoinInProgress = true;
			SessionSettings.bAllowJoinViaPresence = true;
			SessionSettings.bUsesPresence = true;

			SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UPLEOSGameInstance::OnCreateSessionComplete);
			SessionPtr->CreateSession(0, FName("Old War Z Session"), SessionSettings);
		}
	}
}

void UPLEOSGameInstance::OnCreateSessionComplete(FName SessionName, bool bIsSuccesful)
{
	UE_LOG(LogTemp, Warning, TEXT("Success: %d"), bIsSuccesful);

	if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
	{
		SessionPtr->ClearOnCreateSessionCompleteDelegates(this);
	}
}


