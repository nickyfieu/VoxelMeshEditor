// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Editor/UnrealEd/Public/SViewportToolBar.h>

/**
 * A viewport toolbar widget that is placed in a voxel mesh editor editor window
 */
class SVMEEditorViewportToolBar : public SViewportToolBar
{
public:
    SLATE_BEGIN_ARGS(SVMEEditorViewportToolBar)
    {
    }
    SLATE_ARGUMENT(TSharedPtr<class SVMEEditorWindow>, Viewport)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

protected:
    /** Bind all our command binding to handlers here */
    void BindCommands();

private:
    TSharedRef<SWidget> GenerateViewportOptionsMenu();
    void FillViewportOptions(class UToolMenu* Menu);
    
    TSharedRef<SWidget> GenerateFOVMenu();
    float OnGetFOVValue() const;
    void OnFOVValueChanged(float NewValue);

    TSharedRef<SWidget> GenerateFarViewPlaneMenu();
    float OnGetFarViewPlaneValue() const;
    void OnFarViewPlaneValueChanged(float NewValue);

    FText GetCameraMenuLabel() const;
    const FSlateBrush* GetCameraMenuLabelIcon() const;
    TSharedRef<SWidget> GenerateCameraSettings();
    void FillCameraSettings(UToolMenu* Menu);

    FText GetViewModeMenuLabel() const;
    const FSlateBrush* GetViewModeMenuIcon() const;
    TSharedRef<SWidget> GenerateViewModeMenu();
    void FillViewModeMenu(UToolMenu* Menu);

    TSharedRef<SWidget> GenerateEditorModeMenu();

private:
    /** The viewport that we are in */
    TWeakPtr<class SVMEEditorWindow> Viewport;

    TSharedPtr<FUICommandList> CommandList;
};
