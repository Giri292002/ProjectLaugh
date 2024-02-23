// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "PLThrowableBase.generated.h"

class UPLThrowableComponent;

UCLASS()
class PROJECTLAUGH_API APLThrowableBase : public AStaticMeshActor
{
	GENERATED_BODY()

	APLThrowableBase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL")
	UPLThrowableComponent* ThrowableComponent;

	UFUNCTION()
	void OnActorHitWithObject(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
};
