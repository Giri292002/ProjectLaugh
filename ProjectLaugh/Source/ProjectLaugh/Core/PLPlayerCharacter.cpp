// All Rights belong to Backslap Studios 2024

#include "PLPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Curves/CurveFloat.h"
#include "ProjectLaugh/Gameplay/PLGameplayTagComponent.h"
#include "ProjectLaugh/Core/PLPlayerController.h"
#include "ProjectLaugh/Data/PLPlayerAttributesData.h"
#include "ProjectLaugh/Data/PLStunData.h"
#include "ProjectLaugh/Gameplay/Interaction/PLInteractionComponent.h"
#include "ProjectLaugh/Gameplay/Throwables/PLThrowComponent.h"

// Sets default values
APLPlayerCharacter::APLPlayerCharacter()
{
	PLInteractionComponent = CreateDefaultSubobject<UPLInteractionComponent>(FName(TEXT("Interaction Component")));
	PLThrowComponent = CreateDefaultSubobject<UPLThrowComponent>(FName(TEXT("Throw Component")));
	PLThrowComponent->SetupAttachment(GetMesh(), FName("Weapon_R"));
	PLGameplayTagComponent = CreateDefaultSubobject<UPLGameplayTagComponent>(FName(TEXT("PL Gameplaytag Component")));
}

// Called when the game starts or when spawned
void APLPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!ensureAlwaysMsgf(PLPlayerAttributesData, TEXT("PLPlayerAttributesData is null")))
	{
		return;
	}

	Net_SetMaxWalkSpeed(PLPlayerAttributesData->MaxWalkSpeed);
	Net_SetPushForce(PLPlayerAttributesData->PushForce);
	PLGameplayTagComponent->Server_AddTag(PLPlayerAttributesData->AffiliationTag);

	if (IsValid(PLPlayerAttributesData->AppearanceCurve))
	{
		FOnTimelineFloat OnTimelineCallback;
		FOnTimelineEventStatic OnTimelineFinishedCallback;

		OnTimelineCallback.BindUFunction(this, FName(TEXT("AppearanceTimelineCallback")));
		OnTimelineFinishedCallback.BindUFunction(this, FName(TEXT("AppearanceTimelineFinishedCallback")));
		AppearanceTimeline.AddInterpFloat(PLPlayerAttributesData->AppearanceCurve, OnTimelineCallback);
		AppearanceTimeline.SetTimelineFinishedFunc(OnTimelineFinishedCallback);
	}
}

void APLPlayerCharacter::Server_SetMaxWalkSpeed_Implementation(const float InMaxWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = InMaxWalkSpeed;
}

bool APLPlayerCharacter::Server_SetMaxWalkSpeed_Validate(const float InMaxWalkSpeed)
{
	return true;
}

void APLPlayerCharacter::Net_SetMaxWalkSpeed_Implementation(const float InMaxWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = InMaxWalkSpeed;
	if (!HasAuthority())
	{
		Server_SetMaxWalkSpeed(InMaxWalkSpeed);
	}
}

void APLPlayerCharacter::Server_SetPushForce_Implementation(const float InPushForce)
{
	GetCharacterMovement()->InitialPushForceFactor = InPushForce;
}

bool APLPlayerCharacter::Server_SetPushForce_Validate(const float InPushForce)
{
	return true;
}

void APLPlayerCharacter::Net_SetPushForce_Implementation(const float InPushForce)
{
	GetCharacterMovement()->InitialPushForceFactor = InPushForce;
	if (!HasAuthority())
	{
		Server_SetPushForce(InPushForce);
	}
}

float APLPlayerCharacter::GetMaxWalkSpeed()
{
	return GetCharacterMovement()->MaxWalkSpeed;
}

void APLPlayerCharacter::Server_StunCharacter_Implementation()
{
	//Check Stunning conditions
	FGameplayTagContainer BlockedTags;
	BlockedTags.AddTag(SharedGameplayTags::TAG_Character_Status_Stunned);
	BlockedTags.AddTag(SharedGameplayTags::TAG_Character_Status_Spawning);
	if (PLGameplayTagComponent->GetActiveGameplayTags().HasAny(BlockedTags))
	{
		return;
	}

	Net_ToggleFreezeCharacter(true);

	PLGameplayTagComponent->Server_AddTag(SharedGameplayTags::TAG_Character_Status_Stunned); 

	GetWorld()->GetTimerManager().SetTimer(StunTimerHandle,this, &APLPlayerCharacter::Server_StopStunCharacter, PLStunData->StunDuration);
	Multicast_StunCharacter();
}

bool APLPlayerCharacter::Server_StunCharacter_Validate()
{
	return true;
}

void APLPlayerCharacter::Multicast_StunCharacter_Implementation()
{
	check(PLStunData);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PLStunData->StunSound, GetActorLocation());
	FFXSystemSpawnParameters NiagaraSystemParameters;
	NiagaraSystemParameters.WorldContextObject = GetWorld();
	NiagaraSystemParameters.SystemTemplate = PLStunData->HitFX;
	NiagaraSystemParameters.AttachToComponent = GetMesh();
	NiagaraSystemParameters.LocationType = EAttachLocation::SnapToTarget; 
	NiagaraSystemParameters.AttachPointName = FName("StunFX");
	UNiagaraComponent* SpawnedStunFX = UNiagaraFunctionLibrary::SpawnSystemAttachedWithParams(NiagaraSystemParameters);
	SpawnedStunFX->SetVariableFloat(FName("StunLifetime"), PLStunData->StunDuration);
}

