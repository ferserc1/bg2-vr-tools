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
	else if (Interaction == IM_Touch)
	{
		CurrentHit = StraightLineTrace(50.0f);
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
	if (FocusObject != nullptr && FocusObject->Implements<UObjectInteractionInterface>())
	{
		if (FocusComponent != nullptr && FocusComponent->Implements<UObjectInteractionInterface>())
		{
			IObjectInteractionInterface::Execute_EndAction(FocusObject, CurrentHit, Interaction);
		}

		IObjectInteractionInterface::Execute_EndAction(FocusObject, CurrentHit, Interaction);
	}
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
	if (FocusObject != nullptr && FocusObject->Implements<UObjectInteractionInterface>())
	{
		if (FocusComponent != nullptr && FocusComponent->Implements<UObjectInteractionInterface>())
		{
			IObjectInteractionInterface::Execute_EndAction(FocusObject, CurrentHit, Interaction);
		}

		IObjectInteractionInterface::Execute_EndAction(FocusObject, CurrentHit, Interaction);
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
	if (FocusObject != nullptr && FocusObject->Implements<UObjectInteractionInterface>())
	{
		if (FocusComponent != nullptr && FocusComponent->Implements<UObjectInteractionInterface>())
		{
			IObjectInteractionInterface::Execute_EndAction(FocusObject, CurrentHit, Interaction);
		}

		IObjectInteractionInterface::Execute_EndAction(FocusObject, CurrentHit, Interaction);
	}
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

FHitResult UInteractionComponent::StraightLineTrace(float distance)
{
	FHitResult Hit;
	FVector initLoc = GetComponentLocation();
	FHitResult tempHit;
	FCollisionQueryParams traceParams(FName(TEXT("StraightTrace")), true, nullptr);
	traceParams.bTraceComplex = false;
	FVector end = initLoc + (GetForwardVector() * distance);
	DrawDebugLine(GetWorld(), initLoc, end, FColor::Yellow, false, 0, 0, 1);
	if (GetWorld()->LineTraceSingleByChannel(
		tempHit,
		initLoc,
		end,
		ECollisionChannel::ECC_Visibility,
		traceParams))
	{
		Hit = tempHit;
	}
	return Hit;
}

void UInteractionComponent::InteractWithHit(FHitResult Hit)
{
	FHitResult inHit = Hit;
	UPrimitiveComponent * inHitComponent = inHit.Component.Get();
	AActor * inHitActor = Hit.Actor.Get();

	// TODO: Modificar ObjectInteractionInterface para tratar las acciones de teleport, touch y grip en lugar de
	// TraceLeave, TraceHit, TraceHit, etc..
	if (FocusObject != nullptr && FocusObject->Implements<UObjectInteractionInterface>())
	{
		IObjectInteractionInterface::Execute_TraceMove(FocusObject, inHit, Interaction);

		if (FocusComponent != inHitComponent)
		{
			IObjectInteractionInterface::Execute_TraceLeaveComponent(FocusObject, inHit, FocusComponent, Interaction);

			IObjectInteractionInterface::Execute_TraceHitComponent(FocusObject, inHit, inHitComponent, Interaction);

			IObjectInteractionInterface::Execute_TraceLeaveObject(FocusObject, inHit, Interaction);

			FocusObject = nullptr;
			FocusComponent = nullptr;
		}
	}

	if (inHitActor != nullptr && inHitActor->Implements<UObjectInteractionInterface>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit actor"));

		IObjectInteractionInterface::Execute_TraceHitObject(inHitActor, inHit, Interaction);
		IObjectInteractionInterface::Execute_TraceHitComponent(inHitActor, inHit, inHitComponent, Interaction);

		FocusObject = inHitActor;
		FocusComponent = inHitComponent;
	}


}