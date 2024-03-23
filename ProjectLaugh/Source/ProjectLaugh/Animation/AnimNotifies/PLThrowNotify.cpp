// Fill out your copyright notice in the Description page of Project Settings.


#include "PLThrowNotify.h"

#include "ProjectLaugh/Core/PLPlayerCharacter.h"
#include "ProjectLaugh/Core/PLPlayerController.h"
#include "ProjectLaugh/Gameplay/Throwables/PLThrowComponent.h"

void UPLThrowNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	const APLPlayerCharacter* PLPlayerCharacter = Cast<APLPlayerCharacter>(MeshComp->GetOwner());
	if (!IsValid(PLPlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("PL Player Character is invalid"));
		return;
	}

	if (PLPlayerCharacter->IsLocallyControlled())
	{
		PLPlayerCharacter->GetThrowComponent()->Net_Throw(Cast<APLPlayerController>(PLPlayerCharacter->GetController()));
	}
}
