// Copyright (c) 2025 Keita Nakamura
//
// This file is part of the Editor Language Toggle plugin.
#include "EditorLanguageToggle.h"
#include "EditorLanguageToggleSettings.h"
#include "EditorLanguageToggleSettingsCustomization.h"
#include "LevelEditor.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Internationalization/Internationalization.h"
#include "Misc/ConfigCacheIni.h"
#include "ToolMenus.h"
#include "Internationalization/Culture.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "PropertyEditorModule.h"

#define LOCTEXT_NAMESPACE "FEditorLanguageToggleModule"

TSharedPtr<FSlateStyleSet> EditorLanguageToggleStyle;

void FEditorLanguageToggleModule::StartupModule()
{
    Instance = MakeShared<FEditorLanguageToggleModule>();
    
    // メニュー拡張の登録
    if (UToolMenus::IsToolMenuUIEnabled())
    {
        UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(Instance.Get(), &FEditorLanguageToggleModule::RegisterMenu));
    }

    // DetailCustomizationの登録
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomClassLayout(
        UEditorLanguageToggleSettings::StaticClass()->GetFName(),
        FOnGetDetailCustomizationInstance::CreateStatic(&FEditorLanguageToggleSettingsCustomization::MakeInstance));
}

void FEditorLanguageToggleModule::ShutdownModule()
{
    // DetailCustomizationの登録解除
    if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
        PropertyModule.UnregisterCustomClassLayout("EditorLanguageToggleSettings");
    }
    
    if (EditorLanguageToggleStyle.IsValid())
    {
        FSlateStyleRegistry::UnRegisterSlateStyle(*EditorLanguageToggleStyle);
        EditorLanguageToggleStyle.Reset();
    }

    UnregisterMenu();
}

void FEditorLanguageToggleModule::RegisterMenu()
{
    const UEditorLanguageToggleSettings* Settings = UEditorLanguageToggleSettings::Get();
    if (!Settings->bEnableLanguageToggle)
    {
        return;
    }

    UToolMenu* ToolbarMenu = nullptr;
    bool bIsTop = false;
    if (Settings->ToolbarPosition == EToolbarPosition::Top)
    {
        ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.User");
        bIsTop = true;
    }
    else
    {
        ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.StatusBar.ToolBar");
    }

    if (ToolbarMenu == nullptr)
    {
        return;
    }

    FToolMenuInsert InsertPosition;
    if (!bIsTop)
    {
        InsertPosition = FToolMenuInsert("Compile", EToolMenuInsertType::After);
    }

    FToolMenuSection& Section = ToolbarMenu->AddSection("EditorLanguageToggle", LOCTEXT("EditorLanguageToggleSection", "Editor Language Toggle"), InsertPosition);
    FString CurrentCulture = FInternationalization::Get().GetCurrentCulture()->GetName();
    FString TargetCulture = Settings->SelectedCulture;
    Section.AddEntry(FToolMenuEntry::InitToolBarButton(
        "EditorLanguageToggleSwitch",
        FUIAction(
            FExecuteAction::CreateRaw(this, &FEditorLanguageToggleModule::ToggleLanguage),
            FCanExecuteAction(),
            FIsActionChecked::CreateLambda([this]()
            {
                FString CurrentCulture = FInternationalization::Get().GetCurrentCulture()->GetName();
                return (CurrentCulture == "en");
            })
        ),
        FText::GetEmpty(),
        LOCTEXT("EditorLanguageToggleSwitchTooltip", "Toggle Editor Language"),
        FSlateIcon("EditorStyle", "LocalizationDashboard.CompileTextAllTargetsAllCultures"),
        EUserInterfaceActionType::ToggleButton
    ));

}

void FEditorLanguageToggleModule::UnregisterMenu()
{
    if (UToolMenus::IsToolMenuUIEnabled())
    {
        UToolMenus::UnRegisterStartupCallback(Instance.Get());
    }

    if (!UToolMenus::Get())
    {
        return;
    }
    
    if (UToolMenu* LevelEditorToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.User");
        LevelEditorToolbarMenu->FindSection("EditorLanguageToggle"))
    {
        LevelEditorToolbarMenu->RemoveSection("EditorLanguageToggle");
    }
    if (UToolMenu* StatusBarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.StatusBar.ToolBar");
        StatusBarMenu->FindSection("EditorLanguageToggle"))
    {
        StatusBarMenu->RemoveSection("EditorLanguageToggle");
    }
    
}

void FEditorLanguageToggleModule::ToggleLanguage()
{
    const UEditorLanguageToggleSettings* Settings = UEditorLanguageToggleSettings::Get();
    FString CurrentCulture = FInternationalization::Get().GetCurrentCulture()->GetName();
    FString TargetCulture = Settings->SelectedCulture;

    // 現在が設定値なら英語、英語なら設定値、それ以外は英語に切り替え
    if (CurrentCulture == TargetCulture)
    {
        FInternationalization::Get().SetCurrentCulture("en");
    }
    else
    {
        FInternationalization::Get().SetCurrentCulture(TargetCulture);
    }
}

void FEditorLanguageToggleModule::RegisterSettings()
{
    Get().RegisterMenu();
}

void FEditorLanguageToggleModule::UnregisterSettings()
{
    Get().UnregisterMenu();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FEditorLanguageToggleModule, EditorLanguageToggle)
