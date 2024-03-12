// Fill out your copyright notice in the Description page of Project Settings.


#include "PLStaticMeshActor.h"

APLStaticMeshActor::APLStaticMeshActor()
{
	SetReplicates(true);
	SetReplicateMovement(true);
	GetStaticMeshComponent()->SetSimulatePhysics(true);
	bStaticMeshReplicateMovement = true;
}

void APLStaticMeshActor::BeginPlay()
{
	Super::BeginPlay();
	StartingTransform = GetActorTransform();
}

void APLStaticMeshActor::PLReset_Implementation()
{
	SetActorTransform(StartingTransform);
}
