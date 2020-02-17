// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Engine/Engine.h"
#include "HMDSettings.generated.h"

/**
 *
 */
UCLASS()
class BG2VRTOOLS_API UHMDSettings : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "VR")
	FVector HMDOffset;


	UHMDSettings();

	static UHMDSettings* Save(UWorld* World);
	static UHMDSettings* Restore(UWorld* World);

	static FString SaveSlotName;
	static uint32 SaveUserIndex;
};
