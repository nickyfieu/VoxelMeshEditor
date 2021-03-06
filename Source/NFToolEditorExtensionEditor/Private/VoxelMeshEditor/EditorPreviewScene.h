// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Stats/Stats.h"
#include "InputCoreTypes.h"
#include "PreviewScene.h"
#include "TickableEditorObject.h"

class UAssetViewerSettings;
class UStaticMeshComponent;
class UMaterialInstanceConstant;
class FViewport;
class UPostProcessComponent;

struct FPreviewSceneProfile;

enum EVMEEditMode
{
    EditMode,
    PaintMode,
};

// VME => voxel mesh editor
class NFTOOLEDITOREXTENSIONEDITOR_API FVMEPreviewScene : public FPreviewScene, public FTickableEditorObject
{
public:
    FVMEPreviewScene(ConstructionValues CVS, float InFloorOffset = 0.0f);
    ~FVMEPreviewScene();

    // FPreviewScene Interface
    virtual FLinearColor GetBackgroundColor() const override;
    // End of FPreviewScene Interface

    // FTickableEditorObject Interface
    virtual void Tick(float DeltaTime) override;
    virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
    virtual TStatId GetStatId() const override;
    // End of FTickableEditorObject Interface

    void UpdateScene(FPreviewSceneProfile& Profile, bool bUpdateSkyLight = true, bool bUpdateEnvironment = true, bool bUpdatePostProcessing = true, bool bUpdateDirectionalLight = true);

    const bool HandleInputAxis(FViewport* InViewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, bool bGamepad);
    const bool HandleInputKey(FViewport* InViewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed, bool bGamepad);

    void SetSkyRotation(float Rotation);
    void SetFloorVisibility(bool bVisible, bool bDirect = false);
    void SetEnvironmentVisibility(bool bVisible, bool bDirect = false);
    void SetFloorOffset(float FloorOffset);
    void SetProfileIndex(int32 ProfileIndex);

    const UStaticMeshComponent* GetFloor() const;
    const float GetSkyRotation() const;
    const int32 GetCurrentProfileIndex() const;
    const bool IsUsingPostProcessing() const;

protected:

    /** Bind all our command binding to handlers here */
    void BindCommands();

    /** Handles refreshing the scene when settings got changed */
    void OnAssetViewerSettingsChanged(const FName& InPropertyName);

    /** Line traces from the camera in the direction of the mouse world location */
    void LineTraceVoxelMeshEditor(bool bRemoveVoxels);

public:
    // hacks for now
    class FVMEEditorViewportClient* ViewportClient;
    FIntPoint MousePos;
    EVMEEditMode EditMode = EVMEEditMode::EditMode;

    TObjectPtr<class AVoxelMeshEditorGridActor> GridActor;
    TObjectPtr<class AVoxelMeshEditorActor> MeshEditorActor;

protected:
    bool bRotateLighting;
    bool bSkyChanged;
    bool bPostProcessing;
    float CurrentRotationSpeed;
    float PreviousRotation;
    int32 CurrentProfile;

    UStaticMeshComponent* SkySphere;
    UStaticMeshComponent* Floor;
    UPostProcessComponent* PostProcessComponent;
    UMaterialInstanceConstant* SkySphereMaterialInstance;
    UAssetViewerSettings* DefaultSettings;

    /** Command list for input handling */
    TSharedPtr<FUICommandList> CommandList;

    /** Delegate handle used to refresh the scene when settings change */
    FDelegateHandle RefreshDelegate;
};
