
#pragma once

#include "ObjectInteractionInterface.generated.h"

UINTERFACE(BlueprintType)
class BG2VRTOOLS_API UObjectInteractionInterface : public UInterface
{
    GENERATED_UINTERFACE_BODY()
};

class BG2VRTOOLS_API IObjectInteractionInterface
{
    GENERATED_IINTERFACE_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "VR")
    bool TraceHitObject(FHitResult hit);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "VR")
    bool TraceLeaveObject(FHitResult hit);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "VR")
    bool TraceHitComponent(FHitResult hit, UPrimitiveComponent * Component);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "VR")
    bool TraceLeaveComponent(FHitResult hit, UPrimitiveComponent * Component);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "VR")
    bool TraceMove(const FHitResult & hit);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "VR")
    bool TraceActivateDown(FHitResult hit, APawn * Instigator);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "VR")
    bool TraceActivateUp(FHitResult hit, APawn * Instigator);
};
