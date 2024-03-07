// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Core/PLStaticMeshActor.h"
#include "ProjectLaugh/Gameplay/Interaction/PLInteractionInterface.h"
#include "PLThrowableBase.generated.h"

class UPLThrowableComponent;
class UPLInteractableComponent;
class UPLGameplayTagComponent;
class UNiagaraSystem;
class USoundCue;
class UPLStunData;

UCLASS()
class PROJECTLAUGH_API APLThrowableBase : public APLStaticMeshActor, public IPLInteractionInterface
{
	GENERATED_BODY()


protected:
	APLThrowableBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL")
	UPLInteractableComponent* InteractableComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL")
	UPLThrowableComponent* ThrowableComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL")
	UPLGameplayTagComponent* GameplayTagComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL")
	UPLStunData* StunData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL | Hit")
	TArray<UNiagaraSystem*> HitFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL | Hit")
	USoundCue* HitSound;

	UFUNCTION()
	virtual void OnActorHitWithObject(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SpawnHitFX(FVector ImpactPoint);

	UPROPERTY()
	AActor* PreviouslyHitActor;

	UFUNCTION()
	void OnProjectileStopped(const FHitResult& ImpactResult);

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Interact_Implementation(APLPlayerCharacter* InInstigator, UPLInteractionComponent* OtherInteractableComponent) override;
};
