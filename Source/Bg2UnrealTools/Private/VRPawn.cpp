// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawn.h"

#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerInput.h"

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

// Called when the game starts or when spawned
void AVRPawn::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	
	auto World = GetWorld();
	if (AddControlsMapping && World != nullptr && World->GetFirstPlayerController())
	{
		TArray<FInputActionKeyMapping> controllers = {
			// Right controller
			{ "RightThumb", EKeys::MotionController_Right_Thumbstick, 0, 0, 0, 0 },
			{ "RightThumbUp", EKeys::MotionController_Right_Thumbstick_Up, 0, 0, 0, 0 },
			{ "RightThumbDown", EKeys::MotionController_Right_Thumbstick_Down, 0, 0, 0, 0 },
			{ "RightThumbLeft", EKeys::MotionController_Right_Thumbstick_Left, 0, 0, 0, 0 },
			{ "RightThumbRight", EKeys::MotionController_Right_Thumbstick_Right, 0, 0, 0, 0 },
			{ "RightTrigger", EKeys::MotionController_Right_Trigger, 0, 0, 0, 0 },
			{ "RightGrip", EKeys::MotionController_Right_Grip1, 0, 0, 0, 0 },
			{ "RightGrip", EKeys::MotionController_Right_Grip2, 0, 0, 0, 0 },

			// Left controller
			{ "LeftThumb", EKeys::MotionController_Left_Thumbstick, 0, 0, 0, 0 },
			{ "LeftThumbUp", EKeys::MotionController_Left_Thumbstick_Up, 0, 0, 0, 0 },
			{ "LeftThumbDown", EKeys::MotionController_Left_Thumbstick_Down, 0, 0, 0, 0 },
			{ "LeftThumbLeft", EKeys::MotionController_Left_Thumbstick_Left, 0, 0, 0, 0 },
			{ "LeftThumbRight", EKeys::MotionController_Left_Thumbstick_Right, 0, 0, 0, 0 },
			{ "LeftTrigger", EKeys::MotionController_Left_Trigger, 0, 0, 0, 0 },
			{ "LeftGrip", EKeys::MotionController_Left_Grip1, 0, 0, 0, 0 },
			{ "LeftGrip", EKeys::MotionController_Left_Grip2, 0, 0, 0, 0 }
		};

		for (auto& inputKey : controllers)
		{
			World->GetFirstPlayerController()->PlayerInput->AddEngineDefinedActionMapping(inputKey);
		}

		TArray<FInputAxisKeyMapping> controllersAxis = {
			{ "RightThumbX", EKeys::MotionController_Right_Thumbstick_X, 1.0f },
			{ "RightThumbY", EKeys::MotionController_Right_Thumbstick_Y, -1.0f },
			{ "LeftThumbX", EKeys::MotionController_Left_Thumbstick_X, 1.0f },
			{ "LeftThumbY", EKeys::MotionController_Left_Thumbstick_Y, -1.0f }
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
}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Right actions
	PlayerInputComponent->BindAction("RightThumb", IE_Pressed, this, &AVRPawn::RightThumb_Press);
	PlayerInputComponent->BindAction("RightThumb", IE_Released, this, &AVRPawn::RightThumb_Release);
	PlayerInputComponent->BindAction("RightThumbUp", IE_Pressed, this, &AVRPawn::RightThumbUp_Press);
	PlayerInputComponent->BindAction("RightThumbUp", IE_Released, this, &AVRPawn::RightThumbUp_Release);
	PlayerInputComponent->BindAction("RightThumbDown", IE_Pressed, this, &AVRPawn::RightThumbDown_Press);
	PlayerInputComponent->BindAction("RightThumbDown", IE_Released, this, &AVRPawn::RightThumbDown_Release);
	PlayerInputComponent->BindAction("RightThumbLeft", IE_Pressed, this, &AVRPawn::RightThumbLeft_Press);
	PlayerInputComponent->BindAction("RightThumbLeft", IE_Released, this, &AVRPawn::RightThumbLeft_Release);
	PlayerInputComponent->BindAction("RightThumbRight", IE_Pressed, this, &AVRPawn::RightThumbRight_Press);
	PlayerInputComponent->BindAction("RightThumbRight", IE_Released, this, &AVRPawn::RightThumbRight_Release);
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
	PlayerInputComponent->BindAction("LeftThumbUp", IE_Pressed, this, &AVRPawn::LeftThumbUp_Press);
	PlayerInputComponent->BindAction("LeftThumbUp", IE_Released, this, &AVRPawn::LeftThumbUp_Release);
	PlayerInputComponent->BindAction("LeftThumbDown", IE_Pressed, this, &AVRPawn::LeftThumbDown_Press);
	PlayerInputComponent->BindAction("LeftThumbDown", IE_Released, this, &AVRPawn::LeftThumbDown_Release);
	PlayerInputComponent->BindAction("LeftThumbLeft", IE_Pressed, this, &AVRPawn::LeftThumbLeft_Press);
	PlayerInputComponent->BindAction("LeftThumbLeft", IE_Released, this, &AVRPawn::LeftThumbLeft_Release);
	PlayerInputComponent->BindAction("LeftThumbRight", IE_Pressed, this, &AVRPawn::LeftThumbRight_Press);
	PlayerInputComponent->BindAction("LeftThumbRight", IE_Released, this, &AVRPawn::LeftThumbRight_Release);
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
	// UE_LOG(LogTemp, Warning, TEXT("R Thumb Press. Axis=%.2f,%.2f"), RightThumbAxis.X, RightThumbAxis.Y);
	RightInteraction->BeginTeleport();
}

