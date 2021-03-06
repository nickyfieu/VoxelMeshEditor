// Copyright © 2021 Nicky Fieu, All Rights Reserved.


#include "VoxelMeshEditorActor.h"
#include "VoxelMeshEditorGridActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NFToolEditorModule.h"
#include "UObject/ConstructorHelpers.h"
#include "ProceduralMeshComponent.h"
#include "AssetToolsModule.h"
#include "Dialogs/DlgPickAssetPath.h"
#include "RawMesh.h"
#include "ProceduralMeshConversion.h"
#include "MeshDescription.h"
#include "AssetRegistryModule.h"
#include "VoxelMeshEditor/Functionality/Data/VoxelMeshData.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "EditorAssetLibrary.h"

#define LOCTEXT_NAMESPACE "VoxelMeshEditor"

// Sets default values
AVoxelMeshEditorActor::AVoxelMeshEditorActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    UpdateVoxelDataContainers();
    ClearMeshData();
    GenerateMesh();
    ApplyMesh();
}

bool AVoxelMeshEditorActor::Check(FVector Position)
{
    if (!IsValidPosition(Position))
    {
        // When out of bounds return true ( in this scenario we want faces to still be drawn )
        return true;
    }

    FIntVector Index = GetVoxelIndex(Position); 
    // When there is no block in this index we draw the face
    return Voxels[Index.X][Index.Y][Index.Z] == false;
}

void AVoxelMeshEditorActor::CreateVoxel(FHitResult Hit)
{
    if (Hit.bBlockingHit)
    {
        FVector NewVoxelLocation = Hit.ImpactPoint + Hit.ImpactNormal * VoxelScale * 0.5f; // * 0.1f to make sure we are somewhere inside the neighbouring voxel space
        bool GotChanged = false;
        if (IsValidPosition(NewVoxelLocation))
        {
            FIntVector Index = GetVoxelIndex(NewVoxelLocation);
            GotChanged = Voxels[Index.X][Index.Y][Index.Z] == false;
            Voxels[Index.X][Index.Y][Index.Z] = true;
            if (GotChanged)
            {
                VoxelColors[Index.X][Index.Y][Index.Z] = BrushColor;
            }
        }

        if (GotChanged)
        {
            ClearMeshData();
            GenerateMesh();
            ApplyMesh();
        }
    }
}

void AVoxelMeshEditorActor::RemoveVoxel(FHitResult Hit)
{
    if (Hit.bBlockingHit)
    {
        FVector NewVoxelLocation = Hit.ImpactPoint - Hit.ImpactNormal * VoxelScale * 0.5f; // * 0.5f to make sure we are somewhere inside the neighbouring voxel space
        bool GotChanged = false;
        if (IsValidPosition(NewVoxelLocation))
        {
            FIntVector Index = GetVoxelIndex(NewVoxelLocation);
            GotChanged = Voxels[Index.X][Index.Y][Index.Z] == true;
            Voxels[Index.X][Index.Y][Index.Z] = false;
        }

        if (GotChanged)
        {

            ClearMeshData();
            GenerateMesh();
            ApplyMesh();
        }
    }
}

void AVoxelMeshEditorActor::PaintVoxel(FHitResult Hit)
{
    if (Hit.bBlockingHit)
    {
        FVector NewVoxelLocation = Hit.ImpactPoint - Hit.ImpactNormal * VoxelScale * 0.5f; // * 0.5f to make sure we are somewhere inside the neighbouring voxel space
        bool GotChanged = false;
        if (IsValidPosition(NewVoxelLocation))
        {
            FIntVector Index = GetVoxelIndex(NewVoxelLocation);
            GotChanged = Voxels[Index.X][Index.Y][Index.Z] == true;
            if (GotChanged)
            {
                VoxelColors[Index.X][Index.Y][Index.Z] = BrushColor;
            }
        }

        if (GotChanged)
        {
            ClearMeshData();
            GenerateMesh();
            ApplyMesh();
        }
    }
}

void AVoxelMeshEditorActor::CreateVoxelFace(FVector FaceNormal, FVector Position)
{
    Vertices.Append(GetFaceVertices(FaceNormal, Position));
    Indicies.Append({ Verts + 3, Verts + 2, Verts + 0, Verts + 2, Verts + 1, Verts + 0 });
    // Temporary UV data until textures are added
    UV.Append({ FVector2D(1.f, 1.f), FVector2D(1.f, 0.f), FVector2D(0.f, 0.f), FVector2D(0.f, 1.f) });
    Verts += 4;
}

