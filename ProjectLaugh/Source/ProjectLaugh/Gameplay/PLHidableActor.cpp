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
#include "ProjectLaugh/Gameplay/Interaction/PLInteractableComponent.h"
#include "ProjectLaugh/Gameplay/PLGameplayTagComponent.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"

APLHidableActor::APLHidableActor()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName(TEXT("Mesh")));
	SetRootComponent(SkeletalMeshComponent);

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

	AutoPossessAI = EAutoPossessAI::Disabled;
	AutoPossessPlayer = EAutoReceiveInput::Disabled;

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
		Server_OpenDoor();
		return;
	}
	InInstigator->Net_StartHiding(this);
}

void APLHidableActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APLHidableActor::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APLHidableActor::Net_StopHiding);
	}
}

void APLHidableActor::BeginPlay()
{
	Super::BeginPlay();

	check(OpenMontage);
	check(CloseMontage);

	HidingLocationTransform = HidingLocationMarker->GetComponentTransform();

	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &APLHidableActor::HandleOnMontageEnded);
	}
}

void APLHidableActor::SetOccupant(APLPlayerCharacter* InOccupant)
{
	OccupantCharacter = InOccupant;
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

	GetGameplayTagComponent()->GetActiveGameplayTags().AddTag(SharedGameplayTags::TAG_Interactable_Hidable_Status_HasOccupant);
	Server_CloseDoor();
}

void APLHidableActor::UnPossessed()
{
	Super::UnPossessed();
	GetGameplayTagComponent()->GetActiveGameplayTags().RemoveTag(SharedGameplayTags::TAG_Interactable_Hidable_Status_HasOccupant);
}

void APLHidableActor::Server_StartHiding_Implementation(APLPlayerCharacter* InPLPlayerCharacter)
{

}

bool APLHidableActor::Server_StartHiding_Validate(APLPlayerCharacter* InPLPlayerCharacter)
{
	return true;
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
}

bool APLHidableActor::Server_StopHiding_Validate()
{
	return true;
}


void APLHidableActor::Server_OpenDoor_Implementation()
{
	//Open Doors
	PLGameplayTagComponent->GetActiveGameplayTags().RemoveTag(SharedGameplayTags::TAG_Interactable_Status_Close);
	PLGameplayTagComponent->GetActiveGameplayTags().AddTag(SharedGameplayTags::TAG_Interactable_Status_Open);
	Multicast_PlayMontage(OpenMontage);
}

bool APLHidableActor::Server_OpenDoor_Validate()
{
	return true;
}

void APLHidableActor::Server_CloseDoor_Implementation()
{
	//Close Doors
	PLGameplayTagComponent->GetActiveGameplayTags().RemoveTag(SharedGameplayTags::TAG_Interactable_Status_Open);
	PLGameplayTagComponent->GetActiveGameplayTags().AddTag(SharedGameplayTags::TAG_Interactable_Status_Close);
	Multicast_PlayMontage(CloseMontage);
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

	if (Montage == OpenMontage)
	{
	}
	if (Montage == CloseMontage)
	{
	}
}
