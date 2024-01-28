// Fill out your copyright notice in the Description page of Project Settings.


#include "PLGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "AdvancedSessionsLibrary.h"
#include "CreateSessionCallbackProxyAdvanced.h"
#include "FindSessionsCallbackProxyAdvanced.h"
#include "FindSessionsCallbackProxy.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogPLGameInstance);

UPLGameInstance::UPLGameInstance()
{
	GameName = "PROJECT_LAUGH";
}

void UPLGameInstance::Init()
{
	Super::Init();

	//OLD CODE BEGIN

	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			//SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPLGameInstance::OnCreateSessionComplete);
			//SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPLGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPLGameInstance::OnJoinSessionComplete);
		}
	}

	// OLD CODE END
}

void UPLGameInstance::CreateServer(FString ServerName, FString HostName, int32 PlayerCount)
{
	UE_LOG(LogPLGameInstance, Log, TEXT("Creating Server"));

	// OLD CODE BEGINS
	
	//FOnlineSessionSettings SessionSettings;
	//SessionSettings.bAllowJoinInProgress = true;
	//SessionSettings.bIsDedicated = false;
	//SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") ? true : false;
	//SessionSettings.bShouldAdvertise = true;
	//SessionSettings.bUsesPresence = true;
	//SessionSettings.NumPublicConnections = 2;
	//SessionSettings.bUseLobbiesIfAvailable = true;

	////Custom Data
	//SessionSettings.Set(FName("SERVER_NAME_KEY"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	//SessionSettings.Set(FName("HOST_NAME_KEY"), HostName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	//SessionInterface->CreateSession(0, FName(MySessionName), SessionSettings);

	//OLD CODE ENDS

	TArray<FSessionPropertyKeyPair> ExtraSettings;
	ExtraSettings.Add(UAdvancedSessionsLibrary::MakeLiteralSessionPropertyString(FName("SERVER_NAME"), ServerName));
	ExtraSettings.Add(UAdvancedSessionsLibrary::MakeLiteralSessionPropertyString(FName("GAME_NAME"), GameName));

	UCreateSessionCallbackProxyAdvanced* Proxy = UCreateSessionCallbackProxyAdvanced::CreateAdvancedSession(GetWorld(), ExtraSettings, UGameplayStatics::GetPlayerController(GetWorld(), 0), PlayerCount + 1 ,0,false,true,false, true);
	Proxy->OnSuccess.AddDynamic(this,& UPLGameInstance::OnCreateSessionComplete);
	Proxy->OnFailure.AddDynamic(this, &UPLGameInstance::OnCreateSessionFail);
	Proxy->Activate();
}

void UPLGameInstance::FindSessions()
{
	SearchingForServerDelegate.Broadcast(true);

	// OLD CODE BEGINS

	/*SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
	SessionSearch->MaxSearchResults = 100000;
	SessionSearch->QuerySettings.Set("SEARCH_PRESENCE", true, EOnlineComparisonOp::Equals);

	UE_LOG(LogPLGameInstance, Log, TEXT("Finding Sessions"));
	
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());*/

	//OLD CODE ENDS

	TArray<FSessionsSearchSetting> SearchSettings;
	FSessionPropertyKeyPair const Pair = UAdvancedSessionsLibrary::MakeLiteralSessionPropertyString(FName("GAME_NAME"), GameName);

	SearchSettings.Add(FSessionsSearchSetting(EOnlineComparisonOpRedux::Equals, Pair));
	

	UFindSessionsCallbackProxyAdvanced* Proxy = UFindSessionsCallbackProxyAdvanced::FindSessionsAdvanced(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), 500 , false, EBPServerPresenceSearchType::ClientServersOnly, SearchSettings, false, false, false, true);
	Proxy->OnSuccess.AddDynamic(this, &UPLGameInstance::OnFindAdvancedSessionComplete);
	Proxy->OnFailure.AddDynamic(this, &UPLGameInstance::OnFindAdvancedSessionFail);
	Proxy->Activate();
}



