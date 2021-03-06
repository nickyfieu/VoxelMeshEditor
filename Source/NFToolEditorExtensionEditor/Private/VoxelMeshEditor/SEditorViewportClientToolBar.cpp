// Copyright © 2021 Nicky Fieu, All Rights Reserved.


#include "SEditorViewportClientToolBar.h"
#include "SEditorViewportToolBarMenu.h"
#include "ToolMenus.h"
#include "VoxelMeshEditorContext.h"
#include "SEditorWindow.h"
#include "EditorCommands.h"
#include "../NFToolEditorModule.h"
#include "Widgets/Input/SSpinBox.h"
#include "EditorViewportClient.h"
#include "EditorViewportCommands.h"
#include "EditorPreviewScene.h"
#include "VoxelMeshEditorStyle.h"

#define LOCTEXT_NAMESPACE "VoxelMeshEditor"

void SVMEEditorViewportToolBar::Construct(const FArguments& InArgs)
{
    Viewport = InArgs._Viewport;

    const FMargin ToolbarSlotPadding(4.f, 1.f);

    BindCommands();

    ChildSlot
    [
        SNew(SBorder)
        .BorderImage(FAppStyle::Get().GetBrush("EditorViewportToolBar.Background"))
        .Cursor(EMouseCursor::Default)
        [
            SNew(SHorizontalBox) +
            SHorizontalBox::Slot()
            .AutoWidth()
            .Padding(ToolbarSlotPadding)
            [
                SNew(SEditorViewportToolbarMenu)
                .ParentToolBar(SharedThis(this))
                .Visibility(EVisibility::Visible)
                .Image("EditorViewportToolBar.OptionsDropdown")
                .AddMetaData<FTagMetaData>(FTagMetaData(TEXT("VMEEditorViewportToolBar.Options")))
                .OnGetMenuContent(this, &SVMEEditorViewportToolBar::GenerateViewportOptionsMenu)
            ] +
            SHorizontalBox::Slot()
            .AutoWidth()
            .Padding(ToolbarSlotPadding)
            [
                SNew(SEditorViewportToolbarMenu)
                .ParentToolBar(SharedThis(this))
                .Label(this, &SVMEEditorViewportToolBar::GetCameraMenuLabel)
                .LabelIcon(this, &SVMEEditorViewportToolBar::GetCameraMenuLabelIcon)
                .AddMetaData<FTagMetaData>(FTagMetaData(TEXT("VMEEditorViewportToolBar.Camera")))
                .OnGetMenuContent(this, &SVMEEditorViewportToolBar::GenerateCameraSettings)
            ] +
            SHorizontalBox::Slot()
            .AutoWidth()
            .Padding(ToolbarSlotPadding)
            [
                SNew(SEditorViewportToolbarMenu)
                .ParentToolBar(SharedThis(this))
                .Label(this, &SVMEEditorViewportToolBar::GetViewModeMenuLabel)
                .LabelIcon(this, &SVMEEditorViewportToolBar::GetViewModeMenuIcon)
                .AddMetaData<FTagMetaData>(FTagMetaData(TEXT("VMEEditorViewportToolBar.ViewMode")))
                .OnGetMenuContent(this, &SVMEEditorViewportToolBar::GenerateViewModeMenu)
            ] +
            SHorizontalBox::Slot()
            .AutoWidth()
            .Padding(ToolbarSlotPadding)
            [
                GenerateEditorModeMenu()
            ]
        ]
    ];
}

TSharedRef<SWidget> SVMEEditorViewportToolBar::GenerateViewportOptionsMenu()
{
    static const FName MenuName("VoxelMeshEditor.EditorViewportToolBar.ViewportOptions");
    // Register the menu if not registered yet
    if (!UToolMenus::Get()->IsMenuRegistered(MenuName))
    {
        UToolMenu* Menu = UToolMenus::Get()->RegisterMenu(MenuName);
        Menu->AddDynamicSection(
            "DynamicSection", 
            FNewToolMenuDelegate::CreateLambda(
                [](UToolMenu* InMenu)
                {
                    UVMEEditorViewportToolBarContext* Context = InMenu->FindContext<UVMEEditorViewportToolBarContext>();
                    Context->ToolBarWidget.Pin()->FillViewportOptions(InMenu);
                }
            )
        );
    }

    // Initializing data to create the tool menu context object
    TSharedPtr<FExtender> MenuExtender = MakeShared<FExtender>();
    UVMEEditorViewportToolBarContext* ContextObject = NewObject<UVMEEditorViewportToolBarContext>();
    ContextObject->ToolBarWidget = SharedThis(this);
    FToolMenuContext MenuContext(CommandList, MenuExtender, ContextObject);

    return UToolMenus::Get()->GenerateWidget(MenuName, MenuContext);
}

