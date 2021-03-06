// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#include "VoxelMeshEditor.h"
#include "SVoxelMeshEditor.h"
#include "EditorWindow.h"
#include "EditorPropertiesTab.h"
#include "NFToolEditorModule.h"
#include "VoxelMeshEditorStyle.h"
#include "AssetToolsModule.h"
#include "VoxelMeshEditor/Functionality/Data/VoxelMeshDataTypeActions.h"
#include "VoxelMeshEditor/Functionality/Data/VoxelMeshData.h"

#define LOCTEXT_NAMESPACE "VoxelMeshEditor"

const FName FVMETabIds::MainWindow = "VoxelMeshEditorMainPannel";

void VoxelMeshEditor::AddModuleListeners()
{
    ToolTab = MakeShareable(new VMEEditorWindow());
    PropertiesTab = MakeShareable(new VMEEditorPropertiesTab());
    ToolTab->Owner = SharedThis(this);
    PropertiesTab->Owner = SharedThis(this);
    InternalPanelPtrs.Add(ToolTab);
    InternalPanelPtrs.Add(PropertiesTab);
    ModuleListeners.Add(ToolTab.ToSharedRef());
    ModuleListeners.Add(PropertiesTab.ToSharedRef());
}

VoxelMeshEditor::~VoxelMeshEditor()
{
}

void VoxelMeshEditor::OnStartupModule()
{
    Super::OnStartupModule();
    INFToolsModuleInterface::StartupModule();

    /**
     * register voxel mesh data type
     * register its asset type category
     * Add it to CreatedAssetTypeActions for deletion
     */
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    EAssetTypeCategories::Type VoxelMeshDataCategory = EAssetTypeCategories::Misc;
    TSharedPtr<IAssetTypeActions> Action = MakeShareable(new FVoxelMeshDataTypeActions(VoxelMeshDataCategory));
    AssetTools.RegisterAssetTypeActions(Action.ToSharedRef());
    CreatedAssetTypeActions.Add(Action);

    /**
     * Adding a menu entry after the NFAssetTools hook
     */
    FNFToolEditorModule::Get().AddMenuExtension(
        FName("NFAssetTools"),
        FMenuExtensionDelegate::CreateRaw(
            this,
            &Super::MakeMenuEntry
        ),
        EExtensionHook::After
    );
}

void VoxelMeshEditor::OnShutdownModule()
{
    Super::OnShutdownModule();
    INFToolsModuleInterface::ShutdownModule();
    InternalPanelPtrs.Empty();

    /**
     * Unregister all the asset type actions that got created
     */
    if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
    {
        IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
        for (int32 i = 0; i < CreatedAssetTypeActions.Num(); ++i)
        {
            AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[i].ToSharedRef());
        }
    }
    CreatedAssetTypeActions.Empty();

    FVoxelMeshEditorStyle::Shutdown();
}

void VoxelMeshEditor::Initialize()
{
    TabName = FVMETabIds::MainWindow;
    TabDisplayName = FText::FromString("Voxel Mesh Editor");
    TabToolTip = FText::FromString("The main window of the voxel mesh editor.");

    FVoxelMeshEditorStyle::Initialize();
}

TSharedRef<SDockTab> VoxelMeshEditor::SpawnTab(const FSpawnTabArgs& SpawnTabArgs)
{
    ToolTab->bSpawnedTab = false;
    PropertiesTab->bSpawnedTab = false;

    TSharedPtr<SDockTab> VoxelMeshEditorMainWindow;
    SAssignNew(VoxelMeshEditorMainWindow, SDockTab)
        .TabRole(ETabRole::MajorTab);

    TSharedPtr<SVoxelMeshEditor> VoxelMeshEditor;
    SAssignNew(VoxelMeshEditor, SVoxelMeshEditor, VoxelMeshEditorMainWindow.ToSharedRef())
        .OwningObject(SharedThis(this));
    
    VoxelMeshEditorMainWindow->SetContent(VoxelMeshEditor.ToSharedRef());

    return VoxelMeshEditorMainWindow.ToSharedRef();
}

#undef LOCTEXT_NAMESPACE