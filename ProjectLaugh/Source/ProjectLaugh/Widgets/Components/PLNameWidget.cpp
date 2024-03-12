// Fill out your copyright notice in the Description page of Project Settings.


#include "PLNameWidget.h"

#include "Components/TextBlock.h"


void UPLNameWidget::SetName(FString InName)
{
	NameTextBlock->SetText(FText::FromString(InName));
}

void UPLNameWidget::FadeIn()
{
	PlayAnimation(FadeInAnimation);
}

void UPLNameWidget::FadeOut()
{
	PlayAnimation(FadeOutAnimation);
}
