// Copyright © 2021 Nicky Fieu, All Rights Reserved.


#include "EditorViewportClient.h"
#include "EditorPreviewScene.h"
#include "SEditorWindow.h"
#include "AssetEditorModeManager.h"
#include "../NFToolEditorModule.h"
#include "CameraController.h"

#define LOCTEXT_NAMESPACE "VoxelMeshEditor"

FVMEEditorViewportClient::FVMEEditorViewportClient(FVMEPreviewScene* InPreviewScene, const TSharedRef<SVMEEditorWindow>& InVoxelMeshEditorWindow)
    : FEditorViewportClient(nullptr, InPreviewScene, StaticCastSharedRef<SEditorViewport>(InVoxelMeshEditorWindow))
{
    ((FAssetEditorModeManager*)ModeTools.Get())->SetPreviewScene(PreviewScene);

    CameraSpeedSetting = 4;
    CameraSpeedScalar = 1.0f;
    ViewportType = LVT_Perspective;
    ViewFOV = EditorViewportDefs::DefaultPerspectiveFOVAngle;
    FOVAngle = EditorViewportDefs::DefaultPerspectiveFOVAngle;
    AspectRatio = 1.777777f;
    CameraController = new FEditorCameraController();
    CameraUserImpulseData = new FCameraControllerUserImpulseData();

    SetViewLocation(FVector(-800.f, 0.f, 400.f));
    SetViewRotation(FRotator::ZeroRotator);
    SetRealtime(true);

    // Initialize the Cursor visibility struct
    RequiredCursorVisibiltyAndAppearance.bSoftwareCursorVisible = false;
    RequiredCursorVisibiltyAndAppearance.bHardwareCursorVisible = true;
    RequiredCursorVisibiltyAndAppearance.bDontResetCursor = false;
    RequiredCursorVisibiltyAndAppearance.bOverrideAppearance = false;
    RequiredCursorVisibiltyAndAppearance.RequiredCursor = EMouseCursor::Default;

    // Setup defaults for the common draw helper.
    DrawHelper.bDrawPivot = false;
    DrawHelper.bDrawWorldBox = false;
    DrawHelper.bDrawKillZ = false;
    DrawHelper.bDrawGrid = false; // disable this since we rely on the show flags
    DrawHelper.GridColorAxis = FColor(160, 160, 160);
    DrawHelper.GridColorMajor = FColor(144, 144, 144);
    DrawHelper.GridColorMinor = FColor(128, 128, 128);

    // Most editor viewports do not want motion blur.
    EngineShowFlags.MotionBlur = 0;
    EngineShowFlags.DisableAdvancedFeatures();
    EngineShowFlags.SetSnap(0);
    EngineShowFlags.SetSeparateTranslucency(true);

    EnableCameraLock(false);

    InPreviewScene->ViewportClient = this;
    PreviewScene = InPreviewScene;
}

void FVMEEditorViewportClient::Tick(float DeltaSeconds)
{
    FEditorViewportClient::Tick(DeltaSeconds);

    // Tick the preview scene world.
    if (!GIntraFrameDebuggingGameThread)
    {
        PreviewScene->GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
    }
}

void FVMEEditorViewportClient::Draw(FViewport* InViewport, FCanvas* Canvas)
{
    FEditorViewportClient::Draw(InViewport, Canvas);
}

bool FVMEEditorViewportClient::ShouldOrbitCamera() const
{
    return bUsingOrbitCamera;
}

bool FVMEEditorViewportClient::InputKey(FViewport* InViewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed, bool bGamepad)
{
    bool bHandled = FEditorViewportClient::InputKey(InViewport, ControllerId, Key, Event, AmountDepressed, false);

    // Handle other input 
    bHandled |= PreviewScene->HandleInputKey(InViewport, ControllerId, Key, Event, AmountDepressed, bGamepad);

    return bHandled;
}

bool FVMEEditorViewportClient::InputAxis(FViewport* InViewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
    bool bResult = true;

    if (!bDisableInput)
    {
        bResult = PreviewScene->HandleInputAxis(InViewport, ControllerId, Key, Delta, DeltaTime, bGamepad);

        if (bResult)
        {
            Invalidate();
        }
        else
        {
            bResult = FEditorViewportClient::InputAxis(InViewport, ControllerId, Key, Delta, DeltaTime, NumSamples, bGamepad);
        }
    }

    return bResult;
}

void FVMEEditorViewportClient::MouseMove(FViewport* InViewport, int32 x, int32 y)
{
    PreviewScene->MousePos.X = x;
    PreviewScene->MousePos.Y = y;
}

FLinearColor FVMEEditorViewportClient::GetBackgroundColor() const
{
    if (PreviewScene)
    {
        return PreviewScene->GetBackgroundColor();
    }
    else
    {
        return FLinearColor::Black;
    }
}

const FEditorCameraController* FVMEEditorViewportClient::GetCameraController() const
{
    return CameraController;
}

void FVMEEditorViewportClient::SetEditMode(EVMEEditMode NewMode)
{
    if (!PreviewScene)
    {
        return;
    }

    PreviewScene->EditMode = NewMode;
}

bool FVMEEditorViewportClient::CanSetEditMode(EVMEEditMode NewMode) const
{
    if (!PreviewScene)
    {
        return false;
    }

    return PreviewScene->EditMode != NewMode;
}

bool FVMEEditorViewportClient::IsEditModeActive(EVMEEditMode NewMode) const
{
    if (!PreviewScene)
    {
        return false;
    }

    return PreviewScene->EditMode == NewMode;
}