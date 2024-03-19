// Fill out your copyright notice in the Description page of Project Settings.


#include "PLHidableActor.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"

APLHidableActor::APLHidableActor()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(FName(TEXT("Camera")));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(FName(TEXT("Spring Arm")));
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName(TEXT("Mesh")));
}
