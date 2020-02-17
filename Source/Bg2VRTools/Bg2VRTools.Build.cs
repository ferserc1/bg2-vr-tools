using UnrealBuildTool;
using System.IO;

public class Bg2VRTools : ModuleRules
{
    public Bg2VRTools(ReadOnlyTargetRules Target) :base(Target)
    {
        
        PrivatePCHHeaderFile = "Public/Bg2VRToolsPrivatePCH.h";

        PrivateIncludePaths.AddRange(new string[] { "Bg2VRTools/Private" });
        PublicIncludePaths.AddRange(new string[] { "Bg2VRTools/Public" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "SlateCore", "HeadMountedDisplay" });

        PrivateDependencyModuleNames.AddRange(new string[] { "ImageWrapper", "RenderCore" });
    }
}
