// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../NFToolTabBase.h"

/**
 * Voxel mesh editor tab identifiers
 */
struct FVMETabIds
{
    static const FName MainWindow;
    static const FName EditorViewport;
    static const FName EditorProperties;
};

/**
 * Voxel mesh editor "module" start & end point 
 * Handles spawning of the SVoxelMeshEditor tab
 */
class VoxelMeshEditor : public FNFToolTabBase, public INFToolsModuleInterface
{
public:
    virtual ~VoxelMeshEditor();
    // FNFToolTabBase Interface
    virtual void OnStartupModule() override;
    virtual void OnShutdownModule() override;
    virtual void Initialize() override;
    virtual TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& SpawnTabArgs) override;
    // End of FNFToolTabBase Interface

    // INFToolsModuleInterface Interface
    virtual void AddModuleListeners() override;
    // End of INFToolsModuleInterface Interface

public:
    /** Holds shared pointers to all local window tabs. */
    TArray<TSharedPtr<FNFToolTabBase>> InternalPanelPtrs;

    TSharedPtr<class VMEEditorWindow> ToolTab;
    TSharedPtr<class VMEEditorPropertiesTab> PropertiesTab;

protected:
    TArray<TSharedPtr<class IAssetTypeActions>> CreatedAssetTypeActions;
};
