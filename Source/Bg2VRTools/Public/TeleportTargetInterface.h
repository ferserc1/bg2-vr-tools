#pragma once

#include "TeleportTargetInterface.generated.h"

UINTERFACE(BlueprintType)
class BG2VRTOOLS_API UTeleportTargetInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class BG2VRTOOLS_API ITeleportTargetInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VR")
	void TeleportMove(FHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VR")
	void EndTeleport(FHitResult Hit);
};