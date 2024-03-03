// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Components/PLActorComponent.h"
#include "PLSkillCheckComponent.generated.h"

class UPLSkillcheckWidget;
class UPLSkillCheckData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillCheckPositionSignature, float, SkillCheckPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillCheckResultSignature, bool , SkillCheckResult);

UCLASS()
class PROJECTLAUGH_API UPLSkillCheckComponent : public UPLActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "PL | UI")
	TSubclassOf<UPLComponentWidgetBase> SkillcheckWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "PL | Data")
	UPLSkillCheckData* SkillCheckData;

	UPROPERTY()
	float SuccessRegionPosition;

	UPROPERTY()
	float CurrentSkillCheckPosition;

	UPROPERTY()
	uint8 bRunSkillCheck : 1;

	//1 = Forward, -1 = Reverse
	UPROPERTY()
	int32 SkillCheckDirection = 1;

public:
	UFUNCTION(BlueprintCallable)
	void StartSkillCheck();	

	UFUNCTION(BlueprintCallable)
	bool SkillCheck();

	UPROPERTY(BlueprintAssignable)
	FSkillCheckPositionSignature OnSkillCheckPositionUpdate;

	UPROPERTY(BlueprintAssignable)
	FSkillCheckResultSignature OnSkillCheckResultUpdate;

private:
	UFUNCTION()
	void SetupSuccessRegion();

	UFUNCTION()
	bool IsWithinSuccessRegion(const float CurrentPosition); 

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
