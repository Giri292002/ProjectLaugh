// Fill out your copyright notice in the Description page of Project Settings.


#include "PLResultScreenPawn.h"

// Sets default values
APLResultScreenPawn::APLResultScreenPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(FName(TEXT("Camera Component")));
}

