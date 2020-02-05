// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InteractionComponent.h"
#include "VRPawn.generated.h"

UCLASS()
class BG2VRTOOLS_API AVRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRPawn();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VR")
	class UCameraComponent * HMDCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VR")
	class UMotionControllerComponent * LeftMotionController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VR")
	class USphereComponent * LeftCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VR")
	UInteractionComponent * LeftInteraction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VR")
	class UMotionControllerComponent * RightMotionController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VR")
	class USphereComponent * RightCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VR")
	UInteractionComponent * RightInteraction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
	bool AddControlsMapping = true;

protected:
	// Called when the game starts or when spawned
	virtual void PreInitializeComponents() override;

	FVector2D RightThumbAxis;
	FVector2D LeftThumbAxis;
	TEnumAsByte<ThumbDirectionButton> RightThumbButton;
	TEnumAsByte<ThumbDirectionButton> LeftThumbButton;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	void RightThumb_Press();
	void RightThumb_Release();
	void RightTrigger_Press();
	void RightTrigger_Release();
	void RightGrip_Press();
	void RightGrip_Release();
	void RightThumbX(float amount);
	void RightThumbY(float amount);
	void RightThumbDirection_Press(ThumbDirectionButton direction);
	void RightThumbDirection_Release(ThumbDirectionButton direction);

	void LeftThumb_Press();
	void LeftThumb_Release();
	void LeftTrigger_Press();
	void LeftTrigger_Release();
	void LeftGrip_Press();
	void LeftGrip_Release();
	void LeftThumbX(float amount);
	void LeftThumbY(float amount);
	void LeftThumbDirection_Press(ThumbDirectionButton direction);
	void LeftThumbDirection_Release(ThumbDirectionButton direction);
};
