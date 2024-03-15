// Fill out your copyright notice in the Description page of Project Settings.


#include "PLActor.h"

// Sets default values
APLActor::APLActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APLActor::BeginPlay()
{
	Super::BeginPlay();

	BeginTransform = GetActorTransform();
}

// Called every frame
void APLActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APLActor::PLReset_Implementation()
{
	SetActorTransform(BeginTransform);
}

