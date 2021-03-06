// Copyright © 2021 Nicky Fieu, All Rights Reserved.


#include "VoxelMeshEditorGridActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ProceduralMeshComponent.h"
#include "VoxelMeshEditor/Functionality/Data/VoxelMeshData.h"

#define LOCTEXT_NAMESPACE "VoxelMeshEditor"

// Sets default values
AVoxelMeshEditorGridActor::AVoxelMeshEditorGridActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Plane.Plane"));
    PlaneStaticMesh = MeshObj.Object;
    const ConstructorHelpers::FObjectFinder<UMaterial> MaterialObj(TEXT("/Engine/EditorMaterials/GridMaterial.GridMaterial"));
    PlaneMaterial = MaterialObj.Object;

    ClearMeshData();
    GenerateMesh();
    ApplyMesh();
}

void AVoxelMeshEditorGridActor::GenerateMesh()
{
    check(GetActorLocation().IsZero()); // if not zero this could potentially create issues ( could be fixed but not needed for now )
    FVector StartLoc = -FVector((VoxelScale * GridScaleX) * 0.5f, (VoxelScale * GridScaleY) * 0.5f, 0.f);

    // bottom grid
    CreateGroundPlane(FVector(0.f, 0.f, -0.1f), FVector::UpVector);
    for (int32 x = 0; x < (GridScaleX + 1); ++x)
    {
        CreateGridLine(StartLoc + FVector(x * VoxelScale, 0.f, 0.f), FVector::UpVector, FVector::RightVector, GridScaleY, (x == 0), (x == GridScaleX));
    }
    for (int32 y = 0; y < (GridScaleY + 1); ++y)
    {
        CreateGridLine(StartLoc + FVector(0.f, y * VoxelScale, -0.01f), FVector::UpVector, FVector::ForwardVector, GridScaleX, (y == GridScaleY), (y == 0));
    }
    
    // Initializing the vertex colors
    Colors.SetNum(Verts);
    for (int32 i = 0; i < Verts; ++i)
    {
        Colors[i] = FLinearColor::White;
    }
}

void AVoxelMeshEditorGridActor::CreateGridLine(FVector StartLocation, FVector FaceNormal, FVector Direction, float GridScale, bool HalfSizeLeft, bool HalfSizeRight)
{
    const FVector RightVector = FVector::CrossProduct(Direction, FaceNormal);
    const FVector BottomLeft = StartLocation - FVector(RightVector * !HalfSizeLeft * GridLineHalfWidth);
    const FVector BottomRight = StartLocation + FVector(RightVector * !HalfSizeRight * GridLineHalfWidth);
    const FVector TopLeft = BottomLeft + Direction * GridScale * VoxelScale;
    const FVector TopRight = BottomRight + Direction * GridScale * VoxelScale;

    Vertices.Append({ TopLeft, BottomLeft, BottomRight, TopRight });
    Indicies.Append({ Verts + 3, Verts + 2, Verts + 0, Verts + 2, Verts + 1, Verts + 0 });
    // Temporary UV data until textures are added
    UV.Append({ FVector2D(1.f, 1.f), FVector2D(1.f, 0.f), FVector2D(0.f, 0.f), FVector2D(0.f, 1.f) });
    Verts += 4;
}

void AVoxelMeshEditorGridActor::CreateGroundPlane(FVector StartLocation, FVector FaceNormal)
{
    if (!PlaneMeshBottom)
    {
        FString PlaneMeshName = FString("PlaneMeshBottom");
        PlaneMeshBottom = CreateDefaultSubobject<UStaticMeshComponent>(FName(PlaneMeshName));
        PlaneMeshBottom->SetupAttachment(RootComponent);
    }
    PlaneMeshBottom->SetStaticMesh(PlaneStaticMesh);
    UMaterialInstanceDynamic* Mat_Inst = UMaterialInstanceDynamic::Create(PlaneMaterial, nullptr);
    PlaneMeshBottom->SetMaterial(0, Mat_Inst);
    PlaneMeshBottom->SetWorldLocation(StartLocation);
    FRotator Rot = FaceNormal.ToOrientationRotator() - PlaneMeshBottom->GetUpVector().ToOrientationRotator();
    PlaneMeshBottom->AddLocalRotation(Rot);
    FVector ScaleVector = FVector(FMath::Abs(FaceNormal.Y + FaceNormal.Z) * GridScaleX * (VoxelScale/100.f),
                                    FMath::Abs(FaceNormal.X + FaceNormal.Z) * GridScaleY * (VoxelScale / 100.f),
                                        FMath::Abs(FaceNormal.X + FaceNormal.Y) * GridScaleZ * (VoxelScale / 100.f));
    PlaneMeshBottom->SetRelativeScale3D(ScaleVector);
    PlaneMeshBottom->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    PlaneMeshBottom->GetBodySetup()->CollisionTraceFlag = ECollisionTraceFlag::CTF_UseComplexAsSimple;
    PlaneMeshBottom->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
}

void AVoxelMeshEditorGridActor::OnGridLineValueChanged()
{
    bUpdateMesh = true;
    ClearMeshData();
    GenerateMesh();
    ApplyMesh();
    bUpdateMesh = false;
}


void AVoxelMeshEditorGridActor::OnLoad(UVoxelMeshData* Data)
{
    GridScaleX = (Data->GridScaleX <= 0) ? 16 : Data->GridScaleX;
    GridScaleY = (Data->GridScaleY <= 0) ? 16 : Data->GridScaleY;
    GridScaleZ = (Data->GridScaleZ <= 0) ? 16 : Data->GridScaleZ;
    VoxelScale = (FMath::IsNearlyZero(Data->VoxelScale)) ? 100.f : Data->VoxelScale;
    ClearMeshData();
    GenerateMesh();
    ApplyMesh();
}

#undef LOCTEXT_NAMESPACE