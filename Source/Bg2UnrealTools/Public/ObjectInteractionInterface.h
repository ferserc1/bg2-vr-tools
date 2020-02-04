
#pragma once

#include "ObjectInteractionInterface.generated.h"

UENUM(BlueprintType)
enum InteractionMode
{
    IM_None = 0,
    IM_Teleport,
    IM_Touch,
    IM_Grip
};

UINTERFACE(BlueprintType)
class BG2VRTOOLS_API UObjectInteractionInterface : public UInterface
{
    GENERATED_UINTERFACE_BODY()
};

class BG2VRTOOLS_API IObjectInteractionInterface
{
    GENERATED_IINTERFACE_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VR")
    void TraceHitObject(FHitResult hit, InteractionMode interaction);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VR")
    void TraceLeaveObject(FHitResult hit, InteractionMode interaction);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VR")
    void TraceHitComponent(FHitResult hit, UPrimitiveComponent * Component, InteractionMode interaction);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VR")
    void TraceLeaveComponent(FHitResult hit, UPrimitiveComponent * Component, InteractionMode interaction);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VR")
    void TraceMove(const FHitResult & hit, InteractionMode interaction);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VR")
    void TraceActivateDown(FHitResult hit, APawn * Instigator, InteractionMode interaction);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VR")
    void TraceActivateUp(FHitResult hit, APawn * Instigator, InteractionMode interaction);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VR")
    void EndAction(FHitResult Hit, InteractionMode interaction);

};
