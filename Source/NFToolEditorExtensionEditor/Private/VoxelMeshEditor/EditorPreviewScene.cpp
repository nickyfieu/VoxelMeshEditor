// Copyright © 2021 Nicky Fieu, All Rights Reserved.


#include "EditorPreviewScene.h"
#include "AssetViewerSettings.h"
#include "Editor/EditorPerProjectUserSettings.h"
#include "../NFToolEditorModule.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Engine/TextureCube.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/SphereReflectionCaptureComponent.h"
#include "EditorCommands.h"
#include "Functionality/VoxelMeshEditorGridActor.h"
#include "GameFramework/WorldSettings.h"
#include "Functionality/VoxelMeshEditorActor.h"
#include "GameFramework/PlayerController.h"
#include "EditorViewportClient.h"
#include "Editor/EditorEngine.h"
#include "Engine/LocalPlayer.h"
#include "CameraController.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"

#define LOCTEXT_NAMESPACE "VoxelMeshEditor"

/**
 * Class will have allot in comon compared to FAdvancedPreviewScene but with some changes to input & other things.
 */

FVMEPreviewScene::FVMEPreviewScene(ConstructionValues CVS, float InFloorOffset)
    : FPreviewScene(CVS)
{
    DefaultSettings = UAssetViewerSettings::Get();
    check(DefaultSettings);

    // Initializing the delegate used to refresh the scene when settings change
    RefreshDelegate = DefaultSettings->OnAssetViewerSettingsChanged().AddRaw(this, &FVMEPreviewScene::OnAssetViewerSettingsChanged);
    
    // Initializing our settings profile
    CurrentProfile = DefaultSettings->Profiles.Num();
    DefaultSettings->Profiles.Add(FPreviewSceneProfile());
    if (!DefaultSettings->Profiles.IsValidIndex(CurrentProfile))
    {
        UE_LOG(LogNFToolEditor, Fatal, TEXT("Invalid default settings pointer or current profile index!"));
    }
    FPreviewSceneProfile& Profile = DefaultSettings->Profiles[CurrentProfile];

    // Updating the scene to our profile variables
    Profile.LoadEnvironmentMap();
    SetSkyCubemap(Profile.EnvironmentCubeMap.Get());
    SetSkyBrightness(Profile.SkyLightIntensity);
    SetLightDirection(Profile.DirectionalLightRotation);
    bRotateLighting = Profile.bRotateLightingRig;
    CurrentRotationSpeed = Profile.RotationSpeed;
    bSkyChanged = true;

    // Initializing the sky sphere
    const FTransform SkySphereTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector(2000.f)); // Size scale is big to avoid clipping
    SkySphere = NewObject<UStaticMeshComponent>(GetTransientPackage());
    
    UStaticMesh* SkySphereMesh = LoadObject<UStaticMesh>(NULL, TEXT("/Engine/EngineSky/SM_SkySphere.SM_SkySphere"), NULL, LOAD_None, NULL);
    check(SkySphereMesh);
    SkySphere->SetStaticMesh(SkySphereMesh);
    SkySphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SkySphere->bVisibleInRayTracing = false;
    
    UMaterial* SkySphereMaterial = LoadObject<UMaterial>(NULL, TEXT("/Engine/EngineSky/M_Sky_Panning_Clouds2.M_Sky_Panning_Clouds2"), NULL, LOAD_None, NULL);
    check(SkySphereMaterial);
    
    SkySphereMaterialInstance = NewObject<UMaterialInstanceConstant>(GetTransientPackage());
    SkySphereMaterialInstance->Parent = SkySphereMaterial;
    SkySphereMaterialInstance->PostLoad();
    
    SkySphere->SetMaterial(0, SkySphereMaterialInstance);
    AddComponent(SkySphere, SkySphereTransform);

    // Initializing the floor
    //const FTransform FloorTransform(FRotator::ZeroRotator, FVector(0.f, 0.f, -(InFloorOffset)), FVector(4.f, 4.f, 1.f));
    //UStaticMesh* FloorMesh = LoadObject<UStaticMesh>(NULL, TEXT("/Engine/EditorMeshes/AssetViewer/Floor_Mesh.Floor_Mesh"), NULL, LOAD_None, NULL);
    //check(FloorMesh);
    
    //Floor = NewObject<UStaticMeshComponent>(GetTransientPackage());
    //Floor->SetStaticMesh(FloorMesh);
    //AddComponent(Floor, FloorTransform);
    //SetFloorVisibility(false);

    // Initializing the post processing component
    const FTransform PostProcessTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::OneVector);
    PostProcessComponent = NewObject<UPostProcessComponent>(GetTransientPackage());
    PostProcessComponent->Settings = Profile.PostProcessingSettings;
    PostProcessComponent->bUnbound = true;
    AddComponent(PostProcessComponent, PostProcessTransform);

    // Initializing the grid actor
    if (GetWorld())
    {
        GetWorld()->GetWorldSettings()->NotifyBeginPlay();
        GetWorld()->GetWorldSettings()->NotifyMatchStarted();
        GetWorld()->GetWorldSettings()->SetActorHiddenInGame(false);
        GetWorld()->bEnableTraceCollision = 1;
        GetWorld()->bBegunPlay = true;

        FVector Location(0.0f, 0.0f, 0.0f);
        FRotator Rotation(0.0f, 0.0f, 0.0f);
        FActorSpawnParameters SpawnInfo;

        GridActor = GetWorld()->SpawnActor<AVoxelMeshEditorGridActor>(Location, Rotation, SpawnInfo);
        GridActor->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        GridActor->SetActorEnableCollision(true);

        MeshEditorActor = GetWorld()->SpawnActor<AVoxelMeshEditorActor>(Location, Rotation, SpawnInfo);
        MeshEditorActor->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        MeshEditorActor->SetActorEnableCollision(true);
    }

    BindCommands();

    // since advance preview scenes are used in conjunction with material/mesh editors we should match their feature level with the global one
    if (GIsEditor && GEditor != nullptr)
    {
        PreviewWorld->ChangeFeatureLevel(GEditor->DefaultWorldFeatureLevel);
    }
}

