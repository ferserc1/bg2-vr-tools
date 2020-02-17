// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "TeleportTargetInterface.h"
#include "TeleportAreaComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BG2VRTOOLS_API UTeleportAreaComponent : public UBoxComponent, public ITeleportTargetInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTeleportAreaComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void TeleportMove_Implementation(FHitResult hit) override;
	virtual void EndTeleport_Implementation(FHitResult Hit) override;
};
