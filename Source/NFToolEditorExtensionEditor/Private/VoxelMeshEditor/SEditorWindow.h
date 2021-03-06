// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <SEditorViewport.h>
#include <AdvancedPreviewScene.h>
#include <Framework/Application/SlateApplication.h>

// VME => voxel mesh editor
class NFTOOLEDITOREXTENSIONEDITOR_API SVMEEditorWindow : public SEditorViewport
{
public:
    SLATE_BEGIN_ARGS(SVMEEditorWindow)
    {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    ~SVMEEditorWindow();

    class FVMEEditorViewportClient& GetEditorViewportClient() const;

protected:
    // SEditorViewport interface
    virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
    virtual TSharedPtr<SWidget> MakeViewportToolbar() override;
    // End of SEditorViewport interface
private:
    /** Mesh editor viewport client */
    TSharedPtr<class FVMEEditorViewportClient> EditorViewportClient;

    /** Preview Scene - uses advanced preview settings */
    TSharedPtr<class FVMEPreviewScene> PreviewScene;

    class USkyLightComponent* Skylight;

    class USkyAtmosphereComponent* SkyAtmosphere;

    /** Handle to the registered OnPreviewFeatureLevelChanged delegate. */
    FDelegateHandle PreviewFeatureLevelChangedHandle;
};
