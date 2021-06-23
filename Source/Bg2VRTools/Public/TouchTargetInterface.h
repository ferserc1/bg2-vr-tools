#pragma once

#include "TouchTargetInterface.generated.h"

UINTERFACE(BlueprintType)
class BG2VRTOOLS_API UTouchTargetInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class BG2VRTOOLS_API ITouchTargetInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VR")
	void TouchMove(FHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VR")
	void EndTouch(FHitResult Hit);
};