FVMEPreviewScene::~FVMEPreviewScene()
{
    if (UObjectInitialized())
    {
        // Cleaning up the scene refresh delegate
        DefaultSettings = UAssetViewerSettings::Get();
        if (DefaultSettings)
        {
            DefaultSettings->OnAssetViewerSettingsChanged().Remove(RefreshDelegate);
        }
    }
}

FLinearColor FVMEPreviewScene::GetBackgroundColor() const
{
    FLinearColor Color = DefaultSettings->Profiles[CurrentProfile].EnvironmentColor;
    float Intensity = DefaultSettings->Profiles[CurrentProfile].EnvironmentIntensity;
    return Color * Intensity;
}

void FVMEPreviewScene::Tick(float DeltaTime)
{
    if (!(DefaultSettings && DefaultSettings->Profiles.IsValidIndex(CurrentProfile)))
    {
        UE_LOG(LogNFToolEditor, Warning, TEXT("FVMEPreviewScene::Tick => Invalid default settings pointer or current profile index!"));
    }

    UpdateCaptureContents();
    FPreviewSceneProfile& Profile = DefaultSettings->Profiles[CurrentProfile];
    // Updating the light direction
    if (Profile.bRotateLightingRig)
    {
        CurrentRotationSpeed = Profile.RotationSpeed;
        Profile.LightingRigRotation = FMath::Fmod(FMath::Max(FMath::Min(Profile.LightingRigRotation + (CurrentRotationSpeed * DeltaTime), 360.0f), 0.0f), 360.0f);
    
        FRotator LightDir = GetLightDirection();
        LightDir.Yaw += DeltaTime * -CurrentRotationSpeed;
        SetLightDirection(LightDir);
        DefaultSettings->Profiles[CurrentProfile].DirectionalLightRotation = LightDir;
    }
    
    // Updating data when the rotation diffrence becomes big enough so it matters
    if (!FMath::IsNearlyEqual(PreviousRotation, Profile.LightingRigRotation, 0.05f))
    {
        SkyLight->SourceCubemapAngle = Profile.LightingRigRotation;
        SkyLight->SetCaptureIsDirty();
        SkyLight->MarkRenderStateDirty();
        SkyLight->UpdateSkyCaptureContents(PreviewWorld);
    
        
        SkySphereMaterialInstance->SetScalarParameterValueEditorOnly(FName("CubemapRotation"), Profile.LightingRigRotation / 360.0f);
        SkySphereMaterialInstance->PostEditChange();
    
        UReflectionCaptureComponent::UpdateReflectionCaptureContents(PreviewWorld);
        PreviewWorld->UpdateAllSkyCaptures();
    
        PreviousRotation = Profile.LightingRigRotation;
    }
    
    if (bSkyChanged)
    {
        SkyLight->SetCaptureIsDirty();
        SkyLight->MarkRenderStateDirty();
        SkyLight->UpdateSkyCaptureContents(PreviewWorld);
        
        SkySphereMaterialInstance->PostEditChange();
        bSkyChanged = false;
    }
}

