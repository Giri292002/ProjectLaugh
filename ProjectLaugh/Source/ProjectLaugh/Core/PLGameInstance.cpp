// Fill out your copyright notice in the Description page of Project Settings.


#include "PLGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogPLGameInstance);

UPLGameInstance::UPLGameInstance()
{
}

void UPLGameInstance::Init()
{
	Super::Init();

	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPLGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPLGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPLGameInstance::OnJoinSessionComplete);
		}
	}
}

void UPLGameInstance::CreateServer(FString ServerName, FString HostName)
{
	UE_LOG(LogPLGameInstance, Log, TEXT("Creating Server"));

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") ? true : false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 2;

	//Custom Data
	SessionSettings.Set(FName("SERVER_NAME_KEY"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(FName("HOST_NAME_KEY"), HostName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);


	SessionInterface->CreateSession(0, FName("Project Laugh Session"), SessionSettings);
}

void UPLGameInstance::FindSessions()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
	SessionSearch->MaxSearchResults = 100000;
	SessionSearch->QuerySettings.Set("SEARCH_PRESENCE", true, EOnlineComparisonOp::Equals);

	UE_LOG(LogPLGameInstance, Log, TEXT("Finding Sessions"));
	
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UPLGameInstance::OnCreateSessionComplete(FName SessionName, bool bSucceeded)
{
	UE_LOG(LogPLGameInstance, Log, TEXT("OnCreateSessionComplete, Suceeded: %d"), bSucceeded);

	if (bSucceeded)
	{
		GetWorld()->ServerTravel("/Game/ProjectLaugh/Maps/GameLevel?listen");
	}
}

void UPLGameInstance::OnFindSessionComplete(bool bSucceeded)
{
	UE_LOG(LogPLGameInstance, Log, TEXT("OnFindSessionComplete, Suceeded: %d"), bSucceeded);

	if (bSucceeded)
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
		UE_LOG(LogPLGameInstance, Log, TEXT("SearchResults Server count: %d"), SearchResults.Num());

		for (FOnlineSessionSearchResult Result : SearchResults)
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
			Info.SetPlayerCount();

			ServerListDelegate.Broadcast(Info);
		}

		/*if (SearchResults.Num())
		{
			SessionInterface->JoinSession(0, FName("Project Laugh Session"), SearchResults[0]);
		}*/
	}
}

void UPLGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
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