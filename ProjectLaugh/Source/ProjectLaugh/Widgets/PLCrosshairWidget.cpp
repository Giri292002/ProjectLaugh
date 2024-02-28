// Fill out your copyright notice in the Description page of Project Settings.


#include "PLCrosshairWidget.h"
#include "ProjectLaugh/Gameplay/Interaction/PLInteractionComponent.h"

void UPLCrosshairWidget::SetupComponent(UPLActorComponent* PLActorComponent)
{
	PLInteractionComponent = Cast<UPLInteractionComponent>(PLActorComponent);
	checkf(PLInteractionComponent, TEXT("PLInteractionComponent is invalid"));
	PLInteractionComponent->OnCanInteract.AddDynamic(this, &UPLCrosshairWidget::OnCanInteract);
}