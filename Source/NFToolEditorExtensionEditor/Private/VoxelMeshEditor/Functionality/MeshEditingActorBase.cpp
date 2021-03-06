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

    // If the materials do not exist creat them and save them
    if (!UEditorAssetLibrary::DoesAssetExist("/NFGameUtilities/Content/EditorMaterials/DefaultLitVertexColorMaterial.DefaultLitVertexColorMaterial") &&
        !UEditorAssetLibrary::DoesAssetExist("/NFGameUtilities/Content/EditorMaterials/DefaultLitVertexColorMaterial_Inst.DefaultLitVertexColorMaterial_Inst"))
    {
        FString MaterialBaseName = "DefaultLitVertexColorMaterial";
        FString PackageName = "/NFGameUtilities/Content/EditorMaterials/";
        PackageName += MaterialBaseName;
        // Create the Vertex Color Material & Package
        UPackage* VertexColorMaterialPackage = CreatePackage(*PackageName);
        UMaterialFactoryNew* MaterialFactory = NewObject<UMaterialFactoryNew>();
        UMaterial* VertexColorMaterial = (UMaterial*)MaterialFactory->FactoryCreateNew(
            UMaterial::StaticClass(), VertexColorMaterialPackage, *MaterialBaseName, RF_Standalone | RF_Public, NULL, GWarn
        );
        // Notify the AssetRegestryModule that an asset was created
        FAssetRegistryModule::AssetCreated(VertexColorMaterial);
        VertexColorMaterialPackage->FullyLoad();
        VertexColorMaterialPackage->SetDirtyFlag(true);
        // Create and set the vertex color input node
        UMaterialExpressionVertexColor* VertexColorInput = NewObject<UMaterialExpressionVertexColor>(VertexColorMaterial);
        VertexColorMaterial->BaseColor.Expression = VertexColorInput;
        // Let the material update itself if necessary
        VertexColorMaterial->PreEditChange(NULL);
        VertexColorMaterial->PostEditChange();

        FString MaterialInstBaseName = "DefaultLitVertexColorMaterial_Inst";
        PackageName = "/NFGameUtilities/Content/EditorMaterials/";
        PackageName += MaterialInstBaseName;
        // Create the Vertex Color Material Instance & Package
        UPackage* VertexColorMaterialInstPackage = CreatePackage(*PackageName);
        UMaterialInstanceConstantFactoryNew* MaterialInstanceFactory = NewObject<UMaterialInstanceConstantFactoryNew>();
        MaterialInstanceFactory->InitialParent = VertexColorMaterial;
        UMaterialInstanceConstant* VertexColorMaterialInstance = (UMaterialInstanceConstant*)MaterialInstanceFactory->FactoryCreateNew(
            UMaterialInstanceConstant::StaticClass(), VertexColorMaterialInstPackage, *MaterialInstBaseName, RF_Standalone | RF_Public, NULL, GWarn
        );
        // Notify the AssetRegestryModule that an asset was created
        FAssetRegistryModule::AssetCreated(VertexColorMaterialInstance);
        VertexColorMaterialInstPackage->FullyLoad();
        VertexColorMaterialInstPackage->SetDirtyFlag(true);
        // Let the material update itself if necessary
        VertexColorMaterialInstance->PreEditChange(NULL);
        VertexColorMaterialInstance->PostEditChange();

        UEditorAssetLibrary::SaveAsset("/NFGameUtilities/Content/EditorMaterials/DefaultLitVertexColorMaterial", false);
        UEditorAssetLibrary::SaveAsset("/NFGameUtilities/Content/EditorMaterials/DefaultLitVertexColorMaterial_Inst", false);

        return VertexColorMaterialInstance;
    }

    // When the material instance exists this code will run and return the Material instance
    const ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> MaterialInstObj(
        TEXT("/NFGameUtilities/Content/EditorMaterials/DefaultLitVertexColorMaterial_Inst.DefaultLitVertexColorMaterial_Inst")
    );
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