// All Rights belong to Backslap Studios 2024

#include "PLPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ProjectLaugh/Core/PLPlayerController.h"
#include "ProjectLaugh/Data/PLPlayerAttributesData.h"
#include "ProjectLaugh/Data/PLStunData.h"
#include "ProjectLaugh/Gameplay/PLInhalerComponent.h"
#include "ProjectLaugh/Gameplay/Interaction/PLInteractionComponent.h"
#include "ProjectLaugh/Gameplay/Throwables/PLThrowComponent.h"

// Sets default values
APLPlayerCharacter::APLPlayerCharacter()
{
	PLInhalerComponent = CreateDefaultSubobject<UPLInhalerComponent>(FName(TEXT("Inhaler Component")));
	PLInteractionComponent = CreateDefaultSubobject<UPLInteractionComponent>(FName(TEXT("Interaction Component")));
	PLThrowComponent = CreateDefaultSubobject<UPLThrowComponent>(FName(TEXT("Throw Component")));
	PLThrowComponent->SetupAttachment(GetMesh(), FName("RightHand"));
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
}

void APLPlayerCharacter::Inhale(const FInputActionValue& Value)
{
	if (ensure(PLInhalerComponent))
	{
		const bool bIsInhaling = Value.Get<bool>();
		if (bIsInhaling)
		{
			PLInhalerComponent->Net_StartInhale();
		}
		else
		{
			PLInhalerComponent->Net_StopInhale();
		}
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
	GetWorld()->GetTimerManager().SetTimer(StunTimerHandle,this, &APLPlayerCharacter::Server_StopStunCharacter, PLStunData->StunDuration);
	Server_ToggleFreezeCharacter(true, true);
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
}

void APLPlayerCharacter::Server_StopStunCharacter_Implementation()
{
	Server_ToggleFreezeCharacter(false, true);
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

void APLPlayerCharacter::Net_ToggleFreezeCharacter_Implementation(const bool bFreeze, const bool bOverride)
{
	if (ensure(GetController()))
	{
		if (!bOverride && bFreeze != bFrozen)
		{
			return;
		}

		GetController()->SetIgnoreMoveInput(bFreeze);
		if (!HasAuthority())
		{
			Server_ToggleFreezeCharacter(bFreeze, bOverride);
			bFrozen = bFreeze;
		}
		
	}	
}

void APLPlayerCharacter::Server_ToggleFreezeCharacter_Implementation(const bool bFreeze, const bool bOverride)
{
	if (!bOverride && bFreeze != bFrozen)
	{
		return;
	}

	if (ensure(GetController()))
	{
		GetController()->SetIgnoreMoveInput(bFreeze);
		bFrozen = bFreeze;
	}
}

bool APLPlayerCharacter::Server_ToggleFreezeCharacter_Validate(const bool bFreeze, const bool bOverride)
{
	return true;
}



void APLPlayerCharacter::Net_ThrowObject_Implementation()
{
	PLThrowComponent->Net_Throw(PLPlayerController);
}

// Called every frame
void APLPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		PLInteractionComponent->RunInteractTrace(PLPlayerController);
	}
}

// Called to bind functionality to input
void APLPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(InhaleAction, ETriggerEvent::Triggered, this, &APLPlayerCharacter::Inhale);
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
}
