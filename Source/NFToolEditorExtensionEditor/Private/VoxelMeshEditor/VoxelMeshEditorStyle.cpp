// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#include "VoxelMeshEditorStyle.h"
#include <Framework/Application/SlateApplication.h>
#include <Styling/SlateStyleRegistry.h>
#include <Styling/SlateStyleMacros.h>
#include <Styling/ToolBarStyle.h>
#include <Styling/StarshipCoreStyle.h>
#include <Styling/StyleColors.h>
#include <Styling/SlateTypes.h>

#define LOCTEXT_NAMESPACE "VoxelMeshEditor"

FName FVoxelMeshEditorStyle::StyleName("VoxelMeshEditorStyle");
TSharedPtr<FVoxelMeshEditorStyle> FVoxelMeshEditorStyle::Instance(nullptr);

FVoxelMeshEditorStyle::FVoxelMeshEditorStyle()
    : FSlateStyleSet(StyleName)
{
    SetCoreContentRoot( FPaths::EngineContentDir() / TEXT("Slate") );
    SetContentRoot( FPaths::EngineContentDir() / TEXT("Editor/Slate") );
    Set("VoxelMeshEditor.Save", new IMAGE_BRUSH_SVG(TEXT("Starship/Common/SaveCurrent"), Icon32x32));
    Set("VoxelMeshEditor.Load", new CORE_IMAGE_BRUSH_SVG(TEXT("Starship/Common/folder-closed"), Icon32x32));
    Set("VoxelMeshEditor.Export", new IMAGE_BRUSH_SVG(TEXT("Starship/Common/Export"), Icon32x32));
    Set("VoxelMeshEditorToolBar.PaintMode", new IMAGE_BRUSH_SVG("Starship/MainToolbar/paint", Icon16x16));
    Set("VoxelMeshEditorToolBar.EditMode", new IMAGE_BRUSH_SVG("Starship/Common/PlaceActors", Icon16x16));

	{	// Initializing the voxel mesh editor viewport tool bar style
		FToolBarStyle VMEToolbarStyle = FStarshipCoreStyle::GetCoreStyle().GetWidgetStyle<FToolBarStyle>("SlimToolBar");

		FLinearColor TransInput = FStyleColors::Input.GetSpecifiedColor();
		TransInput.A = .71;

		FMargin ViewportMargin(4.f, 4.f);

		FSlateRoundedBoxBrush* VMEGroupBrush = new FSlateRoundedBoxBrush(TransInput, 12.f, FStyleColors::Dropdown, 1.0);
		Set("VoxelMeshEditorToolBar.Group", VMEGroupBrush);

		FButtonStyle VMEMenuButton = FButtonStyle()
			.SetNormal(*VMEGroupBrush)
			.SetHovered(*VMEGroupBrush)
			.SetPressed(*VMEGroupBrush)
			.SetNormalForeground(FStyleColors::Foreground)
			.SetHoveredForeground(FStyleColors::ForegroundHover)
			.SetPressedForeground(FStyleColors::ForegroundHover)
			.SetDisabledForeground(FStyleColors::Foreground)
			.SetNormalPadding(ViewportMargin)
			.SetPressedPadding(ViewportMargin);
		Set("VoxelMeshEditorToolBar.Button", VMEMenuButton);

		const FCheckBoxStyle VMEToggleLeftButtonStyle = FCheckBoxStyle(VMEToolbarStyle.ToggleButton)
			.SetCheckBoxType(ESlateCheckBoxType::ToggleButton)
			.SetUncheckedImage(BOX_BRUSH("Starship/EditorViewport/ToolBarLeftGroup", 12.f / 25.f))
			.SetUncheckedPressedImage(BOX_BRUSH("Starship/EditorViewport/ToolBarLeftGroup", 12.f / 25.f))
			.SetUncheckedHoveredImage(BOX_BRUSH("Starship/EditorViewport/ToolBarLeftGroup", 12.f / 25.f))
			.SetCheckedHoveredImage(BOX_BRUSH("Starship/EditorViewport/ToolBarLeftGroup", 12.f / 25.f))
			.SetCheckedPressedImage(BOX_BRUSH("Starship/EditorViewport/ToolBarLeftGroup", 12.f / 25.f))
			.SetCheckedImage(BOX_BRUSH("Starship/EditorViewport/ToolBarLeftGroup", 12.f / 25.f))
			.SetPadding(ViewportMargin);
		Set("VoxelMeshEditorToolBar.ToggleButton.Start", VMEToggleLeftButtonStyle);

		const FCheckBoxStyle ViewportMenuToggleMiddleButtonStyle = FCheckBoxStyle(VMEToolbarStyle.ToggleButton)
			.SetCheckBoxType(ESlateCheckBoxType::ToggleButton)
			.SetUncheckedImage(BOX_BRUSH("Starship/EditorViewport/ToolBarMiddleGroup", 12.f / 25.f))
			.SetUncheckedPressedImage(BOX_BRUSH("Starship/EditorViewport/ToolBarMiddleGroup", 12.f / 25.f))
			.SetUncheckedHoveredImage(BOX_BRUSH("Starship/EditorViewport/ToolBarMiddleGroup", 12.f / 25.f))
			.SetCheckedHoveredImage(BOX_BRUSH("Starship/EditorViewport/ToolBarMiddleGroup", 12.f / 25.f))
			.SetCheckedPressedImage(BOX_BRUSH("Starship/EditorViewport/ToolBarMiddleGroup", 12.f / 25.f))
			.SetCheckedImage(BOX_BRUSH("Starship/EditorViewport/ToolBarMiddleGroup", 12.f / 25.f))
			.SetPadding(ViewportMargin);
		Set("VoxelMeshEditorToolBar.ToggleButton.Middle", ViewportMenuToggleMiddleButtonStyle);

		const FCheckBoxStyle ViewportMenuToggleRightButtonStyle = FCheckBoxStyle(VMEToolbarStyle.ToggleButton)
			.SetCheckBoxType(ESlateCheckBoxType::ToggleButton)
			.SetUncheckedImage(BOX_BRUSH("Starship/EditorViewport/ToolBarRightGroup", 12.f / 25.f))
			.SetUncheckedPressedImage(BOX_BRUSH("Starship/EditorViewport/ToolBarRightGroup", 12.f / 25.f))
			.SetUncheckedHoveredImage(BOX_BRUSH("Starship/EditorViewport/ToolBarRightGroup", 12.f / 25.f))
			.SetCheckedHoveredImage(BOX_BRUSH("Starship/EditorViewport/ToolBarRightGroup", 12.f / 25.f))
			.SetCheckedPressedImage(BOX_BRUSH("Starship/EditorViewport/ToolBarRightGroup", 12.f / 25.f))
			.SetCheckedImage(BOX_BRUSH("Starship/EditorViewport/ToolBarRightGroup", 12.f / 25.f))
			.SetPadding(ViewportMargin);
		Set("VoxelMeshEditorToolBar.ToggleButton.End", ViewportMenuToggleRightButtonStyle);

		// We want a background-less version as the ComboMenu has its own unified background
		const FToolBarStyle& SlimCoreToolBarStyle = FStarshipCoreStyle::GetCoreStyle().GetWidgetStyle<FToolBarStyle>("SlimToolBar");
		Set("VoxelMeshEditorToolBar.ComboMenu.ButtonStyle", FButtonStyle(SlimCoreToolBarStyle.ButtonStyle).SetNormalPadding(0.0).SetPressedPadding(0.0));
		Set("VoxelMeshEditorToolBar.ComboMenu.ToggleButton", FCheckBoxStyle(SlimCoreToolBarStyle.ToggleButton).SetPadding(0.0));
		Set("VoxelMeshEditorToolBar.ComboMenu.LabelStyle", SlimCoreToolBarStyle.LabelStyle);

		FCheckBoxStyle MaximizeRestoreButton = FCheckBoxStyle(VMEToolbarStyle.ToggleButton)
			.SetUncheckedImage(*VMEGroupBrush)
			.SetUncheckedPressedImage(*VMEGroupBrush)
			.SetUncheckedHoveredImage(*VMEGroupBrush)
			.SetCheckedImage(*VMEGroupBrush)
			.SetCheckedHoveredImage(*VMEGroupBrush)
			.SetCheckedPressedImage(*VMEGroupBrush)
			.SetForegroundColor(FStyleColors::Foreground)
			.SetPressedForegroundColor(FStyleColors::ForegroundHover)
			.SetHoveredForegroundColor(FStyleColors::ForegroundHover)
			.SetCheckedForegroundColor(FStyleColors::Foreground)
			.SetCheckedPressedForegroundColor(FStyleColors::ForegroundHover)
			.SetCheckedHoveredForegroundColor(FStyleColors::ForegroundHover)
			.SetPadding(ViewportMargin);
		Set("VoxelMeshEditorToolBar.MaximizeRestoreButton", MaximizeRestoreButton);

		Set("VoxelMeshEditorToolBar.Heading.Padding", FMargin(4.f));


		// SComboBox 
		FComboButtonStyle ViewportComboButton = FComboButtonStyle()
			.SetButtonStyle(VMEMenuButton)
			.SetContentPadding(ViewportMargin);

		VMEToolbarStyle
			.SetBackground(FSlateNoResource())
			.SetIconSize(Icon16x16)
			.SetBackgroundPadding(FMargin(0))
			.SetLabelPadding(FMargin(0))
			.SetComboButtonPadding(FMargin(4.f, 0.0f))
			.SetBlockPadding(FMargin(0.0f, 0.0f))
			.SetIndentedBlockPadding(FMargin(0))
			.SetButtonPadding(FMargin(0))
			.SetCheckBoxPadding(FMargin(4.0f, 0.0f))
			.SetComboButtonStyle(ViewportComboButton)
			.SetButtonStyle(VMEMenuButton)
			.SetSeparatorBrush(FSlateNoResource())
			.SetSeparatorPadding(FMargin(2.0f, 0.0f))
			.SetExpandBrush(IMAGE_BRUSH("Icons/toolbar_expand_16x", Icon8x8));
		Set("VoxelMeshEditorToolBar", VMEToolbarStyle);

		FButtonStyle ViewportMenuWarningButton = FButtonStyle(Button)
			.SetNormalForeground(FStyleColors::AccentYellow)
			.SetHoveredForeground(FStyleColors::ForegroundHover)
			.SetPressedForeground(FStyleColors::ForegroundHover)
			.SetDisabledForeground(FStyleColors::AccentYellow);
		Set("VoxelMeshEditorToolBar.WarningButton", ViewportMenuWarningButton);

		FLinearColor ToolbarBackgroundColor = FStyleColors::Foldout.GetSpecifiedColor();
		ToolbarBackgroundColor.A = .75f;

		Set("VoxelMeshEditorToolBar.Background", new FSlateNoResource());
		Set("VoxelMeshEditorToolBar.OptionsDropdown", new IMAGE_BRUSH_SVG("Starship/EditorViewport/menu", Icon16x16));

		Set("VoxelMeshEditorToolBar.Font", FStyleFonts::Get().Normal);

		Set("VoxelMeshEditorToolBar.MenuButton", FButtonStyle(VMEMenuButton)
			.SetNormal(BOX_BRUSH("Common/SmallRoundedButton", FMargin(7.f / 16.f), FLinearColor(1, 1, 1, 0.75f)))
			.SetHovered(BOX_BRUSH("Common/SmallRoundedButton", FMargin(7.f / 16.f), FLinearColor(1, 1, 1, 1.0f)))
			.SetPressed(BOX_BRUSH("Common/SmallRoundedButton", FMargin(7.f / 16.f)))
		);


		Set("VoxelMeshEditorToolBar.MenuDropdown", new IMAGE_BRUSH("Common/ComboArrow", Icon8x8));
		Set("VoxelMeshEditorToolBar.Maximize.Normal", new IMAGE_BRUSH_SVG("Starship/EditorViewport/square", Icon16x16));
		Set("VoxelMeshEditorToolBar.Maximize.Checked", new IMAGE_BRUSH_SVG("Starship/EditorViewport/quad", Icon16x16));
		Set("VoxelMeshEditorToolBar.RestoreFromImmersive.Normal", new IMAGE_BRUSH("Icons/icon_RestoreFromImmersive_16px", Icon16x16));

		FLinearColor ViewportOverlayColor = FStyleColors::Input.GetSpecifiedColor();
		ViewportOverlayColor.A = 0.75f;

		Set("VoxelMeshEditorToolBar.OverlayBrush", new FSlateRoundedBoxBrush(ViewportOverlayColor, 8.0, FStyleColors::Dropdown, 1.0));

	}



    SetContentRoot( FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("NFGameUtilities"), TEXT("Resources"), TEXT("Slate")) );
    Set("ClassThumbnail.VoxelMeshData", new IMAGE_BRUSH(TEXT("Icons/DataIcon128"), Icon128x128));

}

void FVoxelMeshEditorStyle::Initialize()
{
    // Making sure to remove any old instance if there ever were to be one
    Shutdown();
    Instance = MakeShareable(new FVoxelMeshEditorStyle());
    FSlateStyleRegistry::RegisterSlateStyle(*Instance.Get());
}

void FVoxelMeshEditorStyle::Shutdown()
{
    if (!Instance.IsValid()) return;
    
    FSlateStyleRegistry::UnRegisterSlateStyle(*Instance.Get());
    Instance = nullptr;
}

const FVoxelMeshEditorStyle& FVoxelMeshEditorStyle::GetInstance()
{
    return *Instance.Get();
}

#undef LOCTEXT_NAMESPACE 