void APLPlayerCharacter::Server_StopStunCharacter_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(StunTimerHandle);
	PLGameplayTagComponent->Server_RemoveTag(SharedGameplayTags::TAG_Character_Status_Stunned);
	Net_ToggleFreezeCharacter(false);
}

bool APLPlayerCharacter::Server_StopStunCharacter_Validate()
{
	return true;
}

void APLPlayerCharacter::Net_TryInteract_Implementation()
{
	PLInteractionComponent->TryInteract();
	if (!HasAuthority())
	{
		Server_TryInteract();
	}
}

void APLPlayerCharacter::Server_TryInteract_Implementation()
{
	PLInteractionComponent->TryInteract();
}

bool APLPlayerCharacter::Server_TryInteract_Validate()
{
	return true;
}

void APLPlayerCharacter::Net_ToggleFreezeCharacter_Implementation(const bool bFreeze)
{
	if (IsValid(GetController()))
	{
		GetController()->ResetIgnoreMoveInput();
		GetController()->SetIgnoreMoveInput(bFreeze);
		if (!HasAuthority())
		{
			Server_ToggleFreezeCharacter(bFreeze);
		}		
	}	
}

void APLPlayerCharacter::Server_ToggleFreezeCharacter_Implementation(const bool bFreeze)
{
	if (IsValid(GetController()))
	{
		GetController()->ResetIgnoreMoveInput();
		GetController()->SetIgnoreMoveInput(bFreeze);
	}
}

bool APLPlayerCharacter::Server_ToggleFreezeCharacter_Validate(const bool bFreeze)
{
	return true;
}

void APLPlayerCharacter::Net_ThrowObject_Implementation()
{
	PLThrowComponent->Net_Throw(PLPlayerController);
}


void APLPlayerCharacter::Net_OnPounced_Implementation()
{
	PLPlayerController->Client_RemoveComponentWidgets();
	PLPlayerController->UnPossess();
	PLPlayerController->SetViewTargetWithBlend(this);
}

void APLPlayerCharacter::Multicast_OnPounced_Implementation()
{
	GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetSimulatePhysics(true);
	PlayDisappearanceTimeline(PLPlayerAttributesData->DisappearanceTime);
}



void APLPlayerCharacter::Server_DisappearCharacter_Implementation()
{
	Multicast_DisappearCharacter();
}

bool APLPlayerCharacter::Server_DisappearCharacter_Validate()
{
	return true;
}

void APLPlayerCharacter::Multicast_DisappearCharacter_Implementation()
{
	PlayDisappearanceTimeline(PLPlayerAttributesData->DisappearanceTime);
}

void APLPlayerCharacter::Server_Destroy_Implementation()
{
	GEngine->AddOnScreenDebugMessage((uint64)("Destroy"), 5.0f, FColor::Purple, FString::Printf(TEXT("CALLED DESTROY")));
	Destroy();
}

bool APLPlayerCharacter::Server_Destroy_Validate()
{
	return true;
}

// Called every frame
void APLPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		PLInteractionComponent->RunInteractTrace(PLPlayerController);
	}
	AppearanceTimeline.TickTimeline(DeltaTime);
}

// Called to bind functionality to input
void APLPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APLPlayerCharacter::Net_TryInteract);
		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Triggered, this, &APLPlayerCharacter::Net_ThrowObject);
	}
}

void APLPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetCharacterMovement()->bPushForceScaledToMass = true;
}

void APLPlayerCharacter::Restart()
{
	Super::Restart();
}

void APLPlayerCharacter::PossessedBy(AController* Possessor)
{
	Super::PossessedBy(Possessor);
	PLPlayerController = Cast<APLPlayerController>(Possessor);
	OnClientControlPossess.Broadcast(GetController());
}

void APLPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APLPlayerCharacter, PLPlayerController);
	DOREPLIFETIME(APLPlayerCharacter, AppearanceTimeline);
}

void APLPlayerCharacter::PlayAppearanceTimeline(const float TimelinePlayLength)
{
	AppearanceTimeline.SetPlayRate(1.f / TimelinePlayLength);
	AppearanceTimeline.PlayFromStart();
}

void APLPlayerCharacter::PlayDisappearanceTimeline(const float TimelinePlayLength)
{
	AppearanceTimeline.SetPlayRate(1.f / TimelinePlayLength);
	AppearanceTimeline.ReverseFromEnd();
}

void APLPlayerCharacter::AppearanceTimelineCallback(float Value)
{
	GetMesh()->SetScalarParameterValueOnMaterials(FName("Appearance"), Value);
}

void APLPlayerCharacter::AppearanceTimelineFinishedCallback()
{
	// We reversed
	//GEngine->AddOnScreenDebugMessage((uint64)("Appearance"), 5.0f, FColor::Purple, FString::Printf(TEXT("Timeline finished at: %f"), AppearanceTimeline.GetPlaybackPosition()));
	if (AppearanceTimeline.GetPlaybackPosition() == 0.f)
	{
		Server_Destroy();
	}
}