TStatId FVMEPreviewScene::GetStatId() const
{
    return TStatId();
}

void FVMEPreviewScene::OnAssetViewerSettingsChanged(const FName& InPropertyName)
{
    if (DefaultSettings->Profiles.IsValidIndex(CurrentProfile))
    {
        const bool bNameNone = InPropertyName == NAME_None;

        const bool bUpdateEnvironment = (InPropertyName == GET_MEMBER_NAME_CHECKED(FPreviewSceneProfile, EnvironmentCubeMap)) || (InPropertyName == GET_MEMBER_NAME_CHECKED(FPreviewSceneProfile, LightingRigRotation) || (InPropertyName == GET_MEMBER_NAME_CHECKED(UAssetViewerSettings, Profiles)));
        const bool bUpdateSkyLight = bUpdateEnvironment || (InPropertyName == GET_MEMBER_NAME_CHECKED(FPreviewSceneProfile, SkyLightIntensity) || InPropertyName == GET_MEMBER_NAME_CHECKED(FPreviewSceneProfile, bUseSkyLighting) || (InPropertyName == GET_MEMBER_NAME_CHECKED(UAssetViewerSettings, Profiles)));
        const bool bUpdateDirectionalLight = (InPropertyName == GET_MEMBER_NAME_CHECKED(FPreviewSceneProfile, DirectionalLightIntensity)) || (InPropertyName == GET_MEMBER_NAME_CHECKED(FPreviewSceneProfile, DirectionalLightColor));
        const bool bUpdatePostProcessing = (InPropertyName == GET_MEMBER_NAME_CHECKED(FPreviewSceneProfile, PostProcessingSettings)) || (InPropertyName == GET_MEMBER_NAME_CHECKED(FPreviewSceneProfile, bPostProcessingEnabled));

        UpdateScene(DefaultSettings->Profiles[CurrentProfile], bUpdateSkyLight || bNameNone, bUpdateEnvironment || bNameNone, bUpdatePostProcessing || bNameNone, bUpdateDirectionalLight || bNameNone);
    }
}

void FVMEPreviewScene::LineTraceVoxelMeshEditor(bool bRemoveVoxels)
{
    if (!PreviewWorld)
    {
        return;
    }

    if (!ViewportClient)
    {
        return;
    }

    FVector WorldPosition;
    FVector WorldDirection;

    FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
        ViewportClient->Viewport,
        ViewportClient->GetScene(),
        ViewportClient->EngineShowFlags));

    FSceneView* View = ViewportClient->CalcSceneView(&ViewFamily);

    View->DeprojectFVector2D(FVector2D(MousePos), /*out*/ WorldPosition, /*out*/ WorldDirection);

    FHitResult HitResult;

    FCollisionQueryParams LineParams(SCENE_QUERY_STAT(FocusOnPoint), true);

    UKismetSystemLibrary::LineTraceSingle(GetWorld(), WorldPosition, WorldPosition + WorldDirection * HALF_WORLD_MAX, ETraceTypeQuery::TraceTypeQuery_MAX, true, {}, EDrawDebugTrace::None, HitResult, true);

    if (bRemoveVoxels)
    {
        if (EditMode == EVMEEditMode::EditMode)
        {
            MeshEditorActor->RemoveVoxel(HitResult);
        }
    }
    else
    {
        if (EditMode == EVMEEditMode::PaintMode)
        {
            MeshEditorActor->PaintVoxel(HitResult);
        }
        else if (EditMode == EVMEEditMode::EditMode)
        {
            MeshEditorActor->CreateVoxel(HitResult);
        }
    }
}

