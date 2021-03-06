// Copyright © 2021 Nicky Fieu, All Rights Reserved.


#include "EditorPropertiesTab.h"
#include "NFToolEditorModule.h"
#include "VoxelMeshEditor.h"
#include "SEditorPropertiesTab.h"
#include "SEditorWindow.h"
#include "EditorPreviewScene.h"
#include "EditorWindow.h"
#include "SEditorWindow.h"
#include "EditorViewportClient.h"
#include "EditorPreviewScene.h"

#define LOCTEXT_NAMESPACE "VoxelMeshEditor"

const FName FVMETabIds::EditorProperties = "VoxelMeshEditorPropertiesTab";


void VMEEditorPropertiesTab::AddModuleListeners()
{

}

VMEEditorPropertiesTab::~VMEEditorPropertiesTab()
{

}

void VMEEditorPropertiesTab::OnStartupModule()
{
    // Ignoring inherited functionality
    // Since this would not allow for the correct spawning behavior to happen
    // Super::OnStartupModule();
    INFToolsModuleInterface::StartupModule();
    Initialize();
}

void VMEEditorPropertiesTab::OnShutdownModule()
{
    Super::OnShutdownModule();
    INFToolsModuleInterface::ShutdownModule();
    FGlobalTabmanager::Get()->UnregisterTabSpawner(TabName);
}

void VMEEditorPropertiesTab::Initialize()
{
    TabName = FVMETabIds::EditorProperties;
    TabDisplayName = FText::FromString("Properties");
    TabToolTip = FText::FromString("The voxel mesh editor properties.");
}

TSharedRef<SDockTab> VMEEditorPropertiesTab::SpawnTab(const FSpawnTabArgs& SpawnTabArgs)
{
    TSharedPtr<SDockTab> EditorTab;
    SAssignNew(EditorTab, SDockTab)
        .TabRole(ETabRole::PanelTab);

    TSharedPtr<SVMEEditorPropertiesTab> PanelContent;
    SAssignNew(PanelContent, SVMEEditorPropertiesTab);
    PropertiesTab = PanelContent;

    EditorTab.Get()->SetContent(PanelContent.ToSharedRef());

    bSpawnedTab = true;
    if (Owner.Pin().IsValid())
    {
        AttemptToSetPropertyTabReferences();
    }
    else
    {
        UE_LOG(LogNFToolEditor, Fatal, TEXT("VMEEditorPropertiesTab::SpawnTab => Voxel Mesh Editor weak pointer invalid!"));
    }
    return EditorTab.ToSharedRef();
}

void VMEEditorPropertiesTab::AttemptToSetPropertyTabReferences()
{
    if (bSpawnedTab && Owner.Pin()->ToolTab->bSpawnedTab)
    {
        FVMEEditorViewportClient& ViewportClient = Owner.Pin()->ToolTab->EditorWindow.Pin()->GetEditorViewportClient();
        PropertiesTab.Pin()->EditorRef = ViewportClient.GetPreviewScene()->MeshEditorActor.Get();
        PropertiesTab.Pin()->GridRef = ViewportClient.GetPreviewScene()->GridActor.Get();
    }
}


#undef LOCTEXT_NAMESPACE