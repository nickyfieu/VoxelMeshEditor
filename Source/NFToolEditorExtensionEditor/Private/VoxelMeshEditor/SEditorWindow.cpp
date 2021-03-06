// Copyright © 2021 Nicky Fieu, All Rights Reserved.


#include "SEditorWindow.h"
#include "EditorViewportClient.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "EditorPreviewScene.h"
#include "SEditorViewportClientToolBar.h"

#define LOCTEXT_NAMESPACE "VoxelMeshEditor"

void SVMEEditorWindow::Construct(const FArguments& InArgs)
{
    FPreviewScene::ConstructionValues ConstructValues;
    ConstructValues.SetCreatePhysicsScene(true);
    ConstructValues.ShouldSimulatePhysics(true);		
    ConstructValues.LightBrightness = 3;					
    ConstructValues.SkyBrightness = 1;						
    ConstructValues.bEditor = true;

    PreviewScene = MakeShareable(new FVMEPreviewScene(ConstructValues));

    Skylight = NewObject<USkyLightComponent>();
    PreviewScene->AddComponent(Skylight, FTransform::Identity);

    SkyAtmosphere = NewObject<USkyAtmosphereComponent>();
    PreviewScene->AddComponent(SkyAtmosphere, FTransform::Identity);

    //PreviewScene->SetFloorVisibility(false);
    PreviewScene->DirectionalLight->SetMobility(EComponentMobility::Movable);
    PreviewScene->DirectionalLight->CastShadows = true;
    PreviewScene->DirectionalLight->CastStaticShadows = true;
    PreviewScene->DirectionalLight->CastDynamicShadows = true;
    PreviewScene->DirectionalLight->SetIntensity(3);
    
    SEditorViewport::Construct(SEditorViewport::FArguments());
}

SVMEEditorWindow::~SVMEEditorWindow()
{
    if (EditorViewportClient.IsValid())
    {
        EditorViewportClient->Viewport = NULL;
    }
}

TSharedRef<FEditorViewportClient> SVMEEditorWindow::MakeEditorViewportClient()
{
    if (!EditorViewportClient.IsValid())
    {
        EditorViewportClient = MakeShareable(new FVMEEditorViewportClient(PreviewScene.Get(), SharedThis(this)));
    }
    
    return EditorViewportClient.ToSharedRef();
}

FVMEEditorViewportClient& SVMEEditorWindow::GetEditorViewportClient() const
{
    return *EditorViewportClient;
}

TSharedPtr<SWidget> SVMEEditorWindow::MakeViewportToolbar()
{
    TSharedPtr<SWidget> ToolBar;
    SAssignNew(ToolBar, SVMEEditorViewportToolBar)
        .Viewport(SharedThis(this));

    return SNew(SVerticalBox)
            .Visibility(EVisibility::SelfHitTestInvisible) +
            SVerticalBox::Slot()
            .AutoHeight()
            .VAlign(VAlign_Top)
            [
                ToolBar.ToSharedRef()
            ];
}

#undef LOCTEXT_NAMESPACE 
