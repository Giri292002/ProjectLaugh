// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "PLResultScreenPawn.generated.h"

UCLASS()
class PROJECTLAUGH_API APLResultScreenPawn : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

public:
	// Sets default values for this pawn's properties
	APLResultScreenPawn();
};