TArray<FVector> AVoxelMeshEditorActor::GetFaceVertices(FVector FaceNormal, FVector Position) const
{
    TArray<FVector> TempVertices;

    int32 FaceIndex =   (FaceNormal.X > 0.99f) * 0 + // Front face
                        (FaceNormal.X < -0.99f) * 1 + // Back face
                        (FaceNormal.Y < -0.99f) * 2 + // Left face
                        (FaceNormal.Y > 0.99f) * 3 + // Right face
                        (FaceNormal.Z > 0.99f) * 4 + // Top face
                        (FaceNormal.Z < -0.99f) * 5; // Bottom Face

    for (int32 i = 0; i < 4; ++i)
    {
        TempVertices.Add(VoxelVerticies[VoxelIndicies[i + FaceIndex * 4]] * VoxelScale + Position);
    }

    return TempVertices;
}

FVector AVoxelMeshEditorActor::GetSuroundVoxel(FVector FaceNormal, FVector Position) const
{
    return Position + FaceNormal * VoxelScale;
}

FIntVector AVoxelMeshEditorActor::GetVoxelIndex(FVector Position)
{
    FIntVector VoxelIndex;
    VoxelIndex.X = abs(int32(Position.X + GridScaleX * 0.5f * VoxelScale) / int32(VoxelScale));
    VoxelIndex.Y = abs(int32(Position.Y + GridScaleY * 0.5f * VoxelScale) / int32(VoxelScale));
    VoxelIndex.Z = abs(int32(Position.Z) / int32(VoxelScale));
    return VoxelIndex;
}

void AVoxelMeshEditorActor::GenerateMesh()
{
    check(GetActorLocation().IsZero()); // if not zero this could potentially create issues ( could be fixed but not needed for now )
    FVector StartLoc = FVector(-VoxelScale * (GridScaleX / 2.f), -VoxelScale * (GridScaleY / 2.f), 0.f);

    for (int32 x = 0; x < GridScaleX; ++x)
    {
        for (int32 y = 0; y < GridScaleY; ++y)
        {
            for (int32 z = 0; z < GridScaleZ; ++z)
            {
                FVector Position = StartLoc + FVector(x, y, z) * VoxelScale + FVector(0.5f) * VoxelScale;

                if (Voxels[x][y][z])
                {
                    int32 prevVerts = Verts;
                    const TArray<FVector> FaceNormals = { FVector::ForwardVector, FVector::BackwardVector, FVector::LeftVector, FVector::RightVector, FVector::UpVector, FVector::DownVector };
                    for (int32 i = 0; i < FaceNormals.Num(); ++i)
                    {
                        if (Check(GetSuroundVoxel(FaceNormals[i], Position)))
                        {
                            CreateVoxelFace(FaceNormals[i], StartLoc + FVector(x, y, z) * VoxelScale);
                        }
                    }
                    for (int32 i = 0; i < (Verts - prevVerts); ++i)
                    {
                        Colors.Add(VoxelColors[x][y][z]);
                    }
                }
            }
        }
    }
}

bool AVoxelMeshEditorActor::IsValidPosition(FVector Position) const
{
    const FVector Center = FVector(0.f, 0.f, VoxelScale * GridScaleZ * 0.5f);
    const FVector Offset = FVector(GridScaleX, GridScaleY, GridScaleZ) * VoxelScale * 0.5f - FVector::OneVector * VoxelScale * 0.01f; // small to negate out of bounds error
    const FVector Cornor1 = Center - Offset;
    const FVector Cornor2 = Center + Offset;
    return  (Position.X > Cornor1.X) && (Position.X < Cornor2.X) &&
            (Position.Y > Cornor1.Y) && (Position.Y < Cornor2.Y) &&
            (Position.Z > Cornor1.Z) && (Position.Z < Cornor2.Z);
}

