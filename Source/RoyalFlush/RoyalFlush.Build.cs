// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RoyalFlush : ModuleRules
{
    public RoyalFlush(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] 
        { 
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore", 
            "EnhancedInput",
            "DlgSystem"
        });

        PublicIncludePaths.AddRange(new string[] { "../Plugins/DlgSystem/Source" });
    }
}
