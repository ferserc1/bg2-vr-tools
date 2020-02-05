// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeleportTargetInterface.h"
#include "TeleportArea.generated.h"

UCLASS()
class BG2VRTOOLS_API ATeleportArea : public AActor, public ITeleportTargetInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleportArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void TeleportMove_Implementation(FHitResult hit) override;
	virtual void EndTeleport_Implementation(FHitResult Hit) override;
};
