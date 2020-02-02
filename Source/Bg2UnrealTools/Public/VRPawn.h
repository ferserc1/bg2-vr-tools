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

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	void RightThumb_Press();
	void RightThumb_Release();
	void RightThumbUp_Press();
	void RightThumbUp_Release();
	void RightThumbDown_Press();
	void RightThumbDown_Release();
	void RightThumbLeft_Press();
	void RightThumbLeft_Release();
	void RightThumbRight_Press();
	void RightThumbRight_Release();
	void RightTrigger_Press();
	void RightTrigger_Release();
	void RightGrip_Press();
	void RightGrip_Release();
	void RightThumbX(float amount);
	void RightThumbY(float amount);

	void LeftThumb_Press();
	void LeftThumb_Release();
	void LeftThumbUp_Press();
	void LeftThumbUp_Release();
	void LeftThumbDown_Press();
	void LeftThumbDown_Release();
	void LeftThumbLeft_Press();
	void LeftThumbLeft_Release();
	void LeftThumbRight_Press();
	void LeftThumbRight_Release();
	void LeftTrigger_Press();
	void LeftTrigger_Release();
	void LeftGrip_Press();
	void LeftGrip_Release();
	void LeftThumbX(float amount);
	void LeftThumbY(float amount);
};
