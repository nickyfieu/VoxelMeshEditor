// Copyright Epic Games, Inc. All Rights Reserved.

#include "NFGameUtilities.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "IAssetTools.h"
#include "IAssetTypeActions.h"
#include "AssetToolsModule.h"

#include <EventSystem\CustomEventFactory.h>

#define LOCTEXT_NAMESPACE "FNFGameUtilitiesModule"

void FNFGameUtilitiesModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

    // -----------------------
    // Custom class thumbnails
    // -----------------------
    CustomSlateStyle = MakeShareable(new FSlateStyleSet("NFUtilities"));

    // Content path of this plugin
    FString ContentDir = IPluginManager::Get().FindPlugin("NFGameUtilities")->GetBaseDir();

    // The image we will be trying to load will be in our plugins resources folder
    CustomSlateStyle->SetContentRoot(ContentDir);

    // Create a thumbnail from an icon
    FSlateImageBrush* ThumbnailBrush = new FSlateImageBrush(
        CustomSlateStyle->RootToContentDir(
            TEXT("Resources/CustomEvent"),
            TEXT(".png")
        ), FVector2D(128.f, 128.f)
    );

    if (ThumbnailBrush)
    {
        // Binding the thumbnail to our class (ClassThumbnail.[c++ class name])
        CustomSlateStyle->Set("ClassThumbnail.CustomEvent", ThumbnailBrush);
        CustomSlateStyle->Set("ClassThumbnail.CustomEventFactory", ThumbnailBrush);

        // Register the style
        FSlateStyleRegistry::RegisterSlateStyle(*CustomSlateStyle);
    }

    // ---------------------
    // Custom asset category
    // ---------------------
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    // add custom category
    EAssetTypeCategories::Type CustomEventCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Event System")), FText::FromString("Event System"));
    TSharedPtr<IAssetTypeActions> Action = MakeShareable(new FCustomEventActions(CustomEventCategory));
    AssetTools.RegisterAssetTypeActions(Action.ToSharedRef());
    CustomAssetTypeActions.Add(Action);

}

void FNFGameUtilitiesModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
    
    // Unregister the custom class icon
    FSlateStyleRegistry::UnRegisterSlateStyle(*CustomSlateStyle.ToSharedRef());
    
    // Unregister all the asset types that we registered
    if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
    {
        IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
        for (int32 i = 0; i < CustomAssetTypeActions.Num(); ++i)
        {
            AssetTools.UnregisterAssetTypeActions(CustomAssetTypeActions[i].ToSharedRef());
        }
    }
    CustomAssetTypeActions.Empty();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNFGameUtilitiesModule, NFGameUtilities)