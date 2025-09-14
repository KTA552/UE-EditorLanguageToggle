// Copyright (c) 2025 Keita Nakamura
//
// This file is part of the Editor Language Toggle plugin.

using UnrealBuildTool;

public class EditorLanguageToggle : ModuleRules
{
    public EditorLanguageToggle(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core", 
            "CoreUObject", 
            "Engine", 
            "UnrealEd", 
            "Slate", 
            "SlateCore", 
            "EditorFramework", 
            "LevelEditor",
            "DeveloperSettings",
            "ToolMenus",
            "InputCore",
            "PropertyEditor",
            "EditorStyle",   
            "EditorWidgets"  
        });
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine"
        });
    }
}
