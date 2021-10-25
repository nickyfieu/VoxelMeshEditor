// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "NFToolsInterfaces.h"
#include <Framework/Application/SlateApplication.h>

/** 
 * Resources used:
 * https://lxjk.github.io/2019/10/01/How-to-Make-Tools-in-U-E.html
 * https://www.youtube.com/watch?v=zg_VstBxDi8&list=PLu1LAfUwLp4Jx5DkSBwPyPCc6jiHfI0dA&index=2&t=310s
 */

DECLARE_LOG_CATEGORY_EXTERN(LogNFToolEditor, Log, All);

struct FModuleStyleIds
{
    static const FName NFToolEditorModuleId;
};

class FNFToolEditorModule : public INFToolsModuleInterface
{
public:
    // INFToolsModuleInterface Interface
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    virtual void AddModuleListeners() override;
    // End of INFToolsModuleInterface Interface

    void AddMenuExtension(FName ExtensionHook, const FMenuExtensionDelegate& ExtensionDelegate, EExtensionHook::Position Position = EExtensionHook::Before, const TSharedPtr<FUICommandList>& CommandList = NULL);
    void AddMenuBarExtension(FName ExtensionHook, const FMenuBarExtensionDelegate& ExtensionDelegate, EExtensionHook::Position Position = EExtensionHook::Before, const TSharedPtr<FUICommandList>& CommandList = NULL);

    static inline FNFToolEditorModule& Get()
    {
        return FModuleManager::LoadModuleChecked<FNFToolEditorModule>("NFToolEditorExtensionEditor");
    }

    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("NFToolEditorExtensionEditor");
    }

    inline TSharedRef<FWorkspaceItem> GetNFToolsMenuRoot() const
    {
        return NFToolsMenuRoot.ToSharedRef();
    }

    inline TSharedRef<FWorkspaceItem> GetNFTabToolCategory() const
    {
        return NFTabToolCategory.ToSharedRef();
    }

    inline TSharedPtr<FExtensibilityManager> GetTabToolExtensibilityManager() const
    {
        return TabToolExtensibilityManager;
    }

    inline TSharedPtr<FSlateStyleSet> GetNFToolEditorModuleStyleSet() const
    {
        return NFToolEditorModuleStyleSet;
    }

protected:
    void InitializeModuleSlateStyles();
    void InitializeModuleData();
    void InitializeLevelEditorMenuBarExtensions();
    void UnRegisterModuleSlateStyles();


    void AddNFToolPullDownMenu(FMenuBarBuilder& MenuBuilder);
    void AddNFPullDownMenuSectionData(FMenuBuilder& MenuBuilder);

protected:
    TSharedPtr<FExtensibilityManager> LevelEditorExtensibilityManager;
    TSharedPtr<FExtender> MenuExtender;

    TSharedPtr<FWorkspaceItem> NFToolsMenuRoot;

    TSharedPtr<FWorkspaceItem> NFTabToolCategory;

    TSharedPtr<FExtensibilityManager> TabToolExtensibilityManager;

    TSharedPtr<FSlateStyleSet> NFToolEditorModuleStyleSet;
};

