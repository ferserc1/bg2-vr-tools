// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawn.h"

#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerInput.h"
#include "HMDSettings.h"
#include "IXRTrackingSystem.h"
#include "Kismet/GameplayStatics.h"

static float THUMB_DIRECTION_BUTTON_OFFSET = 0.5f;

// Sets default values
AVRPawn::AVRPawn()
{
	float defaultColliderRadius = 10.0f;

	auto HMDRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HMDRoot"));
	RootComponent = HMDRoot;

	HMDCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("HMDCam"));
	HMDCamera->SetupAttachment(HMDRoot);

	LeftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController_L"));
	LeftMotionController->SetupAttachment(HMDRoot);
	LeftMotionController->MotionSource = "Left";
	LeftMotionController->DisplayModelSource = "SteamVR";
	LeftMotionController->bDisplayDeviceModel = true;

	LeftCollider = CreateDefaultSubobject<USphereComponent>(TEXT("LeftCollider"));
	LeftCollider->SetupAttachment(LeftMotionController);
	LeftCollider->InitSphereRadius(defaultColliderRadius);

	LeftInteraction = CreateDefaultSubobject<UInteractionComponent>(TEXT("LeftInteraction"));
	LeftInteraction->SetupAttachment(LeftMotionController);

	RightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController_R"));
	RightMotionController->SetupAttachment(HMDRoot);
	RightMotionController->MotionSource = "Right";
	RightMotionController->DisplayModelSource = "SteamVR";
	RightMotionController->bDisplayDeviceModel = true;

	RightCollider = CreateDefaultSubobject<USphereComponent>(TEXT("RightCollider"));
	RightCollider->SetupAttachment(RightMotionController);
	RightCollider->InitSphereRadius(defaultColliderRadius);

	RightInteraction = CreateDefaultSubobject<UInteractionComponent>(TEXT("RightInteraction"));
	RightInteraction->SetupAttachment(RightMotionController);
}

void AVRPawn::AddFloorCalibrationOffset(float offset)
{
	FVector pos = GEngine->XRSystem->GetBasePosition();
	pos.Z += offset;
	GEngine->XRSystem->SetBasePosition(pos);
	UHMDSettings::Save(GetWorld());
}

