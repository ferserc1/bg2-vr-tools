// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportAreaComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Engine/CollisionProfile.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UTeleportAreaComponent::UTeleportAreaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	InitBoxExtent(FVector(1000.0f, 1000.0f, 10.0f));
	SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
}


// Called when the game starts
void UTeleportAreaComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTeleportAreaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTeleportAreaComponent::TeleportMove_Implementation(FHitResult hit)
{
	DrawDebugSphere(
		GetWorld(),
		hit.Location,
		10.0f,
		10,
		FColor::Red,
		false,
		0.0f
	);
}

void UTeleportAreaComponent::EndTeleport_Implementation(FHitResult hit)
{
	auto World = GetWorld();
	if (World)
	{
		auto PlayerController = World->GetFirstPlayerController();
		PlayerController->GetPawn()->SetActorLocation(hit.Location);
	}
}
