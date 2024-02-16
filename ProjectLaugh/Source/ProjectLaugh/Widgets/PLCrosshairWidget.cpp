// Fill out your copyright notice in the Description page of Project Settings.


#include "PLCrosshairWidget.h"
#include "ProjectLaugh/Gameplay/Interaction/PLInteractionComponent.h"

void UPLCrosshairWidget::NativeConstruct()
{
	if (ensureAlwaysMsgf(PLInteractionComponent, TEXT("PLInteractionComponent is invalid. Make sure to set it when creating widget")))
	{
		PLInteractionComponent->OnCanInteract.AddDynamic(this,&UPLCrosshairWidget::OnCanInteract);
	}
}