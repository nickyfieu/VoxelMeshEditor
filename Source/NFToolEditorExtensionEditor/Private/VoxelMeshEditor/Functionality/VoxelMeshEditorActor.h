// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MeshEditingActorBase.h"
#include "VoxelMeshEditorActor.generated.h"

UCLASS()
class AVoxelMeshEditorActor : public AVMEMeshEditingActorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVoxelMeshEditorActor();

    /** Will add a new voxel at the hit location + the impact normal */
    void CreateVoxel(FHitResult Hit);

    /** Vill remove a new voxel at the hit location - the impact normal */
    void RemoveVoxel(FHitResult Hit);

    /** Vill paint an existing voxel at the hit location - the impact normal */
    void PaintVoxel(FHitResult Hit);

    // AVMEMeshEditingActorBase interface
    virtual void OnGridScaleChanged() override;
    // End of AVMEMeshEditingActorBase interface

    void OnClickedSaveVoxelMesh();
    void OnClickedLoadVoxelMesh();
    void OnClickedExportVoxelMesh();

    void OnLoad(class UVoxelMeshData* Data);
    void OnSave(class UVoxelMeshData* Data) const;

protected:
    /** Will check if there is a voxel there already or not */
    bool Check(FVector Position);

    void CreateVoxelFace(FVector FaceNormal, FVector Position);

    void UpdateVoxelDataContainers();

    /** Returns an array with the world space vertex locations */
    TArray<FVector> GetFaceVertices(FVector FaceNormal, FVector Position) const;

    /** Returns a neighbouring voxel position relative to the position */
    FVector GetSuroundVoxel(FVector FaceNormal, FVector Position) const;

    /** Returns the Voxels index at the given xyz index*/
    FIntVector GetVoxelIndex(FVector Position);

    // AVMEMeshEditingActorBase interface
    virtual void GenerateMesh() override;
    // End of AVMEMeshEditingActorBase interface

    bool IsValidPosition(FVector Position) const;

    void OnAssetEntered(const FAssetData& AssetData, TSharedRef<SWindow> OwningWindow);

public:
    FLinearColor BrushColor = FLinearColor::Black;

protected:
    TArray<TArray<TArray<bool>>> Voxels;
    TArray<TArray<TArray<FLinearColor>>> VoxelColors;
    class UVoxelMeshData* VoxelMeshDataAsset;

    const TArray<FVector> VoxelVerticies = TArray<FVector>{
        FVector(1.f, 1.f, 1.f),
        FVector(1.f, 0.f, 1.f),
        FVector(1.f, 0.f, 0.f),
        FVector(1.f, 1.f, 0.f),
        FVector(0.f, 0.f, 1.f),
        FVector(0.f, 1.f, 1.f),
        FVector(0.f, 1.f, 0.f),
        FVector(0.f, 0.f, 0.f)
    };

    const TArray<int32> VoxelIndicies = TArray<int32>{
        0, 1, 2, 3, // Front face
        4, 5, 6, 7, // Back face
        1, 4, 7, 2, // Left face
        5, 0, 3, 6, // Right face
        5, 4, 1, 0, // Top face
        3, 2, 7, 6  // Bottom Face
    };

    FString PreviousSavePath = FString();
    FString PreviousExportPath = FString();
};
