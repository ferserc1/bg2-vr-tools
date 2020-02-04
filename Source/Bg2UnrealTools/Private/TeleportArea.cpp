// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportArea.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ATeleportArea::ATeleportArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATeleportArea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeleportArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ATeleportArea::TraceHitObject_Implementation(FHitResult hit, InteractionMode interaction)
{
	if (interaction == IM_Teleport)
	{
		DrawDebugSphere(
			GetWorld(),
			hit.Location,
			10.0f,
			5,
			FColor::Red,
			false,
			0.0f
		);
	}
}

void ATeleportArea::EndAction_Implementation(FHitResult hit, InteractionMode interaction)
{
	UE_LOG(LogTemp, Warning, TEXT("TraceLeaveObject teleport area"));
	if (interaction == IM_Teleport)
	{
		// Mover el pawn a esta posición
		auto World = GetWorld();
		if (World)
		{
			auto PlayerController = World->GetFirstPlayerController();
			PlayerController->GetPawn()->SetActorLocation(hit.Location);
		}

	}
}


