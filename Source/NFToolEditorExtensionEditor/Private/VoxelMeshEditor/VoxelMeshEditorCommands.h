// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/InputChord.h"
#include "EditorStyleSet.h"
#include "Framework/Commands/Commands.h"

/**
 * Holds all the voxel mesh editor commands 
 */
class NFTOOLEDITOREXTENSIONEDITOR_API FVoxelMeshEditorCommands : public TCommands<FVoxelMeshEditorCommands>
{
public:
    FVoxelMeshEditorCommands();

    // TCommands<> interface
    virtual void RegisterCommands() override;
    // End of TCommands<> interface

public:
    /** Add Command Info member variables here */

    /** Saves the current voxel mesh data */
    TSharedPtr<FUICommandInfo> Save;

    /** Load the selected voxel mesh data */
    TSharedPtr<FUICommandInfo> Load;

    /** Exports the current voxel mesh to a static mesh */
    TSharedPtr<FUICommandInfo> Export;
};
