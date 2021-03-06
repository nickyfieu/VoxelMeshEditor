// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/InputChord.h"
#include "EditorStyleSet.h"
#include "Framework/Commands/Commands.h"

/**
 * Holds all the voxel mesh editor commands 
 */
class FVMEEditorCommands : public TCommands<FVMEEditorCommands>
{
public:
    FVMEEditorCommands()
        : TCommands<FVMEEditorCommands>(
            "VMEEditorCommands",
            NSLOCTEXT("Contexts", "VMEEditorCommands", "NFToolEditor VoxelMeshEditor Editor Commands"),
            NAME_None,
            FEditorStyle::GetStyleSetName()
        )
    {
    }

    // TCommands<> interface
    virtual void RegisterCommands() override;
    // End of TCommands<> interface

public:
    /** Add Command Info member variables here */
    
    /** Changes the viewport to perspective view */
    TSharedPtr<FUICommandInfo> Perspective;

    /** Changes the viewport to top view */
    TSharedPtr<FUICommandInfo> Top;

    /** Changes the viewport to bottom view */
    TSharedPtr<FUICommandInfo> Bottom;

    /** Changes the viewport to left view */
    TSharedPtr<FUICommandInfo> Left;

    /** Changes the viewport to right view */
    TSharedPtr<FUICommandInfo> Right;

    /** Changes the viewport to front view */
    TSharedPtr<FUICommandInfo> Front;

    /** Changes the viewport to back view */
    TSharedPtr<FUICommandInfo> Back;

    /** Changes the viewport to wire frame visualization */
    TSharedPtr<FUICommandInfo> WireframeMode;

    /** Changes the viewport to unlit visualization */
    TSharedPtr<FUICommandInfo> UnlitMode;

    /** Changes the viewport to lit visualization */
    TSharedPtr<FUICommandInfo> LitMode;

    /** Changes the viewport to detailed light only visualization */
    TSharedPtr<FUICommandInfo> DetailLightingMode;

    /** Changes the viewport to only light no texture visualization */
    TSharedPtr<FUICommandInfo> LightingOnlyMode;

    /** Changes the viewport to only light no texture visualization */
    TSharedPtr<FUICommandInfo> VisibilityCollision;

    /** Changes the viewport to perspective view */
    TSharedPtr<FUICommandInfo> EditMode;

    /** Changes the viewport to perspective view */
    TSharedPtr<FUICommandInfo> PaintMode;

};
