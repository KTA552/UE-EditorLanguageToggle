// Copyright (c) 2025 Keita Nakamura
//
// This file is part of the Editor Language Toggle plugin.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Internationalization/Culture.h"
#include "Internationalization/TextLocalizationManager.h"
#include "Internationalization/Internationalization.h"
#include "EditorLanguageToggleSettings.generated.h"

USTRUCT()
struct FEditorLanguageOption
{
    GENERATED_BODY()

    UPROPERTY()
    FString CultureCode;

    UPROPERTY()
    FString DisplayName;
};

UENUM()
enum class EToolbarPosition : uint8
{
    Top,
    Bottom
};

UCLASS(config=EditorPerProjectUserSettings, defaultconfig)
class EDITORLANGUAGETOGGLE_API UEditorLanguageToggleSettings : public UDeveloperSettings
{
    GENERATED_BODY()
public:

    virtual FName GetCategoryName() const override
    {
        return FName("Plugins");
    }

#if WITH_EDITOR
    virtual FText GetSectionText() const override;
    virtual FText GetSectionDescription() const override;
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
    
    UPROPERTY(EditAnywhere, config, Category="EditorLaunguageToggleSettings", meta=(DisplayName="Enable Language Toggle", ToolTip="Enable or disable the editor language toggle feature."))
    bool bEnableLanguageToggle = true;

    UPROPERTY(EditAnywhere, config, Category="EditorLaunguageToggleSettings", meta=(EditCondition = bEnableLanguageToggle, DisplayName="Toolbar Position", ToolTip="The position of the language toggle button in the toolbar."))
    EToolbarPosition ToolbarPosition;
    
    UPROPERTY(EditAnywhere, config, Category="EditorLaunguageToggleSettings", meta=(DisplayName="Selected Culture", ToolTip="The culture to switch to when toggling the editor language."))
    FString SelectedCulture;
    
    // ドロップダウン用: カルチャーコードのリスト
    UFUNCTION()
    TArray<FString> GetAvailableEditorLanguageCodes() const
    {
        TArray<FString> Codes;
        for (const FEditorLanguageOption& Opt : GetAvailableEditorLanguages())
        {
            Codes.Add(Opt.CultureCode);
        }
        return Codes;
    }
    
    // 表示名取得用
    UFUNCTION()
    TArray<FEditorLanguageOption> GetAvailableEditorLanguages() const
    {
        constexpr bool bIncludeDerivedCultures = false;
        const TArray<FString> LocalizedCultureNames = FTextLocalizationManager::Get().GetLocalizedCultureNames(ELocalizationLoadFlags::Editor);
        TArray<FCultureRef> Cultures = FInternationalization::Get().GetAvailableCultures(LocalizedCultureNames, bIncludeDerivedCultures);

        TArray<FEditorLanguageOption> Result;
        for (const FCultureRef& Culture : Cultures)
        {
            FEditorLanguageOption Opt;
            Opt.CultureCode = Culture->GetName();
            Opt.DisplayName = Culture->GetDisplayName();
            Result.Add(Opt);
        }
        return Result;
    }
    
    // 設定の取得用静的関数
    static const UEditorLanguageToggleSettings* Get()
    {
        return GetDefault<UEditorLanguageToggleSettings>();
    }
};