void SVMEEditorViewportToolBar::FillViewportOptions(UToolMenu* Menu)
{
    bool bIsPerspective = Viewport.Pin()->GetEditorViewportClient().IsPerspective();

    {
        FToolMenuSection& Section = Menu->AddSection(
            "VMEEditorToolBarViewportOptions", LOCTEXT("VMEEditorToolBarViewportOptions", "Viewport Options")
        );

        if (bIsPerspective)
        {
            Section.AddEntry(FToolMenuEntry::InitWidget("FOVAngle", GenerateFOVMenu(), LOCTEXT("FOVAngle", "Field of View (H)")));
            Section.AddEntry(FToolMenuEntry::InitWidget("FarViewPlane", GenerateFarViewPlaneMenu(), LOCTEXT("FarViewPlane", "Far View Plane")));
        }
    }
}

TSharedRef<SWidget> SVMEEditorViewportToolBar::GenerateFOVMenu()
{
    const float FOVMin = 10.f;
    const float FOVMax = 170.f;

    return SNew(SBox)
            .HAlign(HAlign_Right)
            [
                SNew(SBox)
                .Padding(FMargin(4.0f, 0.0f, 0.0f, 0.0f))
                .WidthOverride(100.0f)
                [
                    SNew(SBorder)
                    .BorderImage(FAppStyle::Get().GetBrush("Menu.WidgetBorder"))
                    .Padding(FMargin(1.0f))
                    [
                        SNew(SSpinBox<float>)
                        .Style(&FAppStyle::Get(), "Menu.SpinBox")
                        .Font(FEditorStyle::GetFontStyle(TEXT("MenuItem.Font")))
                        .MinValue(FOVMin)
                        .MaxValue(FOVMax)
                        .Value(this, &SVMEEditorViewportToolBar::OnGetFOVValue)
                        .OnValueChanged(this, &SVMEEditorViewportToolBar::OnFOVValueChanged)
                    ]
                ]
            ];
}

float SVMEEditorViewportToolBar::OnGetFOVValue() const
{
    return Viewport.Pin()->GetEditorViewportClient().ViewFOV;
}

void SVMEEditorViewportToolBar::OnFOVValueChanged(float NewValue)
{
    bool bUpdateStoredFOV = true;
    FVMEEditorViewportClient& ViewportClient = Viewport.Pin()->GetEditorViewportClient();

    if (bUpdateStoredFOV)
    {
        ViewportClient.FOVAngle = NewValue;
    }

    ViewportClient.ViewFOV = NewValue;
    ViewportClient.Invalidate();
}

TSharedRef<SWidget> SVMEEditorViewportToolBar::GenerateFarViewPlaneMenu()
{
    return
        SNew(SBox)
        .HAlign(HAlign_Right)
        [
            SNew(SBox)
            .Padding(FMargin(4.0f, 0.0f, 0.0f, 0.0f))
            .WidthOverride(100.0f)
            [
                SNew(SBorder)
                .BorderImage(FAppStyle::Get().GetBrush("Menu.WidgetBorder"))
                .Padding(FMargin(1.0f))
                [
                    SNew(SSpinBox<float>)
                    .Style(&FAppStyle::Get(), "Menu.SpinBox")
                    .ToolTipText(LOCTEXT("FarViewPlaneTooltip", "Distance to use as the far view plane, or zero to enable an infinite far view plane"))
                    .MinValue(0.0f)
                    .MaxValue(100000.0f)
                    .Font(FEditorStyle::GetFontStyle(TEXT("MenuItem.Font")))
                    .Value(this, &SVMEEditorViewportToolBar::OnGetFarViewPlaneValue)
                    .OnValueChanged(this, &SVMEEditorViewportToolBar::OnFarViewPlaneValueChanged)
                ]
            ]
        ];
}

float SVMEEditorViewportToolBar::OnGetFarViewPlaneValue() const
{
    return Viewport.Pin()->GetEditorViewportClient().GetFarClipPlaneOverride();
}

