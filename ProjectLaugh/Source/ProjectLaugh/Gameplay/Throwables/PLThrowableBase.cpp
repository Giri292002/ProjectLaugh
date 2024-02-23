// Fill out your copyright notice in the Description page of Project Settings.


#include "PLThrowableBase.h"

#include "ProjectLaugh/Gameplay/Throwables/PLThrowableComponent.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"

APLThrowableBase::APLThrowableBase()
{
	ThrowableComponent = CreateDefaultSubobject<UPLThrowableComponent>(FName(TEXT("PL Throwable Component")));
	bReplicates = true;
	bStaticMeshReplicateMovement = true;
	GetStaticMeshComponent()->SetSimulatePhysics(true);
	SetReplicateMovement(true);
}

void APLThrowableBase::OnActorHitWithObject( AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority())
	{
		if (OtherActor)
		{
			if (APLPlayerCharacter* PLPlayerCharacter = Cast<APLPlayerCharacter>(OtherActor))
			{
				PLPlayerCharacter->Server_StunCharacter();
			}
		}
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
