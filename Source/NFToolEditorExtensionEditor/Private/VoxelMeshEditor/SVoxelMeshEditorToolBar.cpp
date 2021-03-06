// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#include "SVoxelMeshEditorToolBar.h"
#include "VoxelMeshEditorCommands.h"
#include "NFToolEditorModule.h"
#include "SlateOptMacros.h"
#include "ToolMenus.h"
#include "ToolMenuEntry.h"
#include "VoxelMeshEditorStyle.h"
#include <Styling/ToolBarStyle.h>

#define LOCTEXT_NAMESPACE "VoxelMeshEditor"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SVoxelMeshEditorToolBar::Construct(const FArguments& InArgs, const TSharedRef<FUICommandList>& CommandList)
{
    /* Create the toolbar */
    UToolMenu* Toolbar = UToolMenus::Get()->RegisterMenu("VoxelMeshEditor.VoxelMeshEditorToolBar.ActionToolBar", NAME_None, EMultiBoxType::SlimHorizontalToolBar);
    {   /* Create the toolbar layout */
        FToolMenuSection& Section = Toolbar->AddSection("VMEToolBar");
        Section.AddEntry(FToolMenuEntry::InitToolBarButton(
            FVoxelMeshEditorCommands::Get().Save,
            LOCTEXT("Save", "Save"),
            LOCTEXT("SaveToolTip", "Save to a voxel mesh data file."),
            FSlateIcon(FVoxelMeshEditorStyle::GetInstance().GetStyleSetName(), "VoxelMeshEditor.Save"),
            NAME_None,
            FName("Save")
        ));

        Section.AddEntry(FToolMenuEntry::InitToolBarButton(
            FVoxelMeshEditorCommands::Get().Load,
            LOCTEXT("Load", "Load"),
            LOCTEXT("LoadToolTip", "Load a voxel mesh data file."),
            FSlateIcon(FVoxelMeshEditorStyle::GetInstance().GetStyleSetName(), "VoxelMeshEditor.Load"),
            NAME_None,
            FName("Load")
        ));

        Section.AddEntry(FToolMenuEntry::InitToolBarButton(
            FVoxelMeshEditorCommands::Get().Export,
            TAttribute<FText>(),
            LOCTEXT("ExportToolTip", "Export the current voxel mesh to a static mesh."),
            FSlateIcon(FVoxelMeshEditorStyle::GetInstance().GetStyleSetName(), "VoxelMeshEditor.Export"),
            NAME_None,
            FName("Export")
        ));
    }

    ChildSlot
    [
        SNew(SOverlay) +
        SOverlay::Slot()
        [
            SNew(SImage)
            .Image(&FAppStyle::Get().GetWidgetStyle<FToolBarStyle>("SlimToolBar").BackgroundBrush)
        ] +
        SOverlay::Slot()
        [
            SNew(SHorizontalBox) +
            SHorizontalBox::Slot()
            .Padding(4.f, 0.0f, 0.0f, 0.0f)
            .AutoWidth()
            [
                UToolMenus::Get()->GenerateWidget("VoxelMeshEditor.VoxelMeshEditorToolBar.ActionToolBar", FToolMenuContext(CommandList))
            ]
        ]
    ];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE