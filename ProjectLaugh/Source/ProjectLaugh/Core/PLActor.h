// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectLaugh/Core/System/PLResetInterface.h"
#include "PLActor.generated.h"

UCLASS()
class PROJECTLAUGH_API APLActor : public AActor, public IPLResetInterface 
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APLActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// --- IPLResetInterface begin ---
	virtual void PLReset_Implementation() override;
	// --- IPLResetInterface end ---

private:
	UPROPERTY()
	FTransform BeginTransform;
};