void SVMEEditorViewportToolBar::OnFarViewPlaneValueChanged(float NewValue)
{
    Viewport.Pin()->GetEditorViewportClient().OverrideFarClipPlane(NewValue);
}

FText SVMEEditorViewportToolBar::GetCameraMenuLabel() const
{
    if (Viewport.Pin().IsValid())
    {
        return GetCameraMenuLabelFromViewportType(Viewport.Pin()->GetEditorViewportClient().ViewportType);
    }

    return LOCTEXT("CameraMenuTitle_Default", "Camera");
}

const FSlateBrush* SVMEEditorViewportToolBar::GetCameraMenuLabelIcon() const
{
    if (Viewport.Pin().IsValid())
    {
        return GetCameraMenuLabelIconFromViewportType(Viewport.Pin()->GetEditorViewportClient().ViewportType);
    }

    return FStyleDefaults::GetNoBrush();
}

TSharedRef<SWidget> SVMEEditorViewportToolBar::GenerateCameraSettings()
{
    static const FName MenuName("VoxelMeshEditor.EditorViewportToolBar.CameraSettings");
    // Register the menu if not registered yet
    if (!UToolMenus::Get()->IsMenuRegistered(MenuName))
    {
        UToolMenu* Menu = UToolMenus::Get()->RegisterMenu(MenuName);
        Menu->AddDynamicSection(
            "DynamicSection",
            FNewToolMenuDelegate::CreateLambda(
                [](UToolMenu* InMenu)
                {
                    UVMEEditorViewportToolBarContext* Context = InMenu->FindContext<UVMEEditorViewportToolBarContext>();
                    Context->ToolBarWidget.Pin()->FillCameraSettings(InMenu);
                }
            )
        );
    }

    // Initializing data to create the tool menu context object
    TSharedPtr<FExtender> MenuExtender = MakeShared<FExtender>();
    UVMEEditorViewportToolBarContext* ContextObject = NewObject<UVMEEditorViewportToolBarContext>();
    ContextObject->ToolBarWidget = SharedThis(this);
    FToolMenuContext MenuContext(CommandList, MenuExtender, ContextObject);

    return UToolMenus::Get()->GenerateWidget(MenuName, MenuContext);
}

void SVMEEditorViewportToolBar::FillCameraSettings(UToolMenu* Menu)
{
    {
        FToolMenuSection& Section = Menu->AddSection("VMEEditorViewportCameraSettings", LOCTEXT("VMECameraSettingsHeader", "Camera Settings"));
        Section.AddMenuEntry(FVMEEditorCommands::Get().Perspective, TAttribute<FText>(), TAttribute<FText>(), FSlateIcon(FEditorStyle::GetStyleSetName(), "EditorViewport.Perspective"));
    }

    {
        FToolMenuSection& Section = Menu->AddSection("VMEEditorViewportCameraType_Ortho", LOCTEXT("VMECameraTypeHeader_Ortho", "Orthographic"));
        Section.AddMenuEntry(FVMEEditorCommands::Get().Top      , TAttribute<FText>(), TAttribute<FText>(), FSlateIcon(FEditorStyle::GetStyleSetName(), "EditorViewport.Top"));
        Section.AddMenuEntry(FVMEEditorCommands::Get().Bottom   , TAttribute<FText>(), TAttribute<FText>(), FSlateIcon(FEditorStyle::GetStyleSetName(), "EditorViewport.Bottom"));
        Section.AddMenuEntry(FVMEEditorCommands::Get().Left     , TAttribute<FText>(), TAttribute<FText>(), FSlateIcon(FEditorStyle::GetStyleSetName(), "EditorViewport.Left"));
        Section.AddMenuEntry(FVMEEditorCommands::Get().Right    , TAttribute<FText>(), TAttribute<FText>(), FSlateIcon(FEditorStyle::GetStyleSetName(), "EditorViewport.Right"));
        Section.AddMenuEntry(FVMEEditorCommands::Get().Front    , TAttribute<FText>(), TAttribute<FText>(), FSlateIcon(FEditorStyle::GetStyleSetName(), "EditorViewport.Front"));
        Section.AddMenuEntry(FVMEEditorCommands::Get().Back     , TAttribute<FText>(), TAttribute<FText>(), FSlateIcon(FEditorStyle::GetStyleSetName(), "EditorViewport.Back"));
    }
}

