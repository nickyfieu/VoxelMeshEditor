// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NFToolEditorExtensionRuntime : ModuleRules
{
	public NFToolEditorExtensionRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);

        // list from https://lxjk.github.io/2019/10/01/How-to-Make-Tools-in-U-E.html
        PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
				// ... add other public dependencies that you statically link with here ...
			}
			);

        // list from https://lxjk.github.io/2019/10/01/How-to-Make-Tools-in-U-E.html
        PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
