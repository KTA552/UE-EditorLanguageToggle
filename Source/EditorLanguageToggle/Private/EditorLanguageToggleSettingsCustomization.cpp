// Copyright (c) 2025 Keita Nakamura
//
// This file is part of the Editor Language Toggle plugin.
#include "EditorLanguageToggleSettingsCustomization.h"
#include "EditorLanguageToggleSettings.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "PropertyHandle.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "EditorLanguageToggle"

TSharedRef<IDetailCustomization> FEditorLanguageToggleSettingsCustomization::MakeInstance()
{
    return MakeShared<FEditorLanguageToggleSettingsCustomization>();
}

void FEditorLanguageToggleSettingsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    // SourceCultureとTargetCultureプロパティのハンドルを取得
    TSharedRef<IPropertyHandle> SourceCultureProperty = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UEditorLanguageToggleSettings, SourceCulture));
    TSharedRef<IPropertyHandle> TargetCultureProperty = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UEditorLanguageToggleSettings, TargetCulture));
    
    // デフォルトのプロパティ表示を隠す
    DetailBuilder.HideProperty(SourceCultureProperty);
    DetailBuilder.HideProperty(TargetCultureProperty);
    
    // カスタムカテゴリを作成
    IDetailCategoryBuilder& CategoryBuilder = DetailBuilder.EditCategory("Default");
    
    // 言語オプションを取得
    TArray<FEditorLanguageOption> LanguageOptions = GetDefault<UEditorLanguageToggleSettings>()->GetAvailableEditorLanguages();
    
    // ドロップダウン用のオプション配列を作成
    LanguageComboOptions.Empty();
    for (const FEditorLanguageOption& Option : LanguageOptions)
    {
        LanguageComboOptions.Add(MakeShared<FEditorLanguageOption>(Option));
    }
    
    // Source Language用のカスタムプロパティ行を追加
    FString SourceCurrentValue;
    SourceCultureProperty->GetValue(SourceCurrentValue);
    
    TSharedPtr<FEditorLanguageOption> SourceSelectedOption;
    for (const TSharedPtr<FEditorLanguageOption>& Option : LanguageComboOptions)
    {
        if (Option->CultureCode == SourceCurrentValue)
        {
            SourceSelectedOption = Option;
            break;
        }
    }
    
    CategoryBuilder.AddCustomRow(LOCTEXT("SourceLanguage", "Source Language"))
    .NameContent()
    [
        SourceCultureProperty->CreatePropertyNameWidget()
    ]
    .ValueContent()
    [
        SNew(SComboBox<TSharedPtr<FEditorLanguageOption>>)
        .OptionsSource(&LanguageComboOptions)
        .OnSelectionChanged(this, &FEditorLanguageToggleSettingsCustomization::OnLanguageSelectionChanged, SourceCultureProperty)
        .OnGenerateWidget(this, &FEditorLanguageToggleSettingsCustomization::OnGenerateLanguageWidget)
        .InitiallySelectedItem(SourceSelectedOption)
        [
            SNew(STextBlock)
            .Text(this, &FEditorLanguageToggleSettingsCustomization::GetSelectedLanguageText, SourceCultureProperty)
        ]
    ];
    
    // Target Language用のカスタムプロパティ行を追加
    FString TargetCurrentValue;
    TargetCultureProperty->GetValue(TargetCurrentValue);
    
    TSharedPtr<FEditorLanguageOption> TargetSelectedOption;
    for (const TSharedPtr<FEditorLanguageOption>& Option : LanguageComboOptions)
    {
        if (Option->CultureCode == TargetCurrentValue)
        {
            TargetSelectedOption = Option;
            break;
        }
    }
    
    CategoryBuilder.AddCustomRow(LOCTEXT("TargetLanguage", "Target Language"))
    .NameContent()
    [
        TargetCultureProperty->CreatePropertyNameWidget()
    ]
    .ValueContent()
    [
        SNew(SComboBox<TSharedPtr<FEditorLanguageOption>>)
        .OptionsSource(&LanguageComboOptions)
        .OnSelectionChanged(this, &FEditorLanguageToggleSettingsCustomization::OnLanguageSelectionChanged, TargetCultureProperty)
        .OnGenerateWidget(this, &FEditorLanguageToggleSettingsCustomization::OnGenerateLanguageWidget)
        .InitiallySelectedItem(TargetSelectedOption)
        [
            SNew(STextBlock)
            .Text(this, &FEditorLanguageToggleSettingsCustomization::GetSelectedLanguageText, TargetCultureProperty)
        ]
    ];
}

void FEditorLanguageToggleSettingsCustomization::OnLanguageSelectionChanged(TSharedPtr<FEditorLanguageOption> NewSelection, ESelectInfo::Type SelectInfo, TSharedRef<IPropertyHandle> PropertyHandle)
{
    if (NewSelection.IsValid())
    {
        PropertyHandle->SetValue(NewSelection->CultureCode);
    }
}

TSharedRef<SWidget> FEditorLanguageToggleSettingsCustomization::OnGenerateLanguageWidget(TSharedPtr<FEditorLanguageOption> InOption)
{
    return SNew(STextBlock)
        .Text(FText::FromString(InOption->DisplayName));
}

FText FEditorLanguageToggleSettingsCustomization::GetSelectedLanguageText(TSharedRef<IPropertyHandle> PropertyHandle) const
{
    FString CurrentValue;
    PropertyHandle->GetValue(CurrentValue);
    
    // 現在の値に対応する表示名を取得
    for (const TSharedPtr<FEditorLanguageOption>& Option : LanguageComboOptions)
    {
        if (Option->CultureCode == CurrentValue)
        {
            return FText::FromString(Option->DisplayName);
        }
    }
    
    // 見つからない場合はそのまま表示
    return FText::FromString(CurrentValue);
}

#undef LOCTEXT_NAMESPACE
