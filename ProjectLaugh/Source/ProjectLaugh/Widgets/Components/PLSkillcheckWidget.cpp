// Fill out your copyright notice in the Description page of Project Settings.


#include "PLSkillcheckWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "ProjectLaugh/Gameplay/Skillcheck/PLSkillCheckComponent.h"

void UPLSkillcheckWidget::SetupComponent(UPLActorComponent* PLActorComponent)
{
	SkillCheckMaterialInstance = SkillCheckArea->GetDynamicMaterial();
	SkillCheckComponent = Cast<UPLSkillCheckComponent>(PLActorComponent);
	checkf(SkillCheckComponent, TEXT("SkillCheck Component is invalid"));

	//Bind Functions
	SkillCheckComponent->OnSkillCheckPositionUpdate.AddDynamic(this, &UPLSkillcheckWidget::OnSkillCheckPositionUpdated);
	SkillCheckComponent->OnSkillCheckResultUpdate.AddDynamic(this, &UPLSkillcheckWidget::OnSkillCheckResultUpdated);

	SkillCheckResultBox->SetVisibility(ESlateVisibility::Hidden); 
}

void UPLSkillcheckWidget::SetupSuccessRegion(const float SuccessRegionPosition, const float SuccessRegionSize)
{
	//GEngine->AddOnScreenDebugMessage((uint64)("SkillcheckWidget"), 50.f, FColor::Green, FString::Printf(TEXT("Position: %f, Size: %f"), SuccessRegionPosition, SuccessRegionSize));
	
	SkillCheckMaterialInstance->SetScalarParameterValue(FName("SuccessRegionPosition"), SuccessRegionPosition);
	SkillCheckMaterialInstance->SetScalarParameterValue(FName("SuccessRegionSize"), SuccessRegionSize);
}

void UPLSkillcheckWidget::OnSkillCheckPositionUpdated(float SkillCheckPosition)
{
	SkillCheckMaterialInstance->SetScalarParameterValue(FName("SkillcheckPosition"), SkillCheckPosition);
}

void UPLSkillcheckWidget::OnSkillCheckResultAnimationFinished()
{
	RemoveFromParent();
}

void UPLSkillcheckWidget::OnSkillCheckResultUpdated(bool Result)
{
	FWidgetAnimationDynamicEvent WidgetAnimationEvent;
	WidgetAnimationEvent.BindUFunction(this, FName(TEXT("OnSkillCheckResultAnimationFinished")));
	BindToAnimationFinished(SkillCheckResultAnimation, WidgetAnimationEvent);
	SkillCheckResultBox->SetColorAndOpacity(Result ? FColor::Green : FColor::Red);
	SkillCheckResultBox->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(SkillCheckResultAnimation);
}
