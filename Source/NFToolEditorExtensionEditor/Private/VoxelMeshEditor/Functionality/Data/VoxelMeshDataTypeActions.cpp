// Copyright © 2021 Nicky Fieu, All Rights Reserved.


#include "VoxelMeshDataTypeActions.h"
#include "VoxelMeshData.h"
#include "NFToolEditorModule.h"
#include "NFToolTabBase.h"
#include "VoxelMeshEditor/VoxelMeshEditor.h"
#include "VoxelMeshEditor/SVoxelMeshEditor.h"
#include "VoxelMeshEditor/Functionality/VoxelMeshEditorActor.h"
#include "VoxelMeshEditor/Functionality/VoxelMeshEditorGridActor.h"
#include "Framework/Docking/TabManager.h"

FVoxelMeshDataTypeActions::FVoxelMeshDataTypeActions(EAssetTypeCategories::Type InAssetTypeCategory)
    : AssetTypeCategory(InAssetTypeCategory)
{

}

FText FVoxelMeshDataTypeActions::GetName() const
{
    return FText::FromString("Voxel Mesh Data");
}

FColor FVoxelMeshDataTypeActions::GetTypeColor() const
{
    return FColor::Emerald;
}

UClass* FVoxelMeshDataTypeActions::GetSupportedClass() const
{
    return UVoxelMeshData::StaticClass();
}

uint32 FVoxelMeshDataTypeActions::GetCategories()
{
    return AssetTypeCategory;
}

void FVoxelMeshDataTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
    if (InObjects.Num() <= 0)
    {
        return;
    }
    /**
     * Instead of opening the normal blueprint editor this asset will open the voxel mesh editor.
     * Ontop of this it will also load the selected voxel mesh into the editor.
     */
    TSharedPtr<SDockTab> MainWindow = FGlobalTabmanager::Get()->TryInvokeTab(FVMETabIds::MainWindow);
    if (!MainWindow.IsValid())
    {
        return;
    }

    TSharedPtr<SVoxelMeshEditor> SMainContent = StaticCastSharedRef<SVoxelMeshEditor>(MainWindow->GetContent());
    UVoxelMeshData* Data = (UVoxelMeshData*)InObjects[0];
    if (!SMainContent.IsValid() || !Data)
    {
        return;
    }

    SMainContent->VoxelMeshEditorActor->OnLoad(Data);
    SMainContent->VoxelMeshEditorGridActor->OnLoad(Data);
}