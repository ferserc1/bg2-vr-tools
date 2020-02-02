// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "ObjectInteractionInterface.h"
#include "InteractionComponent.generated.h"

UENUM(BlueprintType)
enum InteracionMode
{
	IM_None = 0,
	IM_Teleport,
	IM_Touch,
	IM_Grip
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BG2VRTOOLS_API UInteractionComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
	TEnumAsByte<InteracionMode> Interaction;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "VR")
	void BeginTeleport();

	UFUNCTION(BlueprintCallable, Category = "VR")
	void EndTeleport();

	UFUNCTION(BlueprintCallable, Category = "VR")
	void BeginTouch();

	UFUNCTION(BlueprintCallable, Category = "VR")
	void EndTouch();

	UFUNCTION(BlueprintCallable, Category = "VR")
	void BeginGrip();

	UFUNCTION(BlueprintCallable, Category = "VR")
	void EndGrip();

	UFUNCTION(BlueprintCallable, Category = "VR")
	FHitResult ParabolicLineTrace(int Steps, float TimeStep, float Speed);

	UFUNCTION(BlueprintCallable, Category = "VR")
	void InteractWithHit(FHitResult Hit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
	UPrimitiveComponent * FocusComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
	TScriptInterface<IObjectInteractionInterface> FocusObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
	FHitResult CurrentHit;
};
