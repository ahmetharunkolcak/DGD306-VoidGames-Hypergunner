using UnrealBuildTool;

public class VoidGamesHypergunner : ModuleRules {
	public VoidGamesHypergunner(ReadOnlyTargetRules Target) : base(Target) {
		this.PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		this.PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG"
		});

		this.PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
