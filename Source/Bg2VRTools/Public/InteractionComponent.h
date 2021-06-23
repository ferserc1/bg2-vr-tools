// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "InteractionMode.h"
#include "InteractionComponent.generated.h"

UENUM(BlueprintType)
enum ThumbDirectionButton {
	TB_None,
	TB_Up,
	TB_Down,
	TB_Left,
	TB_Right
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDownButtonPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpButtonPressed);

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
	TEnumAsByte<InteractionMode> Interaction;

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
	void DirectionButtonPress(ThumbDirectionButton Button);

	UFUNCTION(BlueprintCallable, Category = "VR")
	void DirectionButtonRelease(ThumbDirectionButton Button);

	UFUNCTION(BlueprintCallable, Category = "VR")
	FHitResult ParabolicLineTrace(int Steps, float TimeStep, float Speed);

	UFUNCTION(BlueprintCallable, Category = "VR")
	bool StraightLineTrace(float distance, FHitResult & result);

	UFUNCTION(BlueprintCallable, Category = "VR")
	void InteractWithHit(FHitResult Hit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
	UObject * FocusComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
	UObject * FocusObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
	FHitResult CurrentHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
	UWidgetComponent * FocusWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
	int ParabolicLineSteps = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
	float ParabolicTimeStep = 0.02f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
	float ParabolicLineSpeed = 800.0f;

	UPROPERTY(BlueprintAssignable, Category = "VR")
	FDownButtonPressed OnDownButtonPressed;

	UPROPERTY(BlueprintAssignable, Category = "VR")
	FUpButtonPressed OnUpButtonPressed;

private:
	void CheckInteraction(UObject* InteractionObject, FHitResult Hit);
};