FText SVMEEditorViewportToolBar::GetViewModeMenuLabel() const
{
    EViewModeIndex ViewMode = Viewport.Pin()->GetEditorViewportClient().GetViewMode();
    return UViewModeUtils::GetViewModeDisplayName(ViewMode);
}

const FSlateBrush* SVMEEditorViewportToolBar::GetViewModeMenuIcon() const
{
    EViewModeIndex ViewMode = Viewport.Pin()->GetEditorViewportClient().GetViewMode();
    return UViewModeUtils::GetViewModeDisplayIcon(ViewMode);
}

TSharedRef<SWidget> SVMEEditorViewportToolBar::GenerateViewModeMenu()
{
    static const FName MenuName("VoxelMeshEditor.EditorViewportToolBar.ViewMode");
    // Register the menu if not registered yet
    if (!UToolMenus::Get()->IsMenuRegistered(MenuName))
    {
        UToolMenu* Menu = UToolMenus::Get()->RegisterMenu(MenuName);
        Menu->AddDynamicSection(
            "DynamicSection",
            FNewToolMenuDelegate::CreateLambda(
                [](UToolMenu* InMenu)
                {
                    UVMEEditorViewportToolBarContext* Context = InMenu->FindContext<UVMEEditorViewportToolBarContext>();
                    Context->ToolBarWidget.Pin()->FillViewModeMenu(InMenu);
                }
            )
        );
    }

    // Initializing data to create the tool menu context object
    TSharedPtr<FExtender> MenuExtender = MakeShared<FExtender>();
    UVMEEditorViewportToolBarContext* ContextObject = NewObject<UVMEEditorViewportToolBarContext>();
    ContextObject->ToolBarWidget = SharedThis(this);
    FToolMenuContext MenuContext(CommandList, MenuExtender, ContextObject);

    return UToolMenus::Get()->GenerateWidget(MenuName, MenuContext);
}

void SVMEEditorViewportToolBar::FillViewModeMenu(UToolMenu* Menu)
{
    {
        FToolMenuSection& Section = Menu->AddSection("VMEEditorViewportViewModeOptions", LOCTEXT("VMEViewModeHeader", "View Mode"));
        Section.AddMenuEntry(FVMEEditorCommands::Get().LitMode              , UViewModeUtils::GetViewModeDisplayName(VMI_Lit)                   , TAttribute<FText>(), FSlateIcon(FEditorStyle::GetStyleSetName(), "EditorViewport.LitMode"));
        Section.AddMenuEntry(FVMEEditorCommands::Get().UnlitMode            , UViewModeUtils::GetViewModeDisplayName(VMI_Unlit)                 , TAttribute<FText>(), FSlateIcon(FEditorStyle::GetStyleSetName(), "EditorViewport.UnlitMode"));
        Section.AddMenuEntry(FVMEEditorCommands::Get().WireframeMode        , UViewModeUtils::GetViewModeDisplayName(VMI_BrushWireframe)        , TAttribute<FText>(), FSlateIcon(FEditorStyle::GetStyleSetName(), "EditorViewport.WireframeMode"));
        Section.AddMenuEntry(FVMEEditorCommands::Get().DetailLightingMode   , UViewModeUtils::GetViewModeDisplayName(VMI_Lit_DetailLighting)    , TAttribute<FText>(), FSlateIcon(FEditorStyle::GetStyleSetName(), "EditorViewport.DetailLightingMode"));
        Section.AddMenuEntry(FVMEEditorCommands::Get().LightingOnlyMode     , UViewModeUtils::GetViewModeDisplayName(VMI_LightingOnly)          , TAttribute<FText>(), FSlateIcon(FEditorStyle::GetStyleSetName(), "EditorViewport.LightingOnlyMode"));
        Section.AddMenuEntry(FVMEEditorCommands::Get().VisibilityCollision  , UViewModeUtils::GetViewModeDisplayName(VMI_CollisionVisibility)   , TAttribute<FText>(), FSlateIcon(FEditorStyle::GetStyleSetName(), "EditorViewport.CollisionVisibility"));
    }
}

