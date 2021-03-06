// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorPreviewScene.h"

// VME => voxel mesh editor
class NFTOOLEDITOREXTENSIONEDITOR_API FVMEEditorViewportClient : public FEditorViewportClient
{
public:
    FVMEEditorViewportClient(class FVMEPreviewScene* InPreviewScene, const TSharedRef<class SVMEEditorWindow>& InVoxelMeshEditor);

    // FEditorViewportClient interface
    virtual void Tick(float DeltaSeconds) override;
    virtual void Draw(FViewport* InViewport, FCanvas* Canvas) override;

    virtual bool ShouldOrbitCamera() const override;
    virtual bool InputKey(FViewport* InViewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed = 1.f, bool bGamepad = false) override;
    virtual bool InputAxis(FViewport* InViewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad) override;
    virtual void MouseMove(FViewport* InViewport, int32 x, int32 y) override;

    virtual FLinearColor GetBackgroundColor() const override;
    // End of FEditorViewportClient interface

    const FEditorCameraController* GetCameraController() const;
    class FVMEPreviewScene* GetPreviewScene() const { return PreviewScene; };

    void SetEditMode(EVMEEditMode NewMode);
    bool CanSetEditMode(EVMEEditMode NewMode) const;
    bool IsEditModeActive(EVMEEditMode NewMode) const;

private:
    class FVMEPreviewScene* PreviewScene;
};