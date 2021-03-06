// Copyright © 2021 Nicky Fieu, All Rights Reserved.


#include "EditorCommands.h"

#define LOCTEXT_NAMESPACE "VoxelMeshEditor"

// Register commands using UI_COMMAND(...) & adding a public TSharedPtr<FUICommandInfo> member variable
void FVMEEditorCommands::RegisterCommands()
{
    ///UI_COMMAND(DebugPrintCommand, "Debug Button", "Prints the debug message [Testing123] to the console", EUserInterfaceActionType::ToggleButton, FInputChord());
    
    UI_COMMAND(Perspective  , "Perspective" , "Switches the viewport to perspective view"   , EUserInterfaceActionType::RadioButton, FInputChord(EModifierKey::Alt, EKeys::G));
    UI_COMMAND(Front        , "Front"       , "Switches the viewport to front view"         , EUserInterfaceActionType::RadioButton, FInputChord(EModifierKey::Alt, EKeys::H));
    UI_COMMAND(Back         , "Back"        , "Switches the viewport to back view"          , EUserInterfaceActionType::RadioButton, FInputChord(EModifierKey::Shift, EKeys::H));
    UI_COMMAND(Top          , "Top"         , "Switches the viewport to top view"           , EUserInterfaceActionType::RadioButton, FInputChord(EModifierKey::Alt, EKeys::J));
    UI_COMMAND(Bottom       , "Bottom"      , "Switches the viewport to bottom view"        , EUserInterfaceActionType::RadioButton, FInputChord(EModifierKey::Shift, EKeys::J));
    UI_COMMAND(Left         , "Left"        , "Switches the viewport to left view"          , EUserInterfaceActionType::RadioButton, FInputChord(EModifierKey::Alt, EKeys::K));
    UI_COMMAND(Right        , "Right"       , "Switches the viewport to right view"         , EUserInterfaceActionType::RadioButton, FInputChord(EModifierKey::Shift, EKeys::K));

    UI_COMMAND(WireframeMode      , "Brush Wireframe View Mode"         , "Renders the scene in brush wireframe"                                    , EUserInterfaceActionType::RadioButton, FInputChord(EModifierKey::Alt, EKeys::One));
    UI_COMMAND(UnlitMode          , "Unlit View Mode"                   , "Renders the scene with no lights"                                        , EUserInterfaceActionType::RadioButton, FInputChord(EModifierKey::Alt, EKeys::Two));
    UI_COMMAND(LitMode            , "Lit View Mode"                     , "Renders the scene with normal lighting"                                  , EUserInterfaceActionType::RadioButton, FInputChord(EModifierKey::Alt, EKeys::Three));
    UI_COMMAND(DetailLightingMode , "Detail Lighting View Mode"         , "Renders the scene with detailed lighting only"                           , EUserInterfaceActionType::RadioButton, FInputChord(EModifierKey::Alt, EKeys::Four));
    UI_COMMAND(LightingOnlyMode   , "Lighting Only View Mode"           , "Renders the scene with lights only, no textures"                         , EUserInterfaceActionType::RadioButton, FInputChord(EModifierKey::Alt, EKeys::Five));
    UI_COMMAND(VisibilityCollision, "Visibility Collision View Mode"    , "Renders the scene with object visibility collisions only, no textures"   , EUserInterfaceActionType::RadioButton, FInputChord(EModifierKey::Alt, EKeys::Six));

    UI_COMMAND(EditMode     , "Voxel Mesh Editor Edit Mode"     , "Shift+LMB to Add, Shift+RMB to Remove."          , EUserInterfaceActionType::ToggleButton, FInputChord(EModifierKey::Alt, EKeys::W));
    UI_COMMAND(PaintMode    , "Voxel Mesh Editor Paint Mode"    , "Shift+LMB to paint the current selected color."  , EUserInterfaceActionType::ToggleButton, FInputChord(EModifierKey::Alt, EKeys::E));
}

#undef LOCTEXT_NAMESPACE
