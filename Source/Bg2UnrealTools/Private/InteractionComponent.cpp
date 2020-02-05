// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "TeleportTargetInterface.h"
#include "TouchTargetInterface.h"
#include "GripTargetInterface.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentHit = FHitResult();
	if (Interaction == IM_Teleport)
	{
		CurrentHit = ParabolicLineTrace(10, 0.1f, 500.0f);
		InteractWithHit(CurrentHit);
	}
	else if (Interaction == IM_None)
	{
		if (StraightLineTrace(100.0f, CurrentHit))
		{
			InteractWithHit(CurrentHit);
		}
	}
}

void UInteractionComponent::BeginTeleport()
{
	if (Interaction == IM_None)
	{
		Interaction = IM_Teleport;
	}
}

void UInteractionComponent::EndTeleport()
{
	if (FocusObject != nullptr && FocusObject->Implements<UTeleportTargetInterface>())
	{
		ITeleportTargetInterface::Execute_EndTeleport(FocusObject, CurrentHit);
	}
	Interaction = IM_None;
}

void UInteractionComponent::BeginTouch()
{
	if (Interaction == IM_None)
	{
		Interaction = IM_Touch;
		InteractWithHit(CurrentHit);
	}
}

void UInteractionComponent::EndTouch()
{
	if (FocusObject != nullptr && FocusObject->Implements<UTouchTargetInterface>())
	{
		ITouchTargetInterface::Execute_EndTouch(FocusObject, CurrentHit);
	}
	Interaction = IM_None;
}

void UInteractionComponent::BeginGrip()
{
	if (Interaction == IM_None)
	{
		Interaction = IM_Grip;
	}
}

void UInteractionComponent::EndGrip()
{
	if (FocusObject != nullptr && FocusObject->Implements<UGripTargetInterface>())
	{
		IGripTargetInterface::Execute_EndGrip(FocusObject, CurrentHit);
	}
	Interaction = IM_None;
}

void UInteractionComponent::DirectionButtonPress(ThumbDirectionButton Button)
{
	
}

void UInteractionComponent::DirectionButtonRelease(ThumbDirectionButton Button)
{
	if (Button == TB_Left)
	{
		// Rotar a la izquierda
		auto pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (pawn)
		{	
			FRotator NewRotation = FRotator(0.0f, -22.5f, 0.0f);
			pawn->AddActorLocalRotation(FQuat(NewRotation), false, 0, ETeleportType::None);
		}
	}
	else if (Button == TB_Right)
	{
		// Rotar a la derecha
		auto pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (pawn)
		{
			FRotator NewRotation = FRotator(0.0f, 22.5f, 0.0f);
			pawn->AddActorLocalRotation(FQuat(NewRotation), false, 0, ETeleportType::None);
		}
	}
	//else if (Button == TB_Up)
	//{
	//	// Avanzar
	//	auto World = GetWorld();
	//	if (World)
	//	{
	//		auto PlayerController = World->GetFirstPlayerController();
	//		auto Pawn = PlayerController->GetPawn();
	//		auto distance = 100.0f;
	//		FVector NewLocation = Pawn->GetActorLocation() - (GetForwardVector() * distance);
	//		Pawn->SetActorLocation(NewLocation);
	//	}
	//}
	//else if (Button == TB_Down)
	//{
	//	// Retroceder
	//	auto World = GetWorld();
	//	if (World)
	//	{
	//		auto PlayerController = World->GetFirstPlayerController();
	//		auto Pawn = PlayerController->GetPawn();
	//		auto distance = -100.0f;
	//		FVector NewLocation = Pawn->GetActorLocation() - (GetForwardVector() * distance);
	//		Pawn->SetActorLocation(NewLocation);
	//	}
	//}
}

FHitResult UInteractionComponent::ParabolicLineTrace(int Steps, float TimeStep, float Speed)
{
	//  z = (t * vz) - (0.5 * g * t^2)
	FVector initLoc = GetComponentLocation();
	FVector prevLoc = initLoc;
	FVector velocity = GetForwardVector() * Speed;
	float inTimeStep = TimeStep;
	int inSteps = Steps;
	FHitResult tempHit;

	FCollisionQueryParams traceParams(FName(TEXT("ParabolicTrace")), true, nullptr);
	traceParams.bTraceComplex = false;
	for (int i = 1; i < inSteps; ++i)
	{
		float timeStepIt = inTimeStep * static_cast<float>(i);
		FVector end = FVector(
			velocity.X * timeStepIt,
			velocity.Y * timeStepIt,
			velocity.Z * timeStepIt - timeStepIt * timeStepIt * 980.0f * 0.5f
		) + initLoc;
		DrawDebugLine(GetWorld(), prevLoc, end, FColor::Red, false, 0, 0, 1);
		if (GetWorld()->LineTraceSingleByChannel(
			tempHit,
			prevLoc,
			end,
			ECollisionChannel::ECC_Visibility,
			traceParams))
		{
			break;
		}
		prevLoc = end;
	}
	
	return tempHit;
}

bool UInteractionComponent::StraightLineTrace(float distance, FHitResult& result)
{
	FVector initLoc = GetComponentLocation();
	FHitResult tempHit;
	FCollisionQueryParams traceParams(FName(TEXT("StraightTrace")), true, nullptr);
	traceParams.bTraceComplex = false;
	FVector end = initLoc + (GetForwardVector() * distance);
	if (GetWorld()->LineTraceSingleByChannel(
		tempHit,
		initLoc,
		end,
		ECollisionChannel::ECC_Visibility,
		traceParams))
	{
		result = tempHit;
		if (result.Actor->Implements<UTouchTargetInterface>())
		{
			DrawDebugLine(GetWorld(), initLoc, initLoc + (GetForwardVector() * tempHit.Distance), FColor::Yellow, false, 0, 0, 1);
		}
		return true;
	}
	else
	{
		return false;
	}
}

void UInteractionComponent::InteractWithHit(FHitResult Hit)
{
	FHitResult inHit = Hit;
	UPrimitiveComponent * inHitComponent = inHit.Component.Get();
	AActor * inHitActor = Hit.Actor.Get();

	if (FocusObject != nullptr)
	{
		if (FocusObject->Implements<UTeleportTargetInterface>() && Interaction == IM_Teleport)
		{
			ITeleportTargetInterface::Execute_TeleportMove(FocusObject, Hit);
		}

		if (FocusObject->Implements<UTouchTargetInterface>() && Interaction == IM_Touch)
		{
			ITouchTargetInterface::Execute_TouchMove(FocusObject, Hit);
		}

		if (FocusObject->Implements<UGripTargetInterface>() && Interaction == IM_Grip)
		{
			IGripTargetInterface::Execute_GripMove(FocusObject, Hit);
		}

		FocusObject = nullptr;
	}

	if (inHitActor != nullptr)
	{
		FocusObject = inHitActor;
	}
}