TSharedRef<SWidget> SVMEEditorViewportToolBar::GenerateEditorModeMenu()
{
    FSlimHorizontalToolBarBuilder ToolbarBuilder(CommandList, FMultiBoxCustomization::None, MakeShared<FExtender>());

    // Use a custom style
    FName ToolBarStyle = "VoxelMeshEditorToolBar";
    ToolbarBuilder.SetStyle(&FVoxelMeshEditorStyle::GetInstance(), ToolBarStyle);
    ToolbarBuilder.SetLabelVisibility(EVisibility::Collapsed);

    // Edit mode controls cannot be focusable as it fights with the press space to change transform mode feature
    ToolbarBuilder.SetIsFocusable(false);

    {
        ToolbarBuilder.BeginSection("VoxelMeshEditorModes");
        ToolbarBuilder.BeginBlockGroup();

        // Edit Mode
        static FName EditModeName = FName(TEXT("EditMode"));
        ToolbarBuilder.AddToolBarButton(FVMEEditorCommands::Get().EditMode, NAME_None, TAttribute<FText>(), TAttribute<FText>(), FSlateIcon(FVoxelMeshEditorStyle::GetInstance().GetStyleSetName(), "VoxelMeshEditorToolBar.EditMode", ToolBarStyle), EditModeName);
        
        // Paint Mode
        static FName PaintModeName = FName(TEXT("PaintMode"));
        ToolbarBuilder.AddToolBarButton(FVMEEditorCommands::Get().PaintMode, NAME_None, TAttribute<FText>(), TAttribute<FText>(), FSlateIcon(FVoxelMeshEditorStyle::GetInstance().GetStyleSetName(), "VoxelMeshEditorToolBar.PaintMode", ToolBarStyle), PaintModeName);

        ToolbarBuilder.EndBlockGroup();
        ToolbarBuilder.EndSection();
    }

    return ToolbarBuilder.MakeWidget();
}

