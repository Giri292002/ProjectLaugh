// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "ProjectLaugh/Core/System/PLResetInterface.h"
#include "PLStaticMeshActor.generated.h"

/**
 * Replicated static mesh actor, inherit from this to use for physics objects in level
 */
UCLASS()
class PROJECTLAUGH_API APLStaticMeshActor : public AStaticMeshActor, public IPLResetInterface
{
	GENERATED_BODY()

	APLStaticMeshActor();

	FTransform StartingTransform;

	virtual void BeginPlay() override;

	// --- IPLResetInterface begin ---
	virtual void PLReset_Implementation() override;
	// --- IPLResetInterface end ---
};
