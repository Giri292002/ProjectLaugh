// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Widgets/PLComponentWidgetBase.h"
#include "PLSkillcheckWidget.generated.h"

class UBorder;
class UImage; 
class UPLSkillCheckComponent;

UCLASS()
class PROJECTLAUGH_API UPLSkillcheckWidget : public UPLComponentWidgetBase
{
	GENERATED_BODY()

	virtual void SetupComponent(UPLActorComponent* PLActorComponent) override;	

protected:
	UPROPERTY(Transient, EditDefaultsOnly, Category = "PL | UI", meta = (BindWidget))
	UBorder* SkillCheckArea;

	UPROPERTY(Transient, EditDefaultsOnly, Category = "PL | UI", meta = (BindWidgetAnim)) 
	UWidgetAnimation* SkillCheckResultAnimation;

	UPROPERTY(Transient, EditDefaultsOnly, Category = "PL | UI", meta = (BindWidget))
	UImage* SkillCheckResultBox;

	UFUNCTION()
	void OnSkillCheckResultUpdated(bool Result);

public:
	UFUNCTION(BlueprintCallable)
	void SetupSuccessRegion(const float SuccessRegionPosition, const float SuccessRegionSize);

	UFUNCTION()
	void OnSkillCheckResultAnimationFinished();

private:
	UPROPERTY()
	UMaterialInstanceDynamic* SkillCheckMaterialInstance;

	UPROPERTY()
	UPLSkillCheckComponent* SkillCheckComponent;

	UFUNCTION()
	void OnSkillCheckPositionUpdated(float SkillCheckPosition);
};