void FVMEPreviewScene::UpdateScene(FPreviewSceneProfile& Profile, bool bUpdateSkyLight, bool bUpdateEnvironment, bool bUpdatePostProcessing, bool bUpdateDirectionalLight)
{
    if (bUpdateSkyLight)
    {
        // Update only when the profile light intensity differs greater than or equal to 0.05f
        if (!FMath::IsNearlyEqual(SkyLight->Intensity, Profile.SkyLightIntensity, 0.05f))
        {
            SetSkyBrightness(Profile.SkyLightIntensity);
            SkySphereMaterialInstance->SetScalarParameterValueEditorOnly(FName("Intensity"), Profile.SkyLightIntensity);
            bSkyChanged = true;
        }
    }

    if (bUpdateEnvironment)
    {
        // Updating the skybox
        UTextureCube* TextureCube = Profile.EnvironmentCubeMap.LoadSynchronous();
        UTexture* Texture = TextureCube; // default initializing with TextureCube if GetTextureParameterValue would not change it 
        SkySphereMaterialInstance->GetTextureParameterValue(FName("SkyBox"), Texture);

        if (Texture != TextureCube)
        {
            SkySphereMaterialInstance->SetTextureParameterValueEditorOnly(FName("SkyBox"), TextureCube);
            SetSkyCubemap(TextureCube);
            bSkyChanged = true;
        }

        // updating the rotation of the skybox
        static const float OneOver360 = 1.f / 360.f;
        float Rotation = Profile.LightingRigRotation;
        SkySphereMaterialInstance->GetScalarParameterValue(FName("CubemapRotation"), Rotation);
        FRotator LightDir = GetLightDirection();

        // Test if the diffrence in rotation is enough to require an update
        const bool bLightRotationChanged = !FMath::IsNearlyEqual(Rotation, Profile.RotationSpeed);
        // Test if any change of light direction happend
        const bool bLightDirectionChanged = LightDir != Profile.DirectionalLightRotation;
        
        if (bLightRotationChanged || bLightDirectionChanged)
        {
            SkySphereMaterialInstance->SetScalarParameterValueEditorOnly(FName("CubemapRotation"), Profile.LightingRigRotation * OneOver360);
        
            // Update light direction as well
            LightDir.Yaw = -Profile.LightingRigRotation;
            SetLightDirection(LightDir);
            DefaultSettings->Profiles[CurrentProfile].DirectionalLightRotation = LightDir;
            SkyLight->SourceCubemapAngle = Profile.LightingRigRotation;
            bSkyChanged = true;
        }

    }

    if (bUpdatePostProcessing)
    {
        PostProcessComponent->Settings = Profile.PostProcessingSettings;
        PostProcessComponent->bEnabled = Profile.bPostProcessingEnabled;
        bPostProcessing = Profile.bPostProcessingEnabled;
    }

    if (bUpdateDirectionalLight)
    {
        // Update only when the profile light intensity differs greater than or equal to 0.05f
        if (!FMath::IsNearlyEqual(DirectionalLight->Intensity, Profile.DirectionalLightIntensity, 0.05f))
        {
            DirectionalLight->SetIntensity(Profile.DirectionalLightIntensity);
        }
        DirectionalLight->SetLightColor(Profile.DirectionalLightColor); 
    }

    if (SkySphere)
    {
        SkySphere->SetVisibility(Profile.bShowEnvironment, true);
    }
    if (SkyLight)
    {
        SkyLight->SetVisibility(Profile.bUseSkyLighting, true);
    }

    bRotateLighting = Profile.bRotateLightingRig;
    CurrentRotationSpeed = Profile.RotationSpeed;
}

const bool FVMEPreviewScene::HandleInputAxis(FViewport* InViewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, bool bGamepad)
{
    return false;
}

const bool FVMEPreviewScene::HandleInputKey(FViewport* InViewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed, bool bGamepad)
{
    FInputEventState InputState(InViewport, Key, Event);

    bool handledInput = false;

    if (Key == EKeys::LeftMouseButton && InputState.IsShiftButtonPressed() && Event == IE_Pressed)
    {
        LineTraceVoxelMeshEditor(false);
        handledInput = true;
    }
    if (Key == EKeys::RightMouseButton && InputState.IsShiftButtonPressed() && Event == IE_Pressed)
    {
        LineTraceVoxelMeshEditor(true);
        handledInput = true;
    }
    if (Key == EKeys::F)
    {
        ViewportClient->SetViewLocation(FVector(-800.f, 0.f, 400.f));
        ViewportClient->SetViewRotation(FRotator::ZeroRotator);
    }


    return false;
}