void AVoxelMeshEditorActor::UpdateVoxelDataContainers()
{
    Voxels.SetNum(GridScaleX);
    VoxelColors.SetNum(GridScaleX);
    for (int32 x = 0; x < GridScaleX; ++x)
    {
        Voxels[x].SetNum(GridScaleY);
        VoxelColors[x].SetNum(GridScaleY);
        for (int32 y = 0; y < GridScaleY; ++y)
        {
            Voxels[x][y].SetNum(GridScaleZ);
            VoxelColors[x][y].SetNum(GridScaleZ);
        }
    }
}

void AVoxelMeshEditorActor::OnGridScaleChanged()
{
    UpdateVoxelDataContainers();
    AVMEMeshEditingActorBase::OnGridScaleChanged();
}

void AVoxelMeshEditorActor::OnClickedSaveVoxelMesh()
{
    // Initializing the file name picker & location selector
    FString NewNameSuggestion = FString("ReplaceThis");
    FString PackageName = FString("/Game/") + NewNameSuggestion;
    FString Name;
    FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
    AssetToolsModule.Get().CreateUniqueAssetName(PackageName, TEXT(""), PackageName, Name);

    PackageName = (PreviousSavePath.IsEmpty()) ? PackageName : PreviousSavePath;

    TSharedPtr<SDlgPickAssetPath> PickAssetPathWidget =
        SNew(SDlgPickAssetPath)
        .Title(LOCTEXT("SaveVoxelMeshTitle", "Choose The Location To Save To"))
        .DefaultAssetPath(FText::FromString(PackageName));

    if (PickAssetPathWidget->ShowModal() == EAppReturnType::Ok)
    {
        // Get the full path of where we want to save our voxel mesh data
        FString UserPackageName = PickAssetPathWidget->GetFullAssetPath().ToString();
        FName DataName(*FPackageName::GetLongPackageAssetName(UserPackageName));

        // Check if the given data name is valid otherwise use generated one
        if (DataName == NAME_None)
        {
            UE_LOG(LogNFToolEditor, Warning, TEXT("Cannot save without giving a valid asset name!"));
            return;
        }

        // Then find/create it.
        UPackage* Package = CreatePackage(*UserPackageName);
        check(Package);

        VoxelMeshDataAsset = NewObject<UVoxelMeshData>(Package, DataName, RF_Public | RF_Standalone);

        OnSave(VoxelMeshDataAsset);

        // Notify asset registry of new asset
        FAssetRegistryModule::AssetCreated(VoxelMeshDataAsset);
        UEditorAssetLibrary::SaveAsset(UserPackageName, false);
    }
}

void AVoxelMeshEditorActor::OnClickedLoadVoxelMesh()
{
    FContentBrowserModule & ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
    
    FSetARFilterDelegate FilterDelegate;
    
    FAssetPickerConfig PickerConfig;
    PickerConfig.SelectionMode = ESelectionMode::Single;
    PickerConfig.bAllowDragging = false;
    PickerConfig.bAllowNullSelection = false;
    PickerConfig.InitialAssetViewType = EAssetViewType::Tile;
    PickerConfig.Filter.ClassNames.Add(UVoxelMeshData::StaticClass()->GetFName());
    TSharedRef<SWindow> AssetPickerWindow =
        SNew(SWindow)
        .Title(FText::FromString("Double Click The Asset To Load"))
        .SupportsMinimize(false)
        .SupportsMaximize(false)
        .ClientSize(FVector2D(400.f,400.f));
    PickerConfig.OnAssetDoubleClicked.BindUObject(this, &AVoxelMeshEditorActor::OnAssetEntered, AssetPickerWindow);
    AssetPickerWindow->SetContent(ContentBrowserModule.Get().CreateAssetPicker(PickerConfig));
    GEditor->EditorAddModalWindow(
        AssetPickerWindow
    );
}

void AVoxelMeshEditorActor::OnAssetEntered(const FAssetData& AssetData, TSharedRef<SWindow> OwningWindow)
{
    UVoxelMeshData* DataAsset = Cast<UVoxelMeshData>(AssetData.GetAsset());
    if (DataAsset)
    {
        OnLoad(DataAsset);

        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVoxelMeshEditorGridActor::StaticClass(), FoundActors);
        for (int32 i = 0; i < FoundActors.Num(); ++i)
        {
            AVoxelMeshEditorGridActor* GridActor = Cast<AVoxelMeshEditorGridActor>(FoundActors[i]);
            GridActor->OnLoad(DataAsset);
        }

        OwningWindow->RequestDestroyWindow();
    }
}

