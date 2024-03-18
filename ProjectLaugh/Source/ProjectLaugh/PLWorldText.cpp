// Fill out your copyright notice in the Description page of Project Settings.


#include "PLWorldText.h"

#include "Components/TextRenderComponent.h"

// Sets default values
APLWorldText::APLWorldText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(true);

}

void APLWorldText::Multi_SetText_Implementation(const FText& InText)
{
	GetTextRender()->SetText(InText);
}

// Called when the game starts or when spawned
void APLWorldText::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APLWorldText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

