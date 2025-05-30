using UnrealBuildTool;

public class VoidGamesHypergunnerTarget : TargetRules {
	public VoidGamesHypergunnerTarget(TargetInfo Target) : base(Target) {
		this.Type                 = TargetType.Game;
		this.DefaultBuildSettings = BuildSettingsVersion.V5;
		this.IncludeOrderVersion  = EngineIncludeOrderVersion.Unreal5_5;
		this.ExtraModuleNames.Add("VoidGamesHypergunner");
	}
}
