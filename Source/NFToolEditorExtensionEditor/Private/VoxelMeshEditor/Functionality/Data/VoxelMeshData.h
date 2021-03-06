// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VoxelMeshData.generated.h"

UCLASS(hidecategories = Object, MinimalAPI, NotPlaceable)
class UVoxelMeshData : public UObject
{
public:
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "Data")
        int32 GridScaleX;

    UPROPERTY(BlueprintReadOnly, Category = "Data")
        int32 GridScaleY;
    
    UPROPERTY(BlueprintReadOnly, Category = "Data")
        int32 GridScaleZ;

    UPROPERTY(BlueprintReadOnly, Category = "Data")
        float VoxelScale;

    UPROPERTY(BlueprintReadOnly, Category = "Data")
        TArray<bool> Voxels;

    UPROPERTY(BlueprintReadOnly, Category = "Data")
        TArray<FLinearColor> VoxelColors;
};