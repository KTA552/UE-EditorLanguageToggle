// Copyright (c) 2025 Keita Nakamura
//
// This file is part of the Editor Language Toggle plugin.
#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "EditorLanguageToggleSettings.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Framework/Views/ITypedTableView.h"

class IDetailLayoutBuilder;
class IPropertyHandle;
class SWidget;
class IPropertyTypeCustomizationUtils;

class FEditorLanguageToggleSettingsCustomization : public IDetailCustomization
{
public:
    static TSharedRef<IDetailCustomization> MakeInstance();

    // IDetailCustomization interface
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
    // ドロップダウン用のオプション配列
    TArray<TSharedPtr<FEditorLanguageOption>> LanguageComboOptions;

    // コールバック関数
    void OnLanguageSelectionChanged(TSharedPtr<FEditorLanguageOption> NewSelection, ESelectInfo::Type SelectInfo, TSharedRef<IPropertyHandle> PropertyHandle);
    TSharedRef<SWidget> OnGenerateLanguageWidget(TSharedPtr<FEditorLanguageOption> InOption);
    FText GetSelectedLanguageText(TSharedRef<IPropertyHandle> PropertyHandle) const;
};
