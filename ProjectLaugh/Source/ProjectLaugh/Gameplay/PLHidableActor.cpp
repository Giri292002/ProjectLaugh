// Fill out your copyright notice in the Description page of Project Settings.


#include "PLHidableActor.h"

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BillboardComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"
#include "ProjectLaugh/Core/PLPlayerController.h"
#include "ProjectLaugh/Components/PLHideComponent.h"
#include "ProjectLaugh/Gameplay/Interaction/PLInteractableComponent.h"
#include "ProjectLaugh/Gameplay/PLGameplayTagComponent.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"

APLHidableActor::APLHidableActor()
{
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(FName(TEXT("Root")));
	SetRootComponent(RootSceneComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName(TEXT("Mesh")));
	SkeletalMeshComponent->SetupAttachment(RootSceneComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(FName(TEXT("Spring Arm")));
	SpringArmComponent->SetupAttachment(SkeletalMeshComponent);
	SpringArmComponent->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(FName(TEXT("Camera")));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	PLInteractableComponent = CreateDefaultSubobject<UPLInteractableComponent>(FName(TEXT("Interactable Component")));

	PLGameplayTagComponent = CreateDefaultSubobject<UPLGameplayTagComponent>(FName(TEXT("Gameplaytag Component")));

	HidingLocationMarker = CreateDefaultSubobject<USceneComponent>(FName(TEXT("Hiding Location Marker")));
	HidingLocationMarker->SetupAttachment(SkeletalMeshComponent);
	HidingLocationSprite = CreateDefaultSubobject<UBillboardComponent>(FName(TEXT("Hiding Location Sprite")));
	HidingLocationSprite->SetupAttachment(HidingLocationMarker);

	ExitLocationMarker = CreateDefaultSubobject<USceneComponent>(FName(TEXT("Exit Location Marker")));
	ExitLocationMarker->SetupAttachment(SkeletalMeshComponent);
	ExitLocationSprite = CreateDefaultSubobject<UBillboardComponent>(FName(TEXT("Exit Location Sprite")));
	ExitLocationSprite->SetupAttachment(ExitLocationMarker);

	AutoPossessAI = EAutoPossessAI::Disabled;
	AutoPossessPlayer = EAutoReceiveInput::Disabled;

	SkeletalMeshComponent->OnComponentHit.AddDynamic(this, &APLHidableActor::OnSkeletalMeshComponentHit);

	SetReplicates(true);
}

void APLHidableActor::Interact_Implementation(APLPlayerCharacter* InInstigator, UPLInteractionComponent* OtherInteractableComponent)
{
	if (!IsValid(InInstigator))
	{
		return;
	}
	if (GetGameplayTagComponent()->GetActiveGameplayTags().HasTag(SharedGameplayTags::TAG_Interactable_Hidable_Status_HasOccupant))
	{
		InInstigator->GetPLHideComponent()->Server_ToggleDoor(this);
		return;
	}
	InInstigator->GetPLHideComponent()->Net_StartHiding(this);
}

void APLHidableActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APLHidableActor::Look);
		EnhancedInputComponent->BindAction(StopHidingAction, ETriggerEvent::Triggered, this, &APLHidableActor::Net_StopHiding);
		EnhancedInputComponent->BindAction(OpenDoorAction, ETriggerEvent::Triggered, this, &APLHidableActor::ToggleDoor);
	}
}

void APLHidableActor::BeginPlay()
{
	Super::BeginPlay();

	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &APLHidableActor::HandleOnMontageEnded);
	}

	ReceiveControllerChangedDelegate.AddDynamic(this, &APLHidableActor::OnControllerChanged);
}

void APLHidableActor::SetOccupant(APLPlayerCharacter* InOccupant)
{
	OccupantCharacter = InOccupant;
}

void APLHidableActor::OnSkeletalMeshComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetGameplayTagComponent()->GetActiveGameplayTags().HasTag(SharedGameplayTags::TAG_Interactable_Hidable_Status_HasOccupant))
	{
		return;
	}

	int OutIndex;
	AcceptedCollisionBoneNames.Find(Hit.MyBoneName, OutIndex);
	if (OutIndex != INDEX_NONE)
	{
		if (APLPlayerCharacter* PLPlayerCharacter = Cast<APLPlayerCharacter>(OtherActor))
		{
			//Dont stun self
			if (PLPlayerCharacter == OccupantCharacter)
			{
				return;
			}
			check(PLStunData);
			PLPlayerCharacter->Server_StunCharacter(PLStunData);
		}
	}
}