void SVMEEditorViewportToolBar::BindCommands()
{
    CommandList = MakeShared<FUICommandList>();

    FVMEEditorCommands::Register();

    const FVMEEditorCommands& Commands = FVMEEditorCommands::Get();

    TSharedRef<FVMEEditorViewportClient> ViewportClientRef = StaticCastSharedRef<FVMEEditorViewportClient>(Viewport.Pin()->GetViewportClient().ToSharedRef());

    CommandList->MapAction(
        Commands.Perspective,
        FExecuteAction::CreateSP(ViewportClientRef, &FEditorViewportClient::SetViewportType, ELevelViewportType::LVT_Perspective),
        FCanExecuteAction(),
        FIsActionChecked::CreateSP(ViewportClientRef, &FEditorViewportClient::IsActiveViewportType, ELevelViewportType::LVT_Perspective)
    );

    CommandList->MapAction(
        Commands.Top,
        FExecuteAction::CreateSP(ViewportClientRef, &FEditorViewportClient::SetViewportType, ELevelViewportType::LVT_OrthoXY),
        FCanExecuteAction(),
        FIsActionChecked::CreateSP(ViewportClientRef, &FEditorViewportClient::IsActiveViewportType, ELevelViewportType::LVT_OrthoXY)
    );

    CommandList->MapAction(
        Commands.Bottom,
        FExecuteAction::CreateSP(ViewportClientRef, &FEditorViewportClient::SetViewportType, ELevelViewportType::LVT_OrthoNegativeXY),
        FCanExecuteAction(),
        FIsActionChecked::CreateSP(ViewportClientRef, &FEditorViewportClient::IsActiveViewportType, ELevelViewportType::LVT_OrthoNegativeXY)
    );

    CommandList->MapAction(
        Commands.Left,
        FExecuteAction::CreateSP(ViewportClientRef, &FEditorViewportClient::SetViewportType, ELevelViewportType::LVT_OrthoNegativeXZ),
        FCanExecuteAction(),
        FIsActionChecked::CreateSP(ViewportClientRef, &FEditorViewportClient::IsActiveViewportType, ELevelViewportType::LVT_OrthoNegativeXZ)
    );

    CommandList->MapAction(
        Commands.Right,
        FExecuteAction::CreateSP(ViewportClientRef, &FEditorViewportClient::SetViewportType, ELevelViewportType::LVT_OrthoXZ),
        FCanExecuteAction(),
        FIsActionChecked::CreateSP(ViewportClientRef, &FEditorViewportClient::IsActiveViewportType, ELevelViewportType::LVT_OrthoXZ)
    );

    CommandList->MapAction(
        Commands.Front,
        FExecuteAction::CreateSP(ViewportClientRef, &FEditorViewportClient::SetViewportType, ELevelViewportType::LVT_OrthoNegativeYZ),
        FCanExecuteAction(),
        FIsActionChecked::CreateSP(ViewportClientRef, &FEditorViewportClient::IsActiveViewportType, ELevelViewportType::LVT_OrthoNegativeYZ)
    );

    CommandList->MapAction(
        Commands.Back,
        FExecuteAction::CreateSP(ViewportClientRef, &FEditorViewportClient::SetViewportType, ELevelViewportType::LVT_OrthoYZ),
        FCanExecuteAction(),
        FIsActionChecked::CreateSP(ViewportClientRef, &FEditorViewportClient::IsActiveViewportType, ELevelViewportType::LVT_OrthoYZ)
    );

    CommandList->MapAction(
        Commands.LitMode,
        FExecuteAction::CreateSP(ViewportClientRef, &FEditorViewportClient::SetViewMode, EViewModeIndex::VMI_Lit),
        FCanExecuteAction(),
        FIsActionChecked::CreateSP(ViewportClientRef, &FEditorViewportClient::IsViewModeEnabled, EViewModeIndex::VMI_Lit)
    );
    
    CommandList->MapAction(
        Commands.UnlitMode,
        FExecuteAction::CreateSP(ViewportClientRef, &FEditorViewportClient::SetViewMode, EViewModeIndex::VMI_Unlit),
        FCanExecuteAction(),
        FIsActionChecked::CreateSP(ViewportClientRef, &FEditorViewportClient::IsViewModeEnabled, EViewModeIndex::VMI_Unlit)
    );
    
    CommandList->MapAction(
        Commands.WireframeMode,
        FExecuteAction::CreateSP(ViewportClientRef, &FEditorViewportClient::SetViewMode, EViewModeIndex::VMI_Wireframe),
        FCanExecuteAction(),
        FIsActionChecked::CreateSP(ViewportClientRef, &FEditorViewportClient::IsViewModeEnabled, EViewModeIndex::VMI_Wireframe)
    );
    
    CommandList->MapAction(
        Commands.DetailLightingMode,
        FExecuteAction::CreateSP(ViewportClientRef, &FEditorViewportClient::SetViewMode, EViewModeIndex::VMI_Lit_DetailLighting),
        FCanExecuteAction(),
        FIsActionChecked::CreateSP(ViewportClientRef, &FEditorViewportClient::IsViewModeEnabled, EViewModeIndex::VMI_Lit_DetailLighting)
    );
    
    CommandList->MapAction(
        Commands.LightingOnlyMode,
        FExecuteAction::CreateSP(ViewportClientRef, &FEditorViewportClient::SetViewMode, EViewModeIndex::VMI_LightingOnly),
        FCanExecuteAction(),
        FIsActionChecked::CreateSP(ViewportClientRef, &FEditorViewportClient::IsViewModeEnabled, EViewModeIndex::VMI_LightingOnly)
    );

    CommandList->MapAction(
        Commands.VisibilityCollision,
        FExecuteAction::CreateSP(ViewportClientRef, &FEditorViewportClient::SetViewMode, EViewModeIndex::VMI_CollisionVisibility),
        FCanExecuteAction(),
        FIsActionChecked::CreateSP(ViewportClientRef, &FEditorViewportClient::IsViewModeEnabled, EViewModeIndex::VMI_CollisionVisibility)
    );

    CommandList->MapAction(
        Commands.EditMode,
        FExecuteAction::CreateSP(ViewportClientRef, &FVMEEditorViewportClient::SetEditMode, EVMEEditMode::EditMode),
        FCanExecuteAction::CreateSP(ViewportClientRef, &FVMEEditorViewportClient::CanSetEditMode, EVMEEditMode::EditMode),
        FIsActionChecked::CreateSP(ViewportClientRef, &FVMEEditorViewportClient::IsEditModeActive, EVMEEditMode::EditMode)
    );

    CommandList->MapAction(
        Commands.PaintMode,
        FExecuteAction::CreateSP(ViewportClientRef, &FVMEEditorViewportClient::SetEditMode, EVMEEditMode::PaintMode),
        FCanExecuteAction::CreateSP(ViewportClientRef, &FVMEEditorViewportClient::CanSetEditMode, EVMEEditMode::PaintMode),
        FIsActionChecked::CreateSP(ViewportClientRef, &FVMEEditorViewportClient::IsEditModeActive, EVMEEditMode::PaintMode)
    );
}

#undef LOCTEXT_NAMESPACE