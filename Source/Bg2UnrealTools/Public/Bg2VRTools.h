#pragma once

#include "ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(Bg2VR, Display, All);

class Bg2VRToolsImpl : public IModuleInterface
{
public:
    void StartupModule();
    void ShutdownModule();
};
