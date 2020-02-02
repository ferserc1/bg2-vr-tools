// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

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

	if (Interaction == IM_Teleport)
	{
		CurrentHit = ParabolicLineTrace(10, 0.1f, 500.0f);
		InteractWithHit(CurrentHit);
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
	Interaction = IM_None;
}

void UInteractionComponent::BeginTouch()
{
	if (Interaction == IM_None)
	{
		Interaction = IM_Touch;
	}
}

void UInteractionComponent::EndTouch()
{
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
	Interaction = IM_None;
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

void UInteractionComponent::InteractWithHit(FHitResult Hit)
{
	FHitResult inHit = Hit;
	UPrimitiveComponent * inHitComponent = inHit.Component.Get();
	AActor * inHitActor = Hit.Actor.Get();

	// TODO: Modificar ObjectInteractionInterface para tratar las acciones de teleport, touch y grip en lugar de
	// TraceLeave, TraceHit, TraceHit, etc..
	if (FocusObject != nullptr)
	{
		IObjectInteractionInterface::Execute_TraceMove(FocusObject.GetObject(), inHit);

		if (FocusComponent != inHitComponent)
		{
			IObjectInteractionInterface::Execute_TraceLeaveComponent(FocusObject.GetObject(), inHit, FocusComponent);

			IObjectInteractionInterface::Execute_TraceHitComponent(FocusObject.GetObject(), inHit, inHitComponent);

			IObjectInteractionInterface::Execute_TraceLeaveObject(FocusObject.GetObject(), inHit);

			FocusObject = nullptr;
			FocusComponent = nullptr;

			
			if (inHitActor->Implements<UObjectInteractionInterface>())
			{
				IObjectInteractionInterface::Execute_TraceHitObject(inHitActor, inHit);
				IObjectInteractionInterface::Execute_TraceHitComponent(inHitActor, inHit, inHitComponent);

				FocusObject.SetObject(inHitActor);
				FocusObject.SetInterface(Cast<IObjectInteractionInterface>(inHitActor));
				FocusComponent = inHitComponent;
			}
		}
	}


}