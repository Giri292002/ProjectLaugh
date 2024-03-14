// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectLaugh/Widgets/PLComponentWidgetBase.h"
#include "PLActorComponent.generated.h"

class UPLComponentWidgetBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTLAUGH_API UPLActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPLActorComponent();

	UFUNCTION(BlueprintCallable, Category = "PL | UI")
	TSubclassOf<UPLComponentWidgetBase> GetComponentWidgetClass() const { return ComponentWidgetClass; }

	UFUNCTION(BlueprintCallable, Category = "PL | UI")
	void SetSpawnedComponentWidget(UPLComponentWidgetBase* InSpawnedComponentWidget) { SpawnedComponentWidget = InSpawnedComponentWidget; }

	UFUNCTION(BlueprintCallable, Category = "PL | UI")
	UPLComponentWidgetBase* GetSpawnedComponentWidget() const { return SpawnedComponentWidget; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "PL | UI")
	TSubclassOf<UPLComponentWidgetBase> ComponentWidgetClass;

	UPROPERTY()
	UPLComponentWidgetBase* SpawnedComponentWidget;

	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
