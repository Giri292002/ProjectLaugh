// All Rights belong to Backslap Studios 2024

#include "PLPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Curves/CurveFloat.h"
#include "ProjectLaugh/Core/PLPlayerController.h"
#include "ProjectLaugh/Core/PLPlayerState.h"
#include "ProjectLaugh/Components/PLScoreComponent.h"
#include "ProjectLaugh/Animation/PLAnimationData.h"
#include "ProjectLaugh/Gameplay/Skillcheck/PLSkillCheckComponent.h"
#include "ProjectLaugh/Gameplay/PLGameplayTagComponent.h"
#include "ProjectLaugh/Data/PLPlayerAttributesData.h"
#include "ProjectLaugh/Data/PLStunData.h"
#include "ProjectLaugh/Gameplay/Interaction/PLInteractionComponent.h"
#include "ProjectLaugh/Gameplay/Throwables/PLThrowComponent.h"
#include "Infection/PLGameState_Infection.h"
#include "Infection/PLGameMode_Infection.h"

// Sets default values
APLPlayerCharacter::APLPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PLInteractionComponent = CreateDefaultSubobject<UPLInteractionComponent>(FName(TEXT("Interaction Component")));
	PLThrowComponent = CreateDefaultSubobject<UPLThrowComponent>(FName(TEXT("Throw Component")));
	PLThrowComponent->SetupAttachment(GetMesh(), FName("Weapon_R"));
	PLGameplayTagComponent = CreateDefaultSubobject<UPLGameplayTagComponent>(FName(TEXT("PL Gameplaytag Component")));
	PLSkillCheckComponent = CreateDefaultSubobject<UPLSkillCheckComponent>(FName(TEXT("PL SkillCheck Component")));
}

// Called when the game starts or when spawned
void APLPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	checkf(PLAnimationData, TEXT("PL Animation Data is null"));
	checkf(PLPlayerAttributesData, TEXT("PLPlayerAttributesData is null"));

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

	if (IsValid(DefaultAnimationLayer))
	{
		GetMesh()->LinkAnimClassLayers(DefaultAnimationLayer);
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

void APLPlayerCharacter::Server_StunCharacter_Implementation(UPLStunData* StunData)
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

	GetWorld()->GetTimerManager().SetTimer(StunTimerHandle,this, &APLPlayerCharacter::Server_StopStunCharacter, StunData->StunDuration);
	Multicast_StunCharacter(StunData);
}

bool APLPlayerCharacter::Server_StunCharacter_Validate(UPLStunData* StunData)
{
	return true;
}

void APLPlayerCharacter::Multicast_StunCharacter_Implementation(UPLStunData* StunData)
{
	check(StunData);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), StunData->StunSound, GetActorLocation());
	FFXSystemSpawnParameters NiagaraSystemParameters;
	NiagaraSystemParameters.WorldContextObject = GetWorld();
	NiagaraSystemParameters.SystemTemplate = StunData->StunFX;
	NiagaraSystemParameters.AttachToComponent = GetMesh();
	NiagaraSystemParameters.LocationType = EAttachLocation::SnapToTarget; 
	NiagaraSystemParameters.AttachPointName = FName("StunFX");
	UNiagaraComponent* SpawnedStunFX = UNiagaraFunctionLibrary::SpawnSystemAttachedWithParams(NiagaraSystemParameters);
	SpawnedStunFX->SetVariableFloat(FName("StunLifetime"), StunData->StunDuration);
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
	PLThrowComponent->Net_StartThrow();
}


void APLPlayerCharacter::Net_OnPounced_Implementation()
{
	PLPlayerController->Client_RemoveComponentWidgets();
	PLPlayerController->UnPossess();
	PLPlayerController->SetViewTargetWithBlend(this);

	PLPlayerController->GetPlayerState<APLPlayerState>()->GetPLScoreComponent()->Server_AddScoreFromTimeSurvived();
	PLPlayerController->GetPlayerState<APLPlayerState>()->GetPLScoreComponent()->Server_AddScoreFromPositionSurvived();
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
	APLGameMode_Infection* InfectionGameMode = Cast<APLGameMode_Infection>(GetWorld()->GetAuthGameMode());
	InfectionGameMode->DestroyPLPlayerCharacter(this);

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

void APLPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
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

void APLPlayerCharacter::Server_PlayAnimation_Implementation(UAnimMontage* MontageToPlay, bool bJumpToSection, FName SectionName)
{
	Multicast_PlayAnimation(MontageToPlay, bJumpToSection, SectionName);
}

bool APLPlayerCharacter::Server_PlayAnimation_Validate(UAnimMontage* MontageToPlay, bool bJumpToSection, FName SectionName)
{
	return true;
}

void APLPlayerCharacter::Multicast_PlayAnimation_Implementation(UAnimMontage* MontageToPlay, bool bJumpToSection, FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->Montage_Play(MontageToPlay);
		if (bJumpToSection)
		{
			AnimInstance->Montage_JumpToSection(SectionName);
		}
	}
}

void APLPlayerCharacter::Server_StopAnimation_Implementation(UAnimMontage* MontageToStop)
{
	Multicast_StopAnimation(MontageToStop);
}

bool APLPlayerCharacter::Server_StopAnimation_Validate(UAnimMontage* MontageToStop)
{
	return true;
}

void APLPlayerCharacter::Multicast_StopAnimation_Implementation(UAnimMontage* MontageToStop)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		if (IsValid(MontageToStop))
		{
			if(AnimInstance->Montage_IsPlaying(MontageToStop))
			{
				AnimInstance->Montage_Stop(0.5f, MontageToStop);
			}
		}
		else
		{
			AnimInstance->StopAllMontages(0.5f);
		}
	}
}