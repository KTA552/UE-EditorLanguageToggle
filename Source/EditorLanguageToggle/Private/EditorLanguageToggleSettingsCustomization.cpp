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
    // SelectedCultureプロパティのハンドルを取得
    TSharedRef<IPropertyHandle> SelectedCultureProperty = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UEditorLanguageToggleSettings, SelectedCulture));
    
    // デフォルトのプロパティ表示を隠す
    DetailBuilder.HideProperty(SelectedCultureProperty);
    
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
    
    // 現在選択されている値を取得
    FString CurrentValue;
    SelectedCultureProperty->GetValue(CurrentValue);
    
    // 現在選択されているオプションを特定
    TSharedPtr<FEditorLanguageOption> SelectedOption;
    for (const TSharedPtr<FEditorLanguageOption>& Option : LanguageComboOptions)
    {
        if (Option->CultureCode == CurrentValue)
        {
            SelectedOption = Option;
            break;
        }
    }
    
    // カスタムプロパティ行を追加
    CategoryBuilder.AddCustomRow(LOCTEXT("SelectedLanguage", "Selected Language"))
    .NameContent()
    [
        SelectedCultureProperty->CreatePropertyNameWidget()
    ]
    .ValueContent()
    [
        SNew(SComboBox<TSharedPtr<FEditorLanguageOption>>)
        .OptionsSource(&LanguageComboOptions)
        .OnSelectionChanged(this, &FEditorLanguageToggleSettingsCustomization::OnLanguageSelectionChanged, SelectedCultureProperty)
        .OnGenerateWidget(this, &FEditorLanguageToggleSettingsCustomization::OnGenerateLanguageWidget)
        .InitiallySelectedItem(SelectedOption)
        [
            SNew(STextBlock)
            .Text(this, &FEditorLanguageToggleSettingsCustomization::GetSelectedLanguageText, SelectedCultureProperty)
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
