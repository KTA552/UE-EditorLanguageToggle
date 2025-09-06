// Copyright (c) 2025 Keita Nakamura
//
// This file is part of the Editor Language Toggle plugin.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FEditorLanguageToggleModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    void RegisterMenu();
    void UnregisterMenu();

    void ToggleLanguage();

    static void RegisterSettings();
    static void UnregisterSettings();

    static FEditorLanguageToggleModule& Get()
    {
        return FModuleManager::LoadModuleChecked<FEditorLanguageToggleModule>("EditorLanguageToggle");
    }
    
private:
    TSharedPtr<class FUICommandList> PluginCommands;

    TSharedPtr<FEditorLanguageToggleModule> Instance;
};

