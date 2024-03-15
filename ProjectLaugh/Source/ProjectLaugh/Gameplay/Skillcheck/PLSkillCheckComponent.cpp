// Fill out your copyright notice in the Description page of Project Settings.


#include "PLSkillCheckComponent.h"

#include "ProjectLaugh/Core/PLPlayerCharacter.h"
#include "ProjectLaugh/Core/PLPlayerController.h"
#include "ProjectLaugh/Data/PLSkillCheckData.h"
#include "ProjectLaugh/Widgets/PLComponentWidgetBase.h"
#include "ProjectLaugh/Widgets/Components/PLSkillcheckWidget.h"

void UPLSkillCheckComponent::StartSkillCheck()
{
	APLPlayerCharacter* PLPlayerCharacter = Cast<APLPlayerCharacter>(GetOwner());
	checkf(PLPlayerCharacter, TEXT("PL Player character is invalid"));
	APLPlayerController* PLPlayerController = Cast<APLPlayerController>(PLPlayerCharacter->GetController());
	PLPlayerController->Client_AddComponentWidget(SkillcheckWidgetClass, this);
	SetupSuccessRegion();
	bRunSkillCheck = true;
}

bool UPLSkillCheckComponent::SkillCheck()
{
	float SkillCheckPosition = CurrentSkillCheckPosition;
	bRunSkillCheck = false;
	const bool Result = IsWithinSuccessRegion(SkillCheckPosition);
	OnSkillCheckResultUpdate.Broadcast(Result);
	return Result;
}

void UPLSkillCheckComponent::SetupSuccessRegion()
{
	checkf(SkillCheckData, TEXT("Skillcheck data is invalid"));
	SuccessRegionPosition = FMath::FRandRange(0.1f, 1.f);
	UPLSkillcheckWidget* SkillCheckWidget = Cast<UPLSkillcheckWidget>(SpawnedComponentWidget);
	SkillCheckWidget->SetupSuccessRegion(SuccessRegionPosition, SkillCheckData->SkillCheckSuccessRegionSize);
}

bool UPLSkillCheckComponent::IsWithinSuccessRegion(const float CurrentPosition)
{
	const float MinRegion = SuccessRegionPosition - (SkillCheckData->SkillCheckSuccessRegionSize / 2);
	const float MaxRegion = SuccessRegionPosition + (SkillCheckData->SkillCheckSuccessRegionSize / 2);
	return FMath::IsWithinInclusive(CurrentPosition, MinRegion, MaxRegion);
}

void UPLSkillCheckComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bRunSkillCheck)
	{

		CurrentSkillCheckPosition = FMath::Clamp(CurrentSkillCheckPosition += DeltaTime * SkillCheckData->SkillCheckSpeed * SkillCheckDirection, 0.f, 1.f);

		if (CurrentSkillCheckPosition >= 1.f)
		{
			SkillCheckDirection = -1;
		}
		else if (CurrentSkillCheckPosition <= 0.f)
		{
			SkillCheckDirection = 1;
		}

		OnSkillCheckPositionUpdate.Broadcast(CurrentSkillCheckPosition);
	}
}