// Called when the game starts or when spawned
void AVRPawn::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	
	auto World = GetWorld();
	if (AddControlsMapping && World != nullptr && World->GetFirstPlayerController())
	{
		TArray<FInputActionKeyMapping> controllers = {
			// Right controller
			//{ "RightThumb", EKeys::MotionController_Right_Thumbstick, 0, 0, 0, 0 },
			{ "RightThumb", EKeys::Vive_Right_Trackpad_Click, 0, 0, 0, 0 },

			{ "RightThumbUp", EKeys::Vive_Right_Trackpad_Up, 0, 0, 0, 0 },
			{ "RightThumbDown", EKeys::Vive_Right_Trackpad_Down, 0, 0, 0, 0 },
			{ "RightThumbLeft", EKeys::Vive_Right_Trackpad_Left, 0, 0, 0, 0 },
			{ "RightThumbRight", EKeys::Vive_Right_Trackpad_Right, 0, 0, 0, 0 },
			{ "RightTrigger", EKeys::Vive_Right_Trigger_Click, 0, 0, 0, 0 },
			{ "RightGrip", EKeys::Vive_Right_Grip_Click, 0, 0, 0, 0 },

			// Left controller
			{ "LeftThumb", EKeys::Vive_Left_Trackpad_Click, 0, 0, 0, 0 },
			{ "LeftThumbUp", EKeys::Vive_Left_Trackpad_Up, 0, 0, 0, 0 },
			{ "LeftThumbDown", EKeys::Vive_Left_Trackpad_Down, 0, 0, 0, 0 },
			{ "LeftThumbLeft", EKeys::Vive_Left_Trackpad_Left, 0, 0, 0, 0 },
			{ "LeftThumbRight", EKeys::Vive_Left_Trackpad_Right, 0, 0, 0, 0 },
			{ "LeftTrigger", EKeys::Vive_Left_Trigger_Click, 0, 0, 0, 0 },
			{ "LeftGrip", EKeys::Vive_Left_Grip_Click, 0, 0, 0, 0 },
		};

		for (auto& inputKey : controllers)
		{
			World->GetFirstPlayerController()->PlayerInput->AddEngineDefinedActionMapping(inputKey);
		}

		TArray<FInputAxisKeyMapping> controllersAxis = {
			{ "RightThumbX", EKeys::ValveIndex_Right_Thumbstick_X, 1.0f },
			{ "RightThumbY", EKeys::ValveIndex_Right_Thumbstick_Y, -1.0f },
			{ "LeftThumbX", EKeys::ValveIndex_Left_Thumbstick_X, 1.0f },
			{ "LeftThumbY", EKeys::ValveIndex_Left_Thumbstick_Y, -1.0f }
		};

		for (auto& inputAxis : controllersAxis)
		{
			World->GetFirstPlayerController()->PlayerInput->AddAxisMapping(inputAxis);
		}
	}

	// TODO: Other HMD devices
	FName deviceName = UHeadMountedDisplayFunctionLibrary::GetHMDDeviceName();
	if (deviceName == "SteamVR") {
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	}
	else if (deviceName == "OculusHMD") {
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
	}

	// Restore saved HMD settings
	UHMDSettings::Restore(GetWorld());
}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Right actions
	PlayerInputComponent->BindAction("RightThumb", IE_Pressed, this, &AVRPawn::RightThumb_Press);
	PlayerInputComponent->BindAction("RightThumb", IE_Released, this, &AVRPawn::RightThumb_Release);
	PlayerInputComponent->BindAction("RightTrigger", IE_Pressed, this, &AVRPawn::RightTrigger_Press);
	PlayerInputComponent->BindAction("RightTrigger", IE_Released, this, &AVRPawn::RightTrigger_Release);
	PlayerInputComponent->BindAction("RightGrip", IE_Pressed, this, &AVRPawn::RightGrip_Press);
	PlayerInputComponent->BindAction("RightGrip", IE_Released, this, &AVRPawn::RightGrip_Release);

	// Right axis
	PlayerInputComponent->BindAxis("RightThumbX", this, &AVRPawn::RightThumbX);
	PlayerInputComponent->BindAxis("RightThumbY", this, &AVRPawn::RightThumbY);

	// Left actions
	PlayerInputComponent->BindAction("LeftThumb", IE_Pressed, this, &AVRPawn::LeftThumb_Press);
	PlayerInputComponent->BindAction("LeftThumb", IE_Released, this, &AVRPawn::LeftThumb_Release);
	PlayerInputComponent->BindAction("LeftTrigger", IE_Pressed, this, &AVRPawn::LeftTrigger_Press);
	PlayerInputComponent->BindAction("LeftTrigger", IE_Released, this, &AVRPawn::LeftTrigger_Release);
	PlayerInputComponent->BindAction("LeftGrip", IE_Pressed, this, &AVRPawn::LeftGrip_Press);
	PlayerInputComponent->BindAction("LeftGrip", IE_Released, this, &AVRPawn::LeftGrip_Release);

	// Left axis
	PlayerInputComponent->BindAxis("LeftThumbX", this, &AVRPawn::LeftThumbX);
	PlayerInputComponent->BindAxis("LeftThumbY", this, &AVRPawn::LeftThumbY);
}

