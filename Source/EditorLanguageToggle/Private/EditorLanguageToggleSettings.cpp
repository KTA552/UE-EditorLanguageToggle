// Copyright (c) 2025 Keita Nakamura
//
// This file is part of the Editor Language Toggle plugin.
#include "EditorLanguageToggleSettings.h"
#include "EditorLanguageToggle.h"

#define LOCTEXT_NAMESPACE "FEditorLanguageToggleModule"

#if WITH_EDITOR
FText UEditorLanguageToggleSettings::GetSectionText() const
{
	return LOCTEXT("EditorLanguageToggleSettingsName", "Editor Language Toggle");
}

FText UEditorLanguageToggleSettings::GetSectionDescription() const
{
	return LOCTEXT("EditorLanguageToggleSettingsDescription", "Settings for the Editor Language Toggle plugin.");
}

void UEditorLanguageToggleSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property == nullptr)
	{
		return;
	}
	const FName PropertyName = PropertyChangedEvent.Property->GetFName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UEditorLanguageToggleSettings, bEnableLanguageToggle) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(UEditorLanguageToggleSettings, ToolbarPosition) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(UEditorLanguageToggleSettings, SourceCulture) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(UEditorLanguageToggleSettings, TargetCulture))
	{
		if (bEnableLanguageToggle)
		{
			FEditorLanguageToggleModule::Get().UnregisterMenu();
			FEditorLanguageToggleModule::Get().RegisterMenu();
		}
		else
		{
			FEditorLanguageToggleModule::Get().UnregisterMenu();
		}
	}
	
	OnSettingChanged().Broadcast(this, PropertyChangedEvent);
}

void UEditorLanguageToggleSettings::PostInitProperties()
{
	Super::PostInitProperties();
	
	// 初回起動時またはSourceCultureが空の場合、現在のエディタ言語を設定
	if (SourceCulture.IsEmpty())
	{
		SourceCulture = FInternationalization::Get().GetCurrentCulture()->GetName();
	}
}
#endif

#undef LOCTEXT_NAMESPACE