void AVoxelMeshEditorActor::OnClickedExportVoxelMesh()
{
    if (ProceduralMesh != nullptr)
    {
        // Initializing the file name picker & location selector
        FString NewNameSuggestion = FString("VoxelMesh");
        FString PackageName = FString("/Game/Meshes/") + NewNameSuggestion;
        FString Name;
        FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
        AssetToolsModule.Get().CreateUniqueAssetName(PackageName, TEXT(""), PackageName, Name);

        PackageName = (PreviousExportPath.IsEmpty()) ? PackageName: PreviousExportPath;

        TSharedPtr<SDlgPickAssetPath> PickAssetPathWidget =
            SNew(SDlgPickAssetPath)
            .Title(LOCTEXT("ExportVoxelMeshTitle", "Choose The Location To Export To"))
            .DefaultAssetPath(FText::FromString(PackageName));

        if (PickAssetPathWidget->ShowModal() == EAppReturnType::Ok)
        {
            // Get the full path of where we want to save our voxel mesh
            FString UserPackageName = PickAssetPathWidget->GetFullAssetPath().ToString();
            FName MeshName(*FPackageName::GetLongPackageAssetName(UserPackageName));

            // Check if the given mesh name is valid otherwise use generated one
            if (MeshName == NAME_None)
            {
                // Use the defaults that were previously generated in the initialize section
                UserPackageName = PackageName;
                MeshName = *Name;
            }

            PreviousExportPath = UserPackageName;

            // Raw mesh data we are filling in
            FRawMesh RawMesh;
            // Materials to apply to new mesh
            TArray<UMaterialInterface*> MeshMaterials;

            const int32 NumSections = ProceduralMesh->GetNumSections();
            int32 VertexBase = 0;
            for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
            {
                FProcMeshSection* ProcSection = ProceduralMesh->GetProcMeshSection(SectionIdx);

                // Copy verticies
                for (FProcMeshVertex& Vert : ProcSection->ProcVertexBuffer)
                {
                    RawMesh.VertexPositions.Add(Vert.Position);
                }

                // Copy 'wedge' info
                int32 NumIndices = ProcSection->ProcIndexBuffer.Num();
                for (int32 IndexIdx = 0; IndexIdx < NumIndices; IndexIdx++)
                {
                    int32 Index = ProcSection->ProcIndexBuffer[IndexIdx];

                    RawMesh.WedgeIndices.Add(Index + VertexBase);

                    FProcMeshVertex& ProcVertex = ProcSection->ProcVertexBuffer[Index];

                    FVector TangentX = ProcVertex.Tangent.TangentX;
                    FVector TangentZ = ProcVertex.Normal;
                    FVector TangentY = (TangentX ^ TangentZ).GetSafeNormal() * (ProcVertex.Tangent.bFlipTangentY ? -1.f : 1.f);

                    RawMesh.WedgeTangentX.Add(TangentX);
                    RawMesh.WedgeTangentY.Add(TangentY);
                    RawMesh.WedgeTangentZ.Add(TangentZ);

                    RawMesh.WedgeTexCoords[0].Add(ProcVertex.UV0);
                    RawMesh.WedgeColors.Add(ProcVertex.Color);
                }

                // copy face info
                int32 NumTris = NumIndices / 3;
                for (int32 TriIdx = 0; TriIdx < NumTris; TriIdx++)
                {
                    RawMesh.FaceMaterialIndices.Add(SectionIdx);
                    RawMesh.FaceSmoothingMasks.Add(0); // Assume this is ignored as bRecomputeNormals is false
                }

                // Remember material
                MeshMaterials.Add(ProceduralMesh->GetMaterial(SectionIdx));

                // Update offset for creating one big index/vertex buffer
                VertexBase += ProcSection->ProcVertexBuffer.Num();

                // If we got some valid data.
                if (RawMesh.VertexPositions.Num() > 3 && RawMesh.WedgeIndices.Num() > 3)
                {
                    // Then find/create it.
                    UPackage* Package = CreatePackage(*UserPackageName);
                    check(Package);

                    // Create StaticMesh object
                    UStaticMesh* StaticMesh = NewObject<UStaticMesh>(Package, MeshName, RF_Public | RF_Standalone);
                    StaticMesh->InitResources();

                    StaticMesh->SetLightingGuid(FGuid::NewGuid());

                    // Add source to new StaticMesh
                    FStaticMeshSourceModel& SrcModel = StaticMesh->AddSourceModel();
                    SrcModel.BuildSettings.bRecomputeNormals = false;
                    SrcModel.BuildSettings.bRecomputeTangents = false;
                    SrcModel.BuildSettings.bRemoveDegenerates = false;
                    SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
                    SrcModel.BuildSettings.bUseFullPrecisionUVs = false;
                    SrcModel.BuildSettings.bGenerateLightmapUVs = true;
                    SrcModel.BuildSettings.SrcLightmapIndex = 0;
                    SrcModel.BuildSettings.DstLightmapIndex = 1;
                    SrcModel.RawMeshBulkData->SaveRawMesh(RawMesh);

                    // Copy materials to new mesh
                    for (UMaterialInterface* Material : MeshMaterials)
                    {
                        StaticMesh->GetStaticMaterials().Add(FStaticMaterial(Material));
                    }

                    //Set the Imported version before calling the build
                    StaticMesh->ImportVersion = EImportStaticMeshVersion::LastVersion;

                    // Build mesh from source
                    StaticMesh->Build(false);
                    StaticMesh->PostEditChange();

                    // Notify asset registry of new asset
                    FAssetRegistryModule::AssetCreated(StaticMesh);
                    UEditorAssetLibrary::SaveAsset(UserPackageName, false);
                }
            }
        }
    }
}

