// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#include "NFToolEditorModule.h"
#include "NFToolEditorPrivatePCH.h"
#include "LevelEditor.h"
#include "MenuTool/MenuTool.h"
#include "TabTool/TabTool.h"
#include "TabTool/TabToolPanel.h"
#include "VoxelMeshEditor/VoxelMeshEditor.h"

DEFINE_LOG_CATEGORY(LogNFToolEditor);

#define LOCTEXT_NAMESPACE "FNFToolEditor"

/**
 * Static member definitions.
 */
const FName FModuleStyleIds::NFToolEditorModuleId = TEXT("NFToolEditor");

/**
 * Initialization of the module.
 */
void FNFToolEditorModule::StartupModule()
{
    if (!IsRunningCommandlet())
    {        
        InitializeModuleSlateStyles();
        InitializeModuleData();
        InitializeLevelEditorMenuBarExtensions();
    }
    INFToolsModuleInterface::StartupModule();
}

/**
 * Cleanup of the module.
 */
void FNFToolEditorModule::ShutdownModule()
{
    INFToolsModuleInterface::ShutdownModule();

    TabToolExtensibilityManager.Reset();

    UnRegisterModuleSlateStyles();
}

/**
 * Add listeners to other modules so they can be initalized after the main module and automatically cleaned up before the main module is destroyed.
 */
void FNFToolEditorModule::AddModuleListeners()
{
    // order matters for some !
    ///ModuleListeners.Add(MakeShareable(new MenuTool));
    ///ModuleListeners.Add(MakeShareable(new TabTool));
    ModuleListeners.Add(MakeShareable(new VoxelMeshEditor));
}

/**
 * Extends a menu at the specified extension point
 *
 * @ExtensionHook       Where to apply hooks in relation to the extension hook.
 * @ExtensionDelegate   Called to populate the part of the menu you're extending.
 * @Position            Part of the menu to extend. You can extend the same point multiple times, and extensions will be applied in the order they were registered.
 * @CommandList         The UI command list responsible for handling actions for the menu items you'll be extending the menu with.
 */
void FNFToolEditorModule::AddMenuExtension(FName ExtensionHook, const FMenuExtensionDelegate& ExtensionDelegate, EExtensionHook::Position Position, const TSharedPtr<FUICommandList>& CommandList)
{
    if (!MenuExtender.IsValid())
    {
        UE_LOG(LogNFToolEditor, Warning, TEXT("MenuExtender invalid in class FNFToolEditorModule at line %i!"), int32(__LINE__));
        return;
    }
    MenuExtender->AddMenuExtension(ExtensionHook, Position, CommandList, ExtensionDelegate);
}

/**
 * Extends a menu bar at the specified extension point
 * 
 * @ExtensionHook       Where to apply hooks in relation to the extension hook.
 * @ExtensionDelegate   Called to populate the part of the menu you're extending.
 * @Position            Part of the menu to extend. You can extend the same point multiple times, and extensions will be applied in the order they were registered.
 * @CommandList         The UI command list responsible for handling actions for the menu items you'll be extending the menu with.
 */
void FNFToolEditorModule::AddMenuBarExtension(FName ExtensionHook, const FMenuBarExtensionDelegate& ExtensionDelegate, EExtensionHook::Position Position, const TSharedPtr<FUICommandList>& CommandList)
{
    if (!MenuExtender.IsValid())
    {
        UE_LOG(LogNFToolEditor, Warning, TEXT("MenuExtender invalid in class FNFToolEditorModule at line %i!"), int32(__LINE__));
        return;
    }
    MenuExtender->AddMenuBarExtension(ExtensionHook, Position, CommandList, ExtensionDelegate);
}

/**
 * Initializes all module slate styles.
 * Add slate image brushes, fonts, sounds and other style definitions here.
 */
void FNFToolEditorModule::InitializeModuleSlateStyles()
{
    {   /** NFToolEditorModule style definiation */
        NFToolEditorModuleStyleSet = MakeShareable(new FSlateStyleSet(FModuleStyleIds::NFToolEditorModuleId));
        {   /** TabTool style definitions */
            NFToolEditorModuleStyleSet->Set("NFToolEditor.TabTool.TestSaveImage", new FSlateImageBrush(FPaths::ProjectPluginsDir() / "NFGameUtilities/Resources/Slate/Images/TestSaveImage.png", FVector2D(512.f / 8.f, 512.f / 8.f)));
        }
        FSlateStyleRegistry::RegisterSlateStyle(*NFToolEditorModuleStyleSet.Get());
    }
}

/**
 * Initializes most FNFToolEditorModule variables.
 */
void FNFToolEditorModule::InitializeModuleData()
{
    FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
    LevelEditorExtensibilityManager = LevelEditorModule.GetMenuExtensibilityManager();
    MenuExtender = MakeShareable(new FExtender);
    LevelEditorExtensibilityManager->AddExtender(MenuExtender);

    TabToolExtensibilityManager = MakeShared<FExtensibilityManager>();

    NFToolsMenuRoot = FWorkspaceItem::NewGroup(LOCTEXT("NFToolsMenu_Root", "NFTools Menu Root"));

    NFTabToolCategory = NFToolsMenuRoot->AddGroup(LOCTEXT("NFToolsMenu_TabToolCategory", "TabTools"), FSlateIcon(), true);
}

/** 
 * Defines menu bar extensions to level editor specific hooks.
 */
void FNFToolEditorModule::InitializeLevelEditorMenuBarExtensions()
{
    AddMenuBarExtension(
        "Tools",
        FMenuBarExtensionDelegate::CreateRaw(this, &FNFToolEditorModule::AddNFToolPullDownMenu),
        EExtensionHook::After,
        NULL
    );
}

/**
 * Adds the pull down menu to the NFTool extension using the previously initialized @NFToolMenuPullDownData
 */
void FNFToolEditorModule::AddNFToolPullDownMenu(FMenuBarBuilder& MenuBuilder)
{
    MenuBuilder.AddPullDownMenu(
        FText::FromString("NFTools"),
        FText::FromString("Tools created by Nicky Fieu."),
        FNewMenuDelegate::CreateRaw(this, &FNFToolEditorModule::AddNFPullDownMenuSectionData)
    );
}

/**
 * Adds the menu sections to the pull NFTool pull down menu using @NFToolMenuSectionData
 */
void FNFToolEditorModule::AddNFPullDownMenuSectionData(FMenuBuilder& MenuBuilder)
{
    MenuBuilder.BeginSection(TEXT("NFAssetTools"), FText::FromString("AssetTools"));
    MenuBuilder.AddWidget(SNew(SBox), FText::FromString(""), false, false); // hack to display section in the pulldown menu
    MenuBuilder.EndSection();
    MenuBuilder.BeginSection(TEXT("NFDebugTools"), FText::FromString("DebugTools"));
    MenuBuilder.AddWidget(SNew(SBox), FText::FromString(""), false, false); // hack to display section in the pulldown menu
    MenuBuilder.EndSection();
}

/**
 * UnRegisters all slate styles with their Ids defined in FModuleStyleIds.
 */
void FNFToolEditorModule::UnRegisterModuleSlateStyles()
{
    FSlateStyleRegistry::UnRegisterSlateStyle(FModuleStyleIds::NFToolEditorModuleId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNFToolEditorModule, NFToolEditorExtensionEditor)