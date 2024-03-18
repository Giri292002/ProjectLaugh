// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextRenderActor.h"
#include "ProjectLaugh/SharedGameplayTags.h"
#include "PLWorldText.generated.h"

class UNewTextRenderComponent;

UCLASS()
class PROJECTLAUGH_API APLWorldText : public ATextRenderActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APLWorldText();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_SetText(const FText& InText);

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetTextTag() const { return TextTag; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag TextTag; 

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
