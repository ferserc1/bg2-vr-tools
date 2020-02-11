// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportArea.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Engine/CollisionProfile.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ATeleportArea::ATeleportArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	TeleportSurface = CreateDefaultSubobject<UBoxComponent>(TEXT("TeleportArea"));
	RootComponent = TeleportSurface;

	TeleportSurface->InitBoxExtent(FVector(1000.0, 1000.0, 10.0f));
	TeleportSurface->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
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

void ATeleportArea::TeleportMove_Implementation(FHitResult hit)
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

void ATeleportArea::EndTeleport_Implementation(FHitResult hit)
{
	auto World = GetWorld();
	if (World)
	{
		auto PlayerController = World->GetFirstPlayerController();
		PlayerController->GetPawn()->SetActorLocation(hit.Location);
	}
}