void UPLGameInstance::OnCreateSessionComplete()
{
	UE_LOG(LogPLGameInstance, Log, TEXT("Session Succesfully Created"));

	GetWorld()->ServerTravel("/Game/Levels/Art_Test/NewMap?listen");	
	//UGameplayStatics::OpenLevel(GetWorld(), FName("GameLevel"), true, FString("listen"));
}

void UPLGameInstance::OnCreateSessionFail()
{
	UE_LOG(LogPLGameInstance, Log, TEXT("Session Creation Failed"));
}

void UPLGameInstance::OnFindAdvancedSessionComplete(const TArray<FBlueprintSessionResult>& Results)
{
	UE_LOG(LogPLGameInstance, Log, TEXT("Found %d Sessions"), Results.Num());

	SearchingForServerDelegate.Broadcast(false);

	
	//OLD CODE BEGIN

	/*if (bSucceeded)
	{
		UE_LOG(LogPLGameInstance, Log, TEXT("SearchResults Server count: %d"), SessionSearch->SearchResults.Num());

		int32 ArrayIndex = 0;

		for (FOnlineSessionSearchResult Result : SessionSearch->SearchResults)
		{
			if (!Result.IsValid())
				continue;
			FServerInfo Info;

			FString ServerName = "Empty Server Name";
			FString HostName = "Empty Host Name";

			Result.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), ServerName);
			Result.Session.SessionSettings.Get(FName("HOST_NAME_KEY"), HostName);

			Info.ServerName = ServerName;
			Info.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
			Info.CurrentPlayers = Info.MaxPlayers - Result.Session.NumOpenPublicConnections;
			Info.ServerArrayIndex = ArrayIndex;
			Info.SetPlayerCount();

			ServerListDelegate.Broadcast(Info);
			++ArrayIndex;
		}
	}*/

	//OLD CODE END

	if (Results.Num())
	{
		for (FBlueprintSessionResult SessionResult : Results)
		{
			if (SessionResult.OnlineResult.IsValid() == false)
			{
				UE_LOG(LogPLGameInstance, Log, TEXT("Session not valid, returning"))
				continue;
			}

			FServerInfo Info;
			FString ServerName = "Empty Server Name";

			TArray<FSessionPropertyKeyPair> ExtraSettings;
			UAdvancedSessionsLibrary::GetExtraSettings(SessionResult, ExtraSettings);

			ESessionSettingSearchResult SearchResult;
			UAdvancedSessionsLibrary::GetSessionPropertyString(ExtraSettings, FName("SERVER_NAME"), SearchResult, ServerName);

			Info.ServerName = ServerName;
			Info.MaxPlayers = SessionResult.OnlineResult.Session.SessionSettings.NumPublicConnections;
			Info.CurrentPlayers = Info.MaxPlayers - SessionResult.OnlineResult.Session.NumOpenPublicConnections;
			Info.OnlineResult = SessionResult.OnlineResult;

			UE_LOG(LogPLGameInstance, Log, TEXT("Updated Settings, calling brodcast"));

			ServerListDelegate.Broadcast(Info);
		}
	}	
	UE_LOG(LogPLGameInstance, Log, TEXT("Result is num, leaving"));
}

void UPLGameInstance::OnFindAdvancedSessionFail(const TArray<FBlueprintSessionResult>& Results)
{
	UE_LOG(LogPLGameInstance, Log, TEXT("Find Advanced Sessions Failed"));
}

void UPLGameInstance::JoinFoundSession(FOnlineSessionSearchResult& SearchResult)
{
	if (SearchResult.IsValid())
	{
		UE_LOG(LogPLGameInstance, Log, TEXT("Joining Server..."));
		SessionInterface->JoinSession(0, NAME_GameSession, SearchResult);
	}
	else
	{
		UE_LOG(LogPLGameInstance, Log, TEXT("Failed to join session"));
	}
}

void UPLGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	//UE_LOG(LogPLGameInstance, Log, TEXT("Join Session: %s"), *UEnum::GetValueAsString(Result))
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "")
		{
			PC->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
		}
	}
}