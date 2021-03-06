// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "TeleportTargetInterface.h"
#include "TouchTargetInterface.h"
#include "GripTargetInterface.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "HMDSettings.h"
#include "IXRTrackingSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"

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
		CurrentHit = ParabolicLineTrace(ParabolicLineSteps, ParabolicTimeStep, ParabolicLineSpeed);
		InteractWithHit(CurrentHit);
	}
	else if (Interaction == IM_None)
	{
		if (StraightLineTrace(250.0f, CurrentHit))
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
	if (FocusComponent != nullptr && FocusComponent->Implements<UTeleportTargetInterface>())
	{
		ITeleportTargetInterface::Execute_EndTeleport(FocusComponent, CurrentHit);
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
	if (FocusComponent != nullptr && FocusComponent->Implements<UTouchTargetInterface>())
	{
		ITouchTargetInterface::Execute_EndTouch(FocusComponent, CurrentHit);
	}
	Interaction = IM_None;

	if (FocusWidget != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Perform interaction with widget"));
	}
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
	if (FocusComponent != nullptr && FocusComponent->Implements<UGripTargetInterface>())
	{
		IGripTargetInterface::Execute_EndGrip(FocusComponent, CurrentHit);
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
	else if (Button == TB_Up)
	{
		if (OnUpButtonPressed.IsBound())
		{
			OnUpButtonPressed.Broadcast();
			UE_LOG(LogTemp, Warning, TEXT("Button Up"));
		}
	}
	else if (Button == TB_Down)
	{
		if (OnDownButtonPressed.IsBound())
		{
			OnDownButtonPressed.Broadcast();
			UE_LOG(LogTemp, Warning, TEXT("Button Down"));
		}
	}
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

	struct LineSegment {
		FVector start;
		FVector end;
	};
	TArray<LineSegment> Segments;
	FColor lineColor = FColor::Red;

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
		if (GetWorld()->LineTraceSingleByChannel(
			tempHit,
			prevLoc,
			end,
			ECollisionChannel::ECC_Visibility,
			traceParams))
		{
			lineColor = FColor::Green;
			Segments.Add({ prevLoc, tempHit.TraceEnd });
			break;
		}
		Segments.Add({ prevLoc, end });
		prevLoc = end;
	}

	for (auto s : Segments)
	{
		DrawDebugLine(GetWorld(), s.start, s.end, lineColor, false, 0, 0, 1);
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
		else if (Cast<UWidgetComponent>(result.Component)) {
			DrawDebugLine(GetWorld(), initLoc, initLoc + (GetForwardVector() * tempHit.Distance), FColor::Orange, false, 0, 0, 1);
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

	CheckInteraction(FocusObject, Hit);
	CheckInteraction(FocusComponent, Hit);
	FocusObject = nullptr;
	FocusComponent = nullptr;

	if (inHitActor != nullptr)
	{
		FocusObject = inHitActor;
	}
	if (inHitComponent != nullptr)
	{
		FocusComponent = inHitComponent;
	}
}

void UInteractionComponent::CheckInteraction(UObject* InteractionObject, FHitResult Hit)
{
	// Hint:
	// inHitActor->FindComponentByClass<ITeleportTargetInterface>();
	if (InteractionObject != nullptr)
	{
		if (InteractionObject->Implements<UTeleportTargetInterface>() && Interaction == IM_Teleport)
		{
			ITeleportTargetInterface::Execute_TeleportMove(InteractionObject, Hit);
		}

		if (InteractionObject->Implements<UTouchTargetInterface>() && Interaction == IM_Touch)
		{
			ITouchTargetInterface::Execute_TouchMove(InteractionObject, Hit);
		}

		if (InteractionObject->Implements<UGripTargetInterface>() && Interaction == IM_Grip)
		{
			IGripTargetInterface::Execute_GripMove(InteractionObject, Hit);
		}
		
		FocusWidget = Cast<UWidgetComponent>(Hit.Component);
	}
}
