// Copyright © 2021 Nicky Fieu, All Rights Reserved.


#include "MeshEditingActorBase.h"
#include "ProceduralMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Factories/MaterialFactoryNew.h"
#include "Factories/MaterialInstanceConstantFactoryNew.h"
#include "Materials/MaterialExpressionVertexColor.h"
#include "UObject/ConstructorHelpers.h"
#include "NFToolEditorModule.h"
#include "AssetRegistryModule.h"
#include "EditorAssetLibrary.h"

#define LOCTEXT_NAMESPACE "VoxelMeshEditor"

// Sets default values
AVMEMeshEditingActorBase::AVMEMeshEditingActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
    ProceduralMesh->bUseAsyncCooking = true;
    UMaterialInstanceConstant* mat = GetVoxelMeshMaterial();
    if (mat)
    {
        ProceduralMesh->SetMaterial(0, mat);
    }

    RootComponent = ProceduralMesh;
}

void AVMEMeshEditingActorBase::OnConstruction(const FTransform& Transform)
{
}

void AVMEMeshEditingActorBase::ApplyMesh() const
{
    if (bUpdateMesh)
    {
        ProceduralMesh->UpdateMeshSection_LinearColor(0, Vertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>());
    }
    else
    {
        ProceduralMesh->ClearAllMeshSections();
        ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Indicies, TArray<FVector>(), UV, Colors, TArray<FProcMeshTangent>(), true);
    }
}

void AVMEMeshEditingActorBase::OnGridScaleChanged()
{
    ClearMeshData();
    GenerateMesh();
    ApplyMesh();
}

void AVMEMeshEditingActorBase::OnVoxelScaleChanged()
{
    bUpdateMesh = true;
    ClearMeshData();
    GenerateMesh();
    ApplyMesh();
    bUpdateMesh = false;
}

void AVMEMeshEditingActorBase::GenerateMesh()
{

}

UMaterialInstanceConstant* AVMEMeshEditingActorBase::GetVoxelMeshMaterial() const
{
    if (GEditor == nullptr)
    {
        return nullptr;
    }

    if (!UEditorAssetLibrary::DoesAssetExist("/Engine/EditorMaterials/DefaultLitVertexColorMaterial.DefaultLitVertexColorMaterial") &&
        !UEditorAssetLibrary::DoesAssetExist("/Engine/EditorMaterials/DefaultLitVertexColorMaterial_Inst.DefaultLitVertexColorMaterial_Inst"))
    {
        FString MaterialBaseName = "DefaultLitVertexColorMaterial";
        FString PackageName = "/Engine/EditorMaterials/";
        PackageName += MaterialBaseName;
        UPackage* Package = CreatePackage(*PackageName);

        UMaterialFactoryNew* MaterialFactory = NewObject<UMaterialFactoryNew>();
        UMaterial* VertexColorMaterial = (UMaterial*)MaterialFactory->FactoryCreateNew(UMaterial::StaticClass(), Package, *MaterialBaseName, RF_Standalone | RF_Public, NULL, GWarn);

        FAssetRegistryModule::AssetCreated(VertexColorMaterial);
        Package->FullyLoad();
        Package->SetDirtyFlag(true);

        UMaterialExpressionVertexColor* VertexColorInput = NewObject<UMaterialExpressionVertexColor>(VertexColorMaterial);
        VertexColorMaterial->BaseColor.Expression = VertexColorInput;

        // Let the material update itself if necessary
        VertexColorMaterial->PreEditChange(NULL);
        VertexColorMaterial->PostEditChange();

        FString MaterialInstBaseName = "DefaultLitVertexColorMaterial_Inst";
        PackageName = "/Engine/EditorMaterials/";
        PackageName += MaterialInstBaseName;
        UPackage* PackageInst = CreatePackage(*PackageName);
        
        UMaterialInstanceConstantFactoryNew* MaterialInstanceFactory = NewObject<UMaterialInstanceConstantFactoryNew>();
        MaterialInstanceFactory->InitialParent = VertexColorMaterial;
        UMaterialInstanceConstant* VertexColorMaterialInstance = (UMaterialInstanceConstant*)MaterialInstanceFactory->FactoryCreateNew(UMaterialInstanceConstant::StaticClass(), PackageInst, *MaterialInstBaseName, RF_Standalone | RF_Public, NULL, GWarn);
        
        FAssetRegistryModule::AssetCreated(VertexColorMaterialInstance);
        Package->FullyLoad();
        Package->SetDirtyFlag(true);
        VertexColorMaterialInstance->PreEditChange(NULL);
        VertexColorMaterialInstance->PostEditChange();

        UEditorAssetLibrary::SaveAsset("/Engine/EditorMaterials/DefaultLitVertexColorMaterial", false);
        UEditorAssetLibrary::SaveAsset("/Engine/EditorMaterials/DefaultLitVertexColorMaterial_Inst", false);

        return VertexColorMaterialInstance;
    }

    const ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> MaterialInstObj(TEXT("/Engine/EditorMaterials/DefaultLitVertexColorMaterial_Inst.DefaultLitVertexColorMaterial_Inst"));
    if (MaterialInstObj.Object->IsValidLowLevelFast())
    {
        return MaterialInstObj.Object;
    }
    return nullptr;
}

void AVMEMeshEditingActorBase::ClearMeshData()
{
    Colors.Empty();
    Vertices.Empty();
    Indicies.Empty();
    UV.Empty();
    Verts = 0;
}

#undef LOCTEXT_NAMESPACE 