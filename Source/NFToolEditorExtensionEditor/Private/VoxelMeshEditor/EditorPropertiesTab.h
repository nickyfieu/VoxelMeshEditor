// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../NFToolTabBase.h"

/**
 * Handles the spawning of the mesh editor editor window.
 */
class VMEEditorPropertiesTab : public FNFToolTabBase, public INFToolsModuleInterface
{
public:
    virtual ~VMEEditorPropertiesTab();
    // FNFToolTabBase Interface
    virtual void OnStartupModule() override;
    virtual void OnShutdownModule() override;
    virtual void Initialize() override;
    virtual TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& SpawnTabArgs) override;
    // End of FNFToolTabBase Interface

    // INFToolsModuleInterface Interface
    virtual void AddModuleListeners() override;
    // End of INFToolsModuleInterface Interface

    void AttemptToSetPropertyTabReferences();

public:
    bool bSpawnedTab = false;
    TWeakPtr<class VoxelMeshEditor> Owner;

private:
    TWeakPtr<class SVMEEditorPropertiesTab> PropertiesTab;
};
