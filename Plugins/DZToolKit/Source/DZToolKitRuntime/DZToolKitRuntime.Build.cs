// Copyright 2018 azhecheng, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DZToolKitRuntime : ModuleRules
{
#if WITH_FORWARDED_MODULE_RULES_CTOR
    public DZToolKitRuntime(ReadOnlyTargetRules Target) : base(Target)
#else
	public DZToolKitRuntime(TargetInfo Target)
#endif
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicIncludePaths.AddRange(
			new string[] {
				"DZToolKitRuntime/Public",
				"DZToolKitRuntime/Public/Component"
				// ... add public include paths required here ...
			}
			);


		PrivateIncludePaths.AddRange(
			new string[] {
				"DZToolKitRuntime/Private/Component",
				// ... add other private include paths required here ...
			}
			);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"Slate",
                "HTTP",
                "Json"
				// ... add other public dependencies that you statically link with here ...
			}
			);
	}
}
