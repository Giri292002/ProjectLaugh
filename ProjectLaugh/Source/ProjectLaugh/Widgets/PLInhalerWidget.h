// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PLInhalerWidget.generated.h"

class UProgressBar;
class UPLInhalerComponent;

UCLASS()

class PROJECTLAUGH_API UPLInhalerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void SetPLInhalerComponent(UPLInhalerComponent* PLInhalerComponent);
	
protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UProgressBar* InhalerProgressBar;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UProgressBar* LungProgressBar;

	UPROPERTY()
	UPLInhalerComponent* PLInhalerComponent;

	UFUNCTION()
	void OnInhalerValueChanged(const float Current, const float Max);
	
	UFUNCTION()
	void OnLungValueChanged(const float Current, const float Max);

	void UpdateProgressBar(UProgressBar* ProgressBar, const float Current, const float Max);
};