void APLHidableActor::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APLHidableActor::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	APLPlayerController* PLPlayerController = Cast<APLPlayerController>(NewController);
	if (IsValid(PLPlayerController))
	{
		PLPlayerController->Client_AddPLWidget(PopupWidgetClass);
	}

	GetGameplayTagComponent()->GetActiveGameplayTags().AddTag(SharedGameplayTags::TAG_Interactable_Hidable_Status_HasOccupant);
}

void APLHidableActor::UnPossessed()
{
	Super::UnPossessed();

	GetGameplayTagComponent()->GetActiveGameplayTags().RemoveTag(SharedGameplayTags::TAG_Interactable_Hidable_Status_HasOccupant);
	SetOccupant(nullptr);
}

void APLHidableActor::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (IsValid(GetController()))
	{
		if (GetController()->IsLocalPlayerController())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Cast<APlayerController>(GetController())->GetLocalPlayer()))
			{
				Subsystem->ClearAllMappings();
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void APLHidableActor::Net_StopHiding_Implementation()
{
	Server_StopHiding();
}

void APLHidableActor::Server_StopHiding_Implementation()
{
	if (GetGameplayTagComponent()->GetActiveGameplayTags().HasTag(SharedGameplayTags::TAG_Interactable_Status_Close))
	{
		Server_OpenDoor();
	} 
	OccupantCharacter->GetPLHideComponent()->Net_StopHiding(this);
}

bool APLHidableActor::Server_StopHiding_Validate()
{
	return true;
}


void APLHidableActor::Server_OpenDoor_Implementation()
{
	//Open Doors

	//If door is already open ignore
	if (GetGameplayTagComponent()->GetActiveGameplayTags().HasTag(SharedGameplayTags::TAG_Interactable_Status_Open))
	{
		return;
	}

	PLGameplayTagComponent->GetActiveGameplayTags().RemoveTag(SharedGameplayTags::TAG_Interactable_Status_Close);
	PLGameplayTagComponent->GetActiveGameplayTags().AddTag(SharedGameplayTags::TAG_Interactable_Status_Open);
}

bool APLHidableActor::Server_OpenDoor_Validate()
{
	return true;
}

void APLHidableActor::Server_CloseDoor_Implementation()
{
	//Close Doors
	// 
	//If door is already closed ignore
	if (GetGameplayTagComponent()->GetActiveGameplayTags().HasTag(SharedGameplayTags::TAG_Interactable_Status_Close))
	{
		return;
	}
	PLGameplayTagComponent->GetActiveGameplayTags().RemoveTag(SharedGameplayTags::TAG_Interactable_Status_Open);
	PLGameplayTagComponent->GetActiveGameplayTags().AddTag(SharedGameplayTags::TAG_Interactable_Status_Close);
}

bool APLHidableActor::Server_CloseDoor_Validate()
{
	return true;
}

void APLHidableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APLHidableActor, OccupantCharacter);
}

void APLHidableActor::Multicast_PlayMontage_Implementation(UAnimMontage* InMontage)
{
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	if (!IsValid(AnimInstance))
	{
		return;
	}

	if (AnimInstance->IsAnyMontagePlaying())
	{
		AnimInstance->StopAllMontages(0.f);
	}

	AnimInstance->Montage_Play(InMontage);
}

void APLHidableActor::HandleOnMontageEnded(UAnimMontage* Montage, bool Interrupted)
{
	if (Interrupted)
	{
		return;
	}
}

void APLHidableActor::OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController)
{
	APLPlayerController* PLPlayerController = Cast<APLPlayerController>(OldController);
	if (IsValid(PLPlayerController))
	{
		PLPlayerController->Client_RemovePLWidget(PopupWidgetClass);
	}
}

void APLHidableActor::ToggleDoor()
{
	if (GetGameplayTagComponent()->GetActiveGameplayTags().HasTag(SharedGameplayTags::TAG_Interactable_Status_Close))
	{
		Server_OpenDoor();
		return;
	}

	if (GetGameplayTagComponent()->GetActiveGameplayTags().HasTag(SharedGameplayTags::TAG_Interactable_Status_Open))
	{
		Server_CloseDoor();
		return;
	}
}
