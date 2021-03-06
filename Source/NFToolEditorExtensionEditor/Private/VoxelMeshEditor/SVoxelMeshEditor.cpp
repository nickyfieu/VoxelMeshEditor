// Copyright © 2021 Nicky Fieu, All Rights Reserved.


#include "SVoxelMeshEditor.h"
#include "VoxelMeshEditor.h"
#include "../NFToolEditorModule.h"
#include "StatusBarSubsystem.h"
#include "VoxelMeshEditorCommands.h"
#include "SVoxelMeshEditorToolBar.h"
#include "EditorWindow.h"
#include "SEditorWindow.h"
#include "EditorViewportClient.h"
#include "EditorPreviewScene.h"
#include "Functionality/VoxelMeshEditorActor.h"
#include "SVoxelMeshEditorToolBar.h"

#define LOCTEXT_NAMESPACE "VoxelMeshEditor"

void SVoxelMeshEditor::Construct(const FArguments& InArgs, const TSharedRef<SDockTab>& OwnerTab)
{
    OwningObject = InArgs._OwningObject;
    LayoutName = TEXT("VoxelMeshEditor_Layout_v1.0");

    CommandList = MakeShareable(new FUICommandList);
    FVoxelMeshEditorCommands::Register();
    Initialize(OwnerTab);

    /**
     * Initialize VoxelMeshEditorActor references 
     */
    const FVMEPreviewScene* PreviewScene = OwningObject->ToolTab->EditorWindow.Pin()->GetEditorViewportClient().GetPreviewScene();
    VoxelMeshEditorActor = PreviewScene->MeshEditorActor.Get();
    VoxelMeshEditorGridActor = PreviewScene->GridActor.Get();

    BindCommands();
    RegisterMenus();
}

void SVoxelMeshEditor::Initialize(const TSharedRef<SDockTab>& OwnerTab)
{
    // Checking if we can load the status bar module otherwise there could be problems when adding teh statusbar to our own window
    FModuleManager::Get().LoadModuleChecked("StatusBar");

    // Checking if GEditor is valid for constructing the status bar widget
    if (!GEditor)
    {
        UE_LOG(LogNFToolEditor, Error, TEXT("SVoxelMeshEditor::Initialize => GEditor was invalid!"));
        return;
    }

    // Initializing the voxel mesh editor tab manager
    TabManager = FGlobalTabmanager::Get()->NewTabManager(OwnerTab);
    TSharedRef<FWorkspaceItem> WindowPullDownMenuGroup = TabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WindowPullDownMenuHook", "Local Window Tabs"));
    TabManager->SetAllowWindowMenuBar(true);

    /**
     * Registering the tab spawners of all local child tabs.
     * This needed to be done here for the window pull down menu.
     * When the tab spawners would already be created before this initialize function.
     * The tabs would not wpawn in the correct layout + not be visible in the window pull down menu.
     */
    if (VoxelMeshEditor* VME = OwningObject.Get())
    {
        for (int32 i = 0; i < VME->InternalPanelPtrs.Num(); ++i)
        {
            FNFToolTabBase* ToolTab = VME->InternalPanelPtrs[i].Get();

            FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
                ToolTab->GetTabName(),
                FOnSpawnTab::CreateRaw(ToolTab, &FNFToolTabBase::SpawnTab)
            )  .SetDisplayName(ToolTab->GetTabDisplayName())
               .SetTooltipText(ToolTab->GetTabToolTip())
               .SetGroup(WindowPullDownMenuGroup)
               .SetMenuType(ETabSpawnerMenuType::Enabled);
        }
    }

    // Create this tabs layout
    const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout(LayoutName)
        ->AddArea
        (
            FTabManager::NewPrimaryArea()
            ->SetOrientation(Orient_Horizontal)
            ->Split
            (
                FTabManager::NewStack()
                ->AddTab(FVMETabIds::EditorViewport, ETabState::OpenedTab)
                ->SetHideTabWell(true)
                ->SetSizeCoefficient(0.6f)
            )
            ->Split
            (
                FTabManager::NewStack()
                ->AddTab(FVMETabIds::EditorProperties, ETabState::OpenedTab)
                ->SetHideTabWell(false)
                ->SetSizeCoefficient(0.4f)
            )
        );

    // Create and initialize this tabs menu bar.
    FMenuBarBuilder WindowMenuBarBuilder = FMenuBarBuilder(TSharedPtr<FUICommandList>());

    WindowMenuBarBuilder.AddPullDownMenu(
        LOCTEXT("VMEWindowPullDownMenu", "Window"),
        FText::GetEmpty(),
        FNewMenuDelegate::CreateStatic(&SVoxelMeshEditor::FillWindowPullDownMenu, TabManager),
        "VMEWindowHook" // use this hook to add in front or behind in the menu bar 
    );

    TSharedRef<SWidget> MenuBarWidget = WindowMenuBarBuilder.MakeWidget();

    // All extra UI + layout come together here for the final window layout of this tool window.
    ChildSlot
    [
        SNew(SVerticalBox) +
        SVerticalBox::Slot()
        .Padding(FMargin(0.f,0.f,0.f,2.f))
        .AutoHeight()
        [
            SNew(SVoxelMeshEditorToolBar, CommandList.ToSharedRef())
        ] +
        SVerticalBox::Slot()
        .Padding(FMargin(4.f,2.f,4.f,2.f))
        .FillHeight(1.f)
        [
            TabManager->RestoreFrom(Layout, TSharedPtr<SWindow>()).ToSharedRef()
        ] +
        SVerticalBox::Slot()
        .Padding(FMargin(0.f, 2.f, 0.f, 0.f))
        .AutoHeight()
        [   // This is new to UE5 so if this tool needed to be built for 4.x this would need to be removed
            GEditor->GetEditorSubsystem<UStatusBarSubsystem>()->MakeStatusBarWidget(TEXT("VMEStatusBar"), OwnerTab)
        ]
    ];

    // Tell the tab manager about the menu bar widget since this is not something that can be added trough the childslot.
    TabManager->SetMenuMultiBox(WindowMenuBarBuilder.GetMultiBox(), MenuBarWidget);
}

void SVoxelMeshEditor::BindCommands()
{
    const FVoxelMeshEditorCommands& Commands = FVoxelMeshEditorCommands::Get();

    CommandList->MapAction(
        Commands.Save,
        FExecuteAction::CreateUObject(VoxelMeshEditorActor, &AVoxelMeshEditorActor::OnClickedSaveVoxelMesh)
    );

    CommandList->MapAction(
        Commands.Load,
        FExecuteAction::CreateUObject(VoxelMeshEditorActor, &AVoxelMeshEditorActor::OnClickedLoadVoxelMesh)
    );

    CommandList->MapAction(
        Commands.Export,
        FExecuteAction::CreateUObject(VoxelMeshEditorActor, &AVoxelMeshEditorActor::OnClickedExportVoxelMesh)
    );
}

void SVoxelMeshEditor::RegisterMenus()
{

}

void SVoxelMeshEditor::FillWindowPullDownMenu(FMenuBuilder& MenuBuilder, const TSharedPtr<FTabManager> TabManager)
{
    if (!TabManager.IsValid())
    {
        return;
    }

    // This function will fill the given MenuBuilder with all local tabs from the TabManager
    TabManager->PopulateLocalTabSpawnerMenu(MenuBuilder);
}

#undef LOCTEXT_NAMESPACE