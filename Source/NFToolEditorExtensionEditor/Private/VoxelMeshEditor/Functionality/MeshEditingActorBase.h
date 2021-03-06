// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeshEditingActorBase.generated.h"

class UProceduralMeshComponent;

UCLASS(Abstract)
class AVMEMeshEditingActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
    AVMEMeshEditingActorBase();

    virtual void OnConstruction(const FTransform& Transform) override;

    /** Will pass the verticies and indicies data to the procedural mesh */
    void ApplyMesh() const;

    virtual void OnGridScaleChanged();
    virtual void OnVoxelScaleChanged();

protected:

    /** Reset the procedural mesh data */
    virtual void ClearMeshData();

    /** Will populate the Verticies data */
    virtual void GenerateMesh();

    class UMaterialInstanceConstant* GetVoxelMeshMaterial() const;

public:
    int32 GridScaleX = 16, GridScaleY = 16, GridScaleZ = 16;
    float VoxelScale = 100.f;

    TObjectPtr<UProceduralMeshComponent> ProceduralMesh;

protected:
    TArray<FVector> Vertices;
    TArray<int32> Indicies;
    TArray<FVector2D> UV;
    TArray<FLinearColor> Colors;

    int32 Verts;
    bool bUpdateMesh = false;
    class UMaterialInstanceConstant* VoxelMeshMaterial;
};
