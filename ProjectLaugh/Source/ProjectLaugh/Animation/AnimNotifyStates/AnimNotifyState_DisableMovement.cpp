// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_DisableMovement.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimNotifyState_DisableMovement::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!IsValid(Character))
	{
		return;
	}
	Character->GetCharacterMovement()->DisableMovement();
}

void UAnimNotifyState_DisableMovement::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!IsValid(Character))
	{
		return;
	}
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}
