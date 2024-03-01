// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Sound/SoundCue.h"
#include "NiagaraSystem.h"
#include "PLStunData.generated.h"

UCLASS()
class PROJECTLAUGH_API UPLStunData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundCue* StunSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float StunDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UNiagaraSystem* StunFX;
	
};
