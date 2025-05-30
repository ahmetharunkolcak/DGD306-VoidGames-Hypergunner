using UnrealBuildTool;

public class VoidGamesHypergunnerEditorTarget : TargetRules {
	public VoidGamesHypergunnerEditorTarget(TargetInfo Target) : base(Target) {
		this.Type                 = TargetType.Editor;
		this.DefaultBuildSettings = BuildSettingsVersion.V5;
		this.IncludeOrderVersion  = EngineIncludeOrderVersion.Unreal5_5;
		this.ExtraModuleNames.Add("VoidGamesHypergunner");
	}
}
