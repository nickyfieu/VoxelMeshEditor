// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#include "TabTool/TabToolStyle.h"
#include <Framework/Application/SlateApplication.h>
#include <Styling/SlateStyleRegistry.h>
#include <Styling/SlateStyleMacros.h>

#define LOCTEXT_NAMESPACE "TabTool"

FName FTabToolStyle::StyleName("TabToolStyle");
TSharedPtr<FTabToolStyle> FTabToolStyle::Instance(nullptr);

FTabToolStyle::FTabToolStyle()
    : FSlateStyleSet(StyleName)
{
    SetContentRoot(FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("NFGameUtilities"), TEXT("Resources"), TEXT("Slate")));
    SetCoreContentRoot( FPaths::EngineContentDir() / TEXT("Slate") );

    Set("TabTool.TestSaveImage", new IMAGE_BRUSH(TEXT("Images/TestSaveImage"), Icon60x30));
    
}

void FTabToolStyle::Initialize()
{
    // Making sure to remove any old instance if there ever were to be one
    Shutdown();
    Instance = MakeShareable(new FTabToolStyle());
    FSlateStyleRegistry::RegisterSlateStyle(*Instance.Get());
}

void FTabToolStyle::Shutdown()
{
    if (!Instance.IsValid()) return;
    
    FSlateStyleRegistry::UnRegisterSlateStyle(*Instance.Get());
    Instance = nullptr;
}

const FTabToolStyle& FTabToolStyle::GetInstance()
{
    return *Instance.Get();
}

#undef LOCTEXT_NAMESPACE 
