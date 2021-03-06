// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MeshEditingActorBase.h"
#include "VoxelMeshEditorGridActor.generated.h"

UCLASS()
class AVoxelMeshEditorGridActor : public AVMEMeshEditingActorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVoxelMeshEditorGridActor();

    void OnGridLineValueChanged();

    void OnLoad(class UVoxelMeshData* Data);

protected:
    // AVMEMeshEditingActorBase interface
    virtual void GenerateMesh() override;
    // End of AVMEMeshEditingActorBase interface

    void CreateGridLine(FVector StartLocation, FVector FaceNormal, FVector Direction, float GridScale, bool HalfSizeLeft = false, bool HalfSizeRight = false);

    void CreateGroundPlane(FVector StartLocation, FVector FaceNormal);

public:
    float GridLineHalfWidth = 1.25f;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    class UStaticMeshComponent* PlaneMeshBottom;

protected:
    class UStaticMesh* PlaneStaticMesh;
    class UMaterial* PlaneMaterial;
};