void AVRPawn::RightThumb_Release()
{
	// UE_LOG(LogTemp, Warning, TEXT("R Thumb Release"));
	RightInteraction->EndTeleport();
}

void AVRPawn::RightThumbUp_Press()
{
	UE_LOG(LogTemp, Warning, TEXT("R ThumbUp Press"));
}

void AVRPawn::RightThumbUp_Release()
{
	UE_LOG(LogTemp, Warning, TEXT("R ThumbUp Release"));
}

void AVRPawn::RightThumbDown_Press()
{
	UE_LOG(LogTemp, Warning, TEXT("R ThumbDown Press"));
}

void AVRPawn::RightThumbDown_Release()
{
	UE_LOG(LogTemp, Warning, TEXT("R ThumbDown Release"));
}

void AVRPawn::RightThumbLeft_Press()
{
	UE_LOG(LogTemp, Warning, TEXT("R ThumbLeft Press"));
}

void AVRPawn::RightThumbLeft_Release()
{
	UE_LOG(LogTemp, Warning, TEXT("R ThumbLeft Release"));
}

void AVRPawn::RightThumbRight_Press()
{
	UE_LOG(LogTemp, Warning, TEXT("R ThumbRight Press"));
}

void AVRPawn::RightThumbRight_Release()
{
	UE_LOG(LogTemp, Warning, TEXT("R ThumbRight Release"));
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

void AVRPawn::LeftThumb_Press()
{
	// UE_LOG(LogTemp, Warning, TEXT("L Thumb Press. Axis=%.2f,%.2f"), LeftThumbAxis.X, LeftThumbAxis.Y);
	LeftInteraction->BeginTeleport();
}

void AVRPawn::LeftThumb_Release()
{
	// UE_LOG(LogTemp, Warning, TEXT("L Thumb Release"));
	LeftInteraction->EndTeleport();
}

void AVRPawn::LeftThumbUp_Press()
{
	UE_LOG(LogTemp, Warning, TEXT("L ThumbUp Press"));
}

void AVRPawn::LeftThumbUp_Release()
{
	UE_LOG(LogTemp, Warning, TEXT("L ThumbUp Release"));
}

void AVRPawn::LeftThumbDown_Press()
{
	UE_LOG(LogTemp, Warning, TEXT("L ThumbDown Press"));
}

void AVRPawn::LeftThumbDown_Release()
{
	UE_LOG(LogTemp, Warning, TEXT("L ThumbDown Release"));
}

void AVRPawn::LeftThumbLeft_Press()
{
	UE_LOG(LogTemp, Warning, TEXT("L ThumbLeft Press"));
}

void AVRPawn::LeftThumbLeft_Release()
{
	UE_LOG(LogTemp, Warning, TEXT("L ThumbLeft Release"));
}

void AVRPawn::LeftThumbRight_Press()
{
	UE_LOG(LogTemp, Warning, TEXT("L ThumbRight Press"));
}

void AVRPawn::LeftThumbRight_Release()
{
	UE_LOG(LogTemp, Warning, TEXT("L ThumbRight Release"));
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


