// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VoxelMeshEditor.h"
#include <Widgets/Docking/SDockTab.h>
#include <Framework/Application/SlateApplication.h>


// VME => voxel mesh editor
/**
 * Main UI layer
 * Handles spawning & destroying of the next layer of widgets & elements
 */
class NFTOOLEDITOREXTENSIONEDITOR_API SVoxelMeshEditor : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SVoxelMeshEditor)
    {}
    SLATE_ARGUMENT(TSharedPtr<VoxelMeshEditor>, OwningObject)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, const TSharedRef<SDockTab>& OwnerTab);
    void Initialize(const TSharedRef<SDockTab>& OwnerTab);

protected:
    /** Fills the "window" pull down menu with all local tabs */
    static void FillWindowPullDownMenu(FMenuBuilder& MenuBuilder, const TSharedPtr<FTabManager> TabManager);

private:
    /** Binds the commands to actions for the tab tool pannel */
    void BindCommands();

    /** Registers the menus asociated with the tab tool pannel */
    void RegisterMenus();

public:
    class AVoxelMeshEditorActor* VoxelMeshEditorActor;
    class AVoxelMeshEditorGridActor* VoxelMeshEditorGridActor;

protected:
    TSharedPtr<VoxelMeshEditor> OwningObject;

    FName LayoutName;

private:
    /** Holds the command list that manages the tab tool commands. */
    TSharedPtr<FUICommandList> CommandList;

    /** Holds the tab manager that manager the local voxel mesh editor tabs. */
    TSharedPtr<FTabManager> TabManager;

};
