// Fill out your copyright notice in the Description page of Project Settings.


#include "PLEOSGameInstance.h"

#include "Interfaces/OnlineIdentityInterface.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"
#include "ProjectLaugh/Alphabets.h"

DEFINE_LOG_CATEGORY(LogPLEOS);

UPLEOSGameInstance::UPLEOSGameInstance()
{
	bIsLoggedIn = false;
	RoomCode = FString("");
}

void UPLEOSGameInstance::Init()
{
	Super::Init();

	OnlineSubsystem = IOnlineSubsystem::Get();
	Login();
}

void UPLEOSGameInstance::Shutdown()
{
	Super::Shutdown();

	DestroySession();
}

void UPLEOSGameInstance::Login()
{
	if (OnlineSubsystem)
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			//If with editor using DevAuth Credentials
//#if WITH_EDITOR
//			FOnlineAccountCredentials Credentials;
//			Credentials.Id = FString("127.0.0.1:8081");
//			Credentials.Token = FString("OldWarzGiriCred");
//			Credentials.Type = FString("developer");
//			UE_LOG(LogPLEOS, Log, TEXT("Running in editor, using Dev Auth Credentials"));
//#endif

			//If its a build, tell user to login
//#if !WITH_EDITOR
//#endif
			FOnlineAccountCredentials Credentials;
			Credentials.Id = FString();
			Credentials.Token = FString();
			Credentials.Type = FString("");
			UE_LOG(LogPLEOS, Log, TEXT("Running in build, using Account Portal"));
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
			RoomCode = GenerateRoomCode();
			MaxPlayersInCurrentLobby = MaxPlayers;

			TSharedRef<FOnlineSessionSettings>SessionSettings = MakeShared<FOnlineSessionSettings>();
			SessionSettings->bIsDedicated = false;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bIsLANMatch = false;
			SessionSettings->NumPublicConnections = MaxPlayers;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bUsesPresence = true;
			SessionSettings->bUseLobbiesIfAvailable = true;
			SessionSettings->bUseLobbiesVoiceChatIfAvailable = true;
			FOnlineSessionSetting RoomCodeSessionSetting;
			RoomCodeSessionSetting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
			RoomCodeSessionSetting.Data = RoomCode;
			//SessionSettings->Settings.Add(FName(TEXT("SessionSetting")), FOnlineSessionSetting(FString(TEXT("SettingValue")), EOnlineDataAdvertisementType::ViaOnlineService));
			SessionSettings->Settings.Add(FName(TEXT("ROOMCODE")), RoomCodeSessionSetting);

			SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UPLEOSGameInstance::OnCreateSessionComplete);
			SessionPtr->CreateSession(0, PLSessionName, *SessionSettings);
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
		FString Map = "/Game/ProjectLaugh/Maps/GameJamElderHouse/ElderHouse?listen";
		FURL TravelURL;
		TravelURL.Map = Map;
		GetWorld()->ServerTravel(Map);
	}
}

void UPLEOSGameInstance::FindSession(FString InRoomCode)
{
	if (!bIsLoggedIn)
	{
		UE_LOG(LogPLEOS, Error, TEXT("Cannot Find Session. User is not logged in"));
		return;
	}
	if (ensure(OnlineSubsystem))
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->QuerySettings.SearchParams.Empty();

			UE_LOG(LogPLEOS, Log, TEXT("Searching for room code %s"), *InRoomCode);
			SessionSearch->bIsLanQuery = false;
			SessionSearch->MaxSearchResults = 10000;
			SessionSearch->QuerySettings.Set(FName("ROOMCODE"), InRoomCode, EOnlineComparisonOp::Equals);
			SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Type::Equals);

			SessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &UPLEOSGameInstance::OnFindSessionComplete);
			SessionPtr->FindSessions(0, SessionSearch.ToSharedRef());
		}
	}
}


void UPLEOSGameInstance::OnFindSessionComplete(bool bWasSuccesful)
{
	UE_LOG(LogPLEOS, Log, TEXT("Find Session: %d with %i sessions"), bWasSuccesful, SessionSearch->SearchResults.Num());

	if (bWasSuccesful)
	{
		if (ensure(OnlineSubsystem))
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				SessionPtr->ClearOnFindSessionsCompleteDelegates(this);

				if (SessionSearch->SearchResults.IsValidIndex(0))
				{
					if (SessionPtr->GetResolvedConnectString(SessionSearch->SearchResults[0], NAME_GamePort, ConnectString))
					{
						SessionToJoin = &SessionSearch->SearchResults[0];
					}
					UE_LOG(LogPLEOS, Warning, TEXT("Resolved String: %s"), *ConnectString);
					JoinEOSSession();
				}
				else
				{
					UE_LOG(LogPLEOS, Error, TEXT("First found session is invalid"));
				}
			}
		}
	}
}

void UPLEOSGameInstance::JoinEOSSession()
{
	if (ensure(OnlineSubsystem))
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &UPLEOSGameInstance::OnJoinSessionCompleteCallback);
			if (!SessionPtr->JoinSession(0, PLSessionName, *SessionToJoin))
			{
				UE_LOG(LogPLEOS, Error, TEXT("Join Lobby failed"));
			}
		}
	}
}

void UPLEOSGameInstance::OnJoinSessionCompleteCallback(FName InSessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (ensureAlwaysMsgf(PlayerController, TEXT("Player Controller is invalid")))
		{
			PlayerController->ClientTravel(ConnectString, TRAVEL_Absolute);
		}
	}
}

void UPLEOSGameInstance::DestroySession()
{
	if (!bIsLoggedIn)
	{
		UE_LOG(LogPLEOS, Error, TEXT("Cannot Destroy Session. User is not logged in"));
		return;
	}
	if (ensure(OnlineSubsystem))
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			SessionPtr->OnDestroySessionCompleteDelegates.AddUObject(this, &UPLEOSGameInstance::OnDestroySessionComplete);
			SessionPtr->DestroySession(PLSessionName);
		}
	}
}

void UPLEOSGameInstance::OnDestroySessionComplete(FName InSessionName, bool bWasSuccessful)
{
	if (ensure(OnlineSubsystem))
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			RoomCode = FString("");
			SessionPtr->ClearOnRegisterPlayersCompleteDelegates(this);
		}
	}
}

FString UPLEOSGameInstance::GenerateRoomCode()
{
	const int RoomCodeLength = 5;
	FString GeneratedRoomCode = FString("");
	for (int i = 0; i < RoomCodeLength; ++i)
	{
		GeneratedRoomCode = GeneratedRoomCode + (Alphabets::SupportedAlphabets[FMath::Rand() % 26]);
	}
	UE_LOG(LogPLEOS, Log, TEXT("Generated Room Code: %s"), *GeneratedRoomCode);

	return GeneratedRoomCode;
}


