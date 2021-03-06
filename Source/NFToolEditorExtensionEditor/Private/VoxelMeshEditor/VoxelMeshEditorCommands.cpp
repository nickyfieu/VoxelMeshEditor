// Copyright © 2021 Nicky Fieu, All Rights Reserved.


#include "VoxelMeshEditorCommands.h"

#define LOCTEXT_NAMESPACE "VoxelMeshEditor"

FVoxelMeshEditorCommands::FVoxelMeshEditorCommands()
    : TCommands<FVoxelMeshEditorCommands>(
        "VoxelMeshEditor",
        NSLOCTEXT("Contexts", "VoxelMeshEditor", "NFToolEditor VoxelMeshEditor Editor Tab Commands"),
        NAME_None,
        FEditorStyle::GetStyleSetName()
    )
{

}

// Register commands using UI_COMMAND(...) & adding a public TSharedPtr<FUICommandInfo> member variable
void FVoxelMeshEditorCommands::RegisterCommands()
{
    UI_COMMAND(Save     , "Save"    , "Save to a voxel mesh data file."                 , EUserInterfaceActionType::Button, FInputChord(EModifierKey::Control, EKeys::S));
    UI_COMMAND(Load     , "Load"    , "Load a voxel mesh data file."                    , EUserInterfaceActionType::Button, FInputChord(EModifierKey::Control, EKeys::L));
    UI_COMMAND(Export   , "Export"  , "Export the current voxel mesh to a static mesh." , EUserInterfaceActionType::Button, FInputChord(EModifierKey::Control, EKeys::E));
}

#undef LOCTEXT_NAMESPACE
