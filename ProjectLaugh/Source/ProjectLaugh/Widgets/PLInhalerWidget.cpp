// Fill out your copyright notice in the Description page of Project Settings.


#include "PLInhalerWidget.h"

#include "Components/ProgressBar.h"
#include "ProjectLaugh/Gameplay/PLInhalerComponent.h"

void UPLInhalerWidget::NativeConstruct()
{
	if (ensureAlwaysMsgf(PLInhalerComponent, TEXT("PL Inhaler component is invalid, did you set it during construction")))
	{
		PLInhalerComponent->OnInhalerValueChange.AddDynamic(this, &UPLInhalerWidget::OnInhalerValueChanged);
		PLInhalerComponent->OnLungValueChange.AddDynamic(this, &UPLInhalerWidget::OnLungValueChanged);
	}
}

void UPLInhalerWidget::SetupComponent(UPLActorComponent* PLActorComponent)
{
	PLInhalerComponent = Cast<UPLInhalerComponent>(PLActorComponent);
	checkf(PLInhalerComponent, TEXT("INPLInhalerComponent is invalid"));
}

void UPLInhalerWidget::OnInhalerValueChanged(const float Current, const float Max)
{
	UpdateProgressBar(InhalerProgressBar, Current, Max);
}

void UPLInhalerWidget::OnLungValueChanged(const float Current, const float Max)
{
	UpdateProgressBar(LungProgressBar, Current, Max);
}

void UPLInhalerWidget::UpdateProgressBar(UProgressBar* InProgressBar, const float InCurrent, const float InMax)
{
	InProgressBar->SetPercent((InCurrent / InMax));
	//GEngine->AddOnScreenDebugMessage(2, 0.2f, FColor::Purple, FString::Printf(TEXT("Updating %s to %f"), *(InProgressBar->GetFName().ToString()), (InCurrent / InMax)));
}