void AVoxelMeshEditorActor::OnLoad(UVoxelMeshData* Data)
{
    VoxelMeshDataAsset = Data;
    GridScaleX = (VoxelMeshDataAsset->GridScaleX <= 0) ? 16 : VoxelMeshDataAsset->GridScaleX;
    GridScaleY = (VoxelMeshDataAsset->GridScaleY <= 0) ? 16 : VoxelMeshDataAsset->GridScaleY;
    GridScaleZ = (VoxelMeshDataAsset->GridScaleZ <= 0) ? 16 : VoxelMeshDataAsset->GridScaleZ;
    VoxelScale = (FMath::IsNearlyZero(VoxelMeshDataAsset->VoxelScale)) ? 100.f : VoxelMeshDataAsset->VoxelScale;
    
    Voxels.Empty();
    VoxelColors.Empty();
    Voxels.SetNum(GridScaleX);
    VoxelColors.SetNum(GridScaleX);
    for (int32 x = 0; x < GridScaleX; ++x)
    {
        Voxels[x].SetNum(GridScaleY);
        VoxelColors[x].SetNum(GridScaleY);
        for (int32 y = 0; y < GridScaleY; ++y)
        {
            Voxels[x][y].SetNum(GridScaleZ);
            VoxelColors[x][y].SetNum(GridScaleZ);
            for (int32 z = 0; z < GridScaleZ; ++z)
            {
                int32 Index = x + GridScaleX * (y + z * GridScaleY);
                Voxels[x][y][z] = Data->Voxels[Index];
                VoxelColors[x][y][z] = Data->VoxelColors[Index];
            }
        }
    }

    ClearMeshData();
    GenerateMesh();
    ApplyMesh();
}

/**
 * Save all mesh editor data into the given voxel mesh data
 */
void AVoxelMeshEditorActor::OnSave(class UVoxelMeshData* Data) const
{
    Data->GridScaleX = GridScaleX;
    Data->GridScaleY = GridScaleY;
    Data->GridScaleZ = GridScaleZ;
    Data->VoxelScale = VoxelScale;
    Data->Voxels.Empty();
    Data->VoxelColors.Empty();
    Data->Voxels.SetNum(Data->GridScaleX * Data->GridScaleY * Data->GridScaleZ);
    Data->VoxelColors.SetNum(Data->GridScaleX * Data->GridScaleY * Data->GridScaleZ);
    for (int32 x = 0; x < Data->GridScaleX; ++x)
    {
        for (int32 y = 0; y < Data->GridScaleY; ++y)
        {
            for (int32 z = 0; z < Data->GridScaleZ; ++z)
            {
                int32 Index = x + GridScaleX * (y + z * GridScaleY);
                Data->Voxels[Index] = Voxels[x][y][z];
                Data->VoxelColors[Index] = VoxelColors[x][y][z];
            }
        }
    }
}

#undef LOCTEXT_NAMESPACE