void AVRPawn::RightThumb_Press()
{
	if (RightThumbAxis.X < -THUMB_DIRECTION_BUTTON_OFFSET)
	{
		RightThumbButton = TB_Left;
	}
	else if (RightThumbAxis.X > THUMB_DIRECTION_BUTTON_OFFSET)
	{
		RightThumbButton = TB_Right;
	}
	else if (RightThumbAxis.Y < -THUMB_DIRECTION_BUTTON_OFFSET)
	{
		RightThumbButton = TB_Down;
	}
	else if (RightThumbAxis.Y > THUMB_DIRECTION_BUTTON_OFFSET)
	{
		RightThumbButton = TB_Up;
	}
	else {
		RightThumbButton = TB_None;
	}

	if (RightThumbButton != TB_None)
	{
		RightThumbDirection_Press(RightThumbButton);
	}
	else
	{
		RightInteraction->BeginTeleport();
	}
}

void AVRPawn::RightThumb_Release()
{
	if (RightThumbButton != TB_None)
	{
		RightThumbDirection_Release(RightThumbButton);
	}
	else
	{
		RightInteraction->EndTeleport();
	}

	RightThumbButton = TB_None;
}

void AVRPawn::RightTrigger_Press()
{
	RightInteraction->BeginTouch();
}

void AVRPawn::RightTrigger_Release()
{
	RightInteraction->EndTouch();
}

void AVRPawn::RightGrip_Press()
{
	RightInteraction->BeginGrip();
}

void AVRPawn::RightGrip_Release()
{
	RightInteraction->EndGrip();
}

void AVRPawn::RightThumbX(float amount)
{
	RightThumbAxis.X = amount;
}

void AVRPawn::RightThumbY(float amount)
{
	RightThumbAxis.Y = amount;
}

void AVRPawn::RightThumbDirection_Press(ThumbDirectionButton direction)
{
	RightInteraction->DirectionButtonPress(direction);
}

void AVRPawn::RightThumbDirection_Release(ThumbDirectionButton direction)
{
	RightInteraction->DirectionButtonRelease(direction);
}

void AVRPawn::LeftThumb_Press()
{
	if (LeftThumbAxis.X < -THUMB_DIRECTION_BUTTON_OFFSET)
	{
		LeftThumbButton = TB_Left;
	}
	else if (LeftThumbAxis.X > THUMB_DIRECTION_BUTTON_OFFSET)
	{
		LeftThumbButton = TB_Right;
	}
	else if (LeftThumbAxis.Y < -THUMB_DIRECTION_BUTTON_OFFSET)
	{
		LeftThumbButton = TB_Down;
	}
	else if (LeftThumbAxis.Y > THUMB_DIRECTION_BUTTON_OFFSET)
	{
		LeftThumbButton = TB_Up;
	}
	else {
		LeftThumbButton = TB_None;
	}

	if (LeftThumbButton != TB_None)
	{
		LeftThumbDirection_Press(LeftThumbButton);
	}
	else
	{
		LeftInteraction->BeginTeleport();
	}
}

void AVRPawn::LeftThumb_Release()
{
	if (LeftThumbButton != TB_None)
	{
		LeftThumbDirection_Release(LeftThumbButton);
	}
	else
	{
		LeftInteraction->EndTeleport();
	}

	LeftThumbButton = TB_None;
}

void AVRPawn::LeftTrigger_Press()
{
	LeftInteraction->BeginTouch();
}

void AVRPawn::LeftTrigger_Release()
{
	LeftInteraction->EndTouch();
}

void AVRPawn::LeftGrip_Press()
{
	LeftInteraction->BeginGrip();
}

void AVRPawn::LeftGrip_Release()
{
	LeftInteraction->EndGrip();
}

void AVRPawn::LeftThumbX(float amount)
{
	LeftThumbAxis.X = amount;
}

void AVRPawn::LeftThumbY(float amount)
{
	LeftThumbAxis.Y = amount;
}

void AVRPawn::LeftThumbDirection_Press(ThumbDirectionButton direction)
{
	LeftInteraction->DirectionButtonPress(direction);
}

void AVRPawn::LeftThumbDirection_Release(ThumbDirectionButton direction)
{
	LeftInteraction->DirectionButtonRelease(direction);
}

