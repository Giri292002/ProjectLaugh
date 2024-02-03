// Fill out your copyright notice in the Description page of Project Settings.


#include "PLEOSGameInstance.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"
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

void UPLEOSGameInstance::Shutdown()
{
	DestroySession();
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
			UE_LOG(LogPLEOS, Log, TEXT("Running in editor, using Dev Auth Credentials"));
#endif

			//If its a build, tell user to login
#if !WITH_EDITOR
			FOnlineAccountCredentials Credentials;
			Credentials.Id = FString();
			Credentials.Token = FString();
			Credentials.Type = FString("accountportal");
			UE_LOG(LogPLEOS, Log, TEXT("Running in build, using Account Portal"));
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
			TSharedRef<FOnlineSessionSettings>SessionSettings = MakeShared<FOnlineSessionSettings>();
			SessionSettings->bIsDedicated = false;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bIsLANMatch = false;
			SessionSettings->NumPublicConnections = MaxPlayers;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bUsesPresence = true;
			SessionSettings->bUseLobbiesIfAvailable = true;
			SessionSettings->Settings.Add(FName(TEXT("SessionSetting")), FOnlineSessionSetting(FString(TEXT("SettingValue")), EOnlineDataAdvertisementType::ViaOnlineService));
			SessionSettings->Set(SEARCH_KEYWORDS, FString(TEXT("OldWarZListenLobby")), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

			SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UPLEOSGameInstance::OnCreateSessionComplete);
			SessionPtr->CreateSession(0, SessionName, *SessionSettings);
			UE_LOG(LogPLEOS, Log, TEXT("Requested to create session"));
		}
	}
}

void UPLEOSGameInstance::OnCreateSessionComplete(FName InSessionName, bool bIsSuccesful)
{
	UE_LOG(LogTemp, Warning, TEXT("Success: %d"), bIsSuccesful);

	if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
	{
		SessionPtr->ClearOnCreateSessionCompleteDelegates(this);
		GetWorld()->ServerTravel("/Game/Levels/Art_Test/NewMap?listen");
	}
}

void UPLEOSGameInstance::DestroySession()
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
			SessionPtr->OnDestroySessionCompleteDelegates.AddUObject(this, &UPLEOSGameInstance::OnDestroySessionComplete);
			SessionPtr->DestroySession(SessionName);
		}
	}
}

void UPLEOSGameInstance::OnDestroySessionComplete(FName InSessionName, bool bWasSuccessful)
{
	if (ensure(OnlineSubsystem))
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			SessionPtr->ClearOnRegisterPlayersCompleteDelegates(this);
		}
	}
}


