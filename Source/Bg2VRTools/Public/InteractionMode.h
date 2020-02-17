#pragma once

#include "InteractionMode.generated.h"

UENUM(BlueprintType)
enum InteractionMode
{
    IM_None = 0,
    IM_Teleport,
    IM_Touch,
    IM_Grip
};
