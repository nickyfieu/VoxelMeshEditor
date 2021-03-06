// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TabTool.h"
#include <Widgets/Docking/SDockTab.h>
#include <Framework/Application/SlateApplication.h>


class FTabManager;

/** 
 * The window panel that gets spawned when opening the tab bool
 */
class STabTool : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(STabTool)
    {}
    SLATE_ARGUMENT(TSharedPtr<TabTool>, Tool)
    SLATE_ARGUMENT(FName, Name)
    SLATE_ARGUMENT(FText, DisplayName)
    SLATE_ARGUMENT(FText, ToolTip)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, const TSharedRef<SDockTab>& OwnerTab);
    void Initialize(const TSharedRef<SDockTab>& OwnerTab);

    TSharedPtr<FTabManager> GetTabManager() const { return TabManager; }

    /*
    TSharedRef<SWidget> RestoreContentArea(const TSharedRef<SDockTab>& OwnerTab, const TSharedRef<SWindow>& OwnerWindow);
    */

protected:
    static void FillTestMenuBar(FMenuBuilder& MenuBuilder, const TSharedPtr<FTabManager> TabManager);

private:

    /** Binds the commands to actions for the tab tool pannel */
    void BindCommands();
    
    /** Registers the menus asociated with the tab tool pannel */
    void RegisterMenus();

    /** Callback for executing the 'TestCommand' action. */
    void HandleTestCommandActionExecute();

    /** Callback for executing the 'TestCommand' action can execute. */
    bool HandleTestCommandActionCanExecute();

protected:
    TSharedPtr<TabTool> Tool;

    FName Name;
    FText DisplayName;
    FText ToolTip;

    FName LayoutName;

private:
    /** Holds the command list that manages the tab tool commands. */
    TSharedPtr<FUICommandList> TabToolCommandList;

    /** Holds the tab manager that manages the tab tool tabs. */
    TSharedPtr<FTabManager> TabManager;

};
