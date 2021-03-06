// Copyright © 2021 Nicky Fieu, All Rights Reserved.


#include "EditorWindow.h"
#include "VoxelMeshEditor.h"
#include "SEditorWindow.h"
#include "EditorPropertiesTab.h"
#include "NFToolEditorModule.h"

#define LOCTEXT_NAMESPACE "VoxelMeshEditor"

const FName FVMETabIds::EditorViewport = "VoxelMeshEditorEditorPanel";


void VMEEditorWindow::AddModuleListeners()
{

}

VMEEditorWindow::~VMEEditorWindow()
{

}

void VMEEditorWindow::OnStartupModule()
{
    // Ignoring inherited functionality
    // Since this would not allow for the correct spawning behavior to happen
    // Super::OnStartupModule();
    INFToolsModuleInterface::StartupModule();
    Initialize();
}

void VMEEditorWindow::OnShutdownModule()
{
    Super::OnShutdownModule();
    INFToolsModuleInterface::ShutdownModule();
    FGlobalTabmanager::Get()->UnregisterTabSpawner(TabName);
}

void VMEEditorWindow::Initialize()
{
    TabName = FVMETabIds::EditorViewport;
    TabDisplayName = FText::FromString("Editor Viewport");
    TabToolTip = FText::FromString("The viewport displaying the voxel mesh currently beeing edited.");
}

TSharedRef<SDockTab> VMEEditorWindow::SpawnTab(const FSpawnTabArgs& SpawnTabArgs)
{
    TSharedPtr<SDockTab> EditorTab;
    SAssignNew(EditorTab, SDockTab)
        .TabRole(ETabRole::PanelTab);

    TSharedPtr<SVMEEditorWindow> Editor;
    SAssignNew(Editor, SVMEEditorWindow);
    EditorWindow = Editor;

    EditorTab.Get()->SetContent(Editor.ToSharedRef());

    bSpawnedTab = true;
    if (Owner.Pin().IsValid())
    {
        Owner.Pin()->PropertiesTab->AttemptToSetPropertyTabReferences();
    }
    else
    {
        UE_LOG(LogNFToolEditor, Fatal, TEXT("VMEEditorWindow::SpawnTab => Voxel Mesh Editor weak pointer invalid!"));
    }
    return EditorTab.ToSharedRef();
}


#undef LOCTEXT_NAMESPACE