void FVMEPreviewScene::SetSkyRotation(float Rotation)
{
    if (!(DefaultSettings && DefaultSettings->Profiles.IsValidIndex(CurrentProfile)))
    {
        UE_LOG(LogNFToolEditor, Warning, TEXT("FVMEPreviewScene::SetSkyRotation => Invalid default settings pointer or current profile index!"));
    }
    
    float ClampedSkyRotation = FMath::Fmod(Rotation, 360.f);
    ClampedSkyRotation += (ClampedSkyRotation < 0.f) * 360.f;
    DefaultSettings->Profiles[CurrentProfile].LightingRigRotation = ClampedSkyRotation;
}

const float FVMEPreviewScene::GetSkyRotation() const
{
    if (!(DefaultSettings && DefaultSettings->Profiles.IsValidIndex(CurrentProfile)))
    {
        UE_LOG(LogNFToolEditor, Warning, TEXT("FVMEPreviewScene::GetSkyRotation => Invalid default settings pointer or current profile index!"));
    }
    return DefaultSettings->Profiles[CurrentProfile].LightingRigRotation;
}

void FVMEPreviewScene::SetFloorVisibility(bool bVisible, bool bDirect)
{
    // When not direct => Set visibility in profile and refresh the scene
    // Else => Set visibility directly on the component
    if (!bDirect)
    {
        FProperty* FloorProperty = FindFProperty<FProperty>(FPreviewSceneProfile::StaticStruct(), GET_MEMBER_NAME_CHECKED(FPreviewSceneProfile, bShowFloor));
        DefaultSettings->Profiles[CurrentProfile].bShowFloor = bVisible;

        FPropertyChangedEvent PropertyEvent(FloorProperty);
        DefaultSettings->PostEditChangeProperty(PropertyEvent);
    }
    else
    {
        Floor->SetVisibility(bVisible);
    }
}

const UStaticMeshComponent* FVMEPreviewScene::GetFloor() const
{
    if (!(DefaultSettings && DefaultSettings->Profiles.IsValidIndex(CurrentProfile)))
    {
        UE_LOG(LogNFToolEditor, Warning, TEXT("FVMEPreviewScene::GetSkyRotation => Invalid default settings pointer or current profile index!"));
    }

    return Floor;
}

void FVMEPreviewScene::SetEnvironmentVisibility(bool bVisible, bool bDirect)
{
    // When not direct => Set visibility in profile and refresh the scene
    // Else => Set visibility directly on the component
    if (!bDirect)
    {
        FProperty* EnvironmentProperty = FindFProperty<FProperty>(FPreviewSceneProfile::StaticStruct(), GET_MEMBER_NAME_CHECKED(FPreviewSceneProfile, bShowEnvironment));
        DefaultSettings->Profiles[CurrentProfile].bShowEnvironment = bVisible;

        FPropertyChangedEvent PropertyEvent(EnvironmentProperty);
        DefaultSettings->PostEditChangeProperty(PropertyEvent);
    }
    else
    {
        SkySphere->SetVisibility(bVisible);
    }
}

void FVMEPreviewScene::SetFloorOffset(float FloorOffset)
{
    const FTransform FloorTransform(FRotator::ZeroRotator, FVector(0.f, 0.f, -(FloorOffset)), FVector(4.f, 4.f, 1.f));

    Floor->SetRelativeTransform(FloorTransform);
}

void FVMEPreviewScene::SetProfileIndex(int32 ProfileIndex)
{
    CurrentProfile = ProfileIndex;
    DefaultSettings->Profiles[CurrentProfile].LoadEnvironmentMap();
    SetLightDirection(DefaultSettings->Profiles[CurrentProfile].DirectionalLightRotation);
    UpdateScene(DefaultSettings->Profiles[CurrentProfile]);
    DefaultSettings->OnAssetViewerSettingsChanged().Broadcast(NAME_None);
}

const int32 FVMEPreviewScene::GetCurrentProfileIndex() const
{
    return CurrentProfile;
}

const bool FVMEPreviewScene::IsUsingPostProcessing() const
{
    return bPostProcessing;
}

void FVMEPreviewScene::BindCommands()
{
    CommandList = MakeShared<FUICommandList>();
}

#undef LOCTEXT_NAMESPACE