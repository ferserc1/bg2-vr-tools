#pragma once

#include "GripTargetInterface.generated.h"

UINTERFACE(BlueprintType)
class BG2VRTOOLS_API UGripTargetInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class BG2VRTOOLS_API IGripTargetInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VR")
	void GripMove(FHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VR")
	void EndGrip(FHitResult Hit);
};
