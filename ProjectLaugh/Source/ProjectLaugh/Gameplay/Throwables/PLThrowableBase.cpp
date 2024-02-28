// Fill out your copyright notice in the Description page of Project Settings.


#include "PLThrowableBase.h"

#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"
#include "ProjectLaugh/Gameplay/PLGameplayTagComponent.h"
#include "ProjectLaugh/Gameplay/Throwables/PLThrowComponent.h" 
#include "ProjectLaugh/Gameplay/Throwables/PLThrowableComponent.h"
#include "ProjectLaugh/Gameplay/Interaction/PLInteractableComponent.h"
#include "Sound/SoundCue.h"

APLThrowableBase::APLThrowableBase()
{
	ThrowableComponent = CreateDefaultSubobject<UPLThrowableComponent>(FName(TEXT("PL Throwable Component")));
	InteractableComponent = CreateDefaultSubobject<UPLInteractableComponent>(FName(TEXT("PL Interactable Component")));
	GameplayTagComponent = CreateDefaultSubobject<UPLGameplayTagComponent>(FName(TEXT("Gameplay Tag Component")));

	bReplicates = true;
	bStaticMeshReplicateMovement = true;
	GetStaticMeshComponent()->SetSimulatePhysics(true);
	SetReplicateMovement(true);
	ThrowableComponent->OnProjectileStop.AddDynamic(this, &APLThrowableBase::OnProjectileStopped);
}

void APLThrowableBase::OnActorHitWithObject( AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority() && GameplayTagComponent->GetActiveGameplayTags().HasTag(SharedGameplayTags::TAG_Ability_Throw_Thrown))
	{
		if (OtherActor)
		{
			if (APLPlayerCharacter* PLPlayerCharacter = Cast<APLPlayerCharacter>(OtherActor))
			{
				PLPlayerCharacter->Server_StunCharacter();
			}
			if (OtherActor != PreviouslyHitActor)
			{
				Multicast_SpawnHitFX(Hit.ImpactPoint);
				PreviouslyHitActor = OtherActor;
			}
		}
		
	}
}

void APLThrowableBase::Multicast_SpawnHitFX_Implementation(FVector ImpactPoint)
{
	const int RandomIndex = FMath::RandRange(0, HitFX.Num() - 1);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitFX[RandomIndex], ImpactPoint);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, ImpactPoint);
}

void APLThrowableBase::OnProjectileStopped(const FHitResult& ImpactResult)
{
	if (HasAuthority())
	{
		GameplayTagComponent->Server_RemoveTag(SharedGameplayTags::TAG_Ability_Throw_Thrown);
		PreviouslyHitActor = nullptr;
	}
}

void APLThrowableBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GetStaticMeshComponent()->SetSimulatePhysics(true);
}

void APLThrowableBase::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &APLThrowableBase::OnActorHitWithObject);
}

void APLThrowableBase::Interact_Implementation(APLPlayerCharacter* InInstigator, UPLInteractionComponent* OtherInteractableComponent)
{
	UPLThrowComponent* ThrowComponent = InInstigator->FindComponentByClass<UPLThrowComponent>();
	if (!IsValid(ThrowComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Throw component is invalid on %s"), *GetNameSafe(InInstigator));
		return;
	}
	ThrowComponent->Net_HoldObject(this);
}