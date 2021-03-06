// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class FVoxelMeshDataTypeActions : public FAssetTypeActions_Base
{
public:
    FVoxelMeshDataTypeActions(EAssetTypeCategories::Type InAssetTypeCategory);

    // FAssetTypeActions_Base interface
    virtual FText GetName() const override;
    virtual FColor GetTypeColor() const override;
    virtual UClass* GetSupportedClass() const override;
    virtual uint32 GetCategories() override;
    virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
    // End of FAssetTypeActions_Base interface

private:
    EAssetTypeCategories::Type AssetTypeCategory;
};
