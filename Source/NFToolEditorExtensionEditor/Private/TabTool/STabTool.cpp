// Copyright © 2021 Nicky Fieu, All Rights Reserved.


#include "TabTool/STabTool.h"
#include "TabTool/STabToolToolBar.h"
#include "TabTool/TabToolCommands.h"
#include "TabTool/TabToolPanel.h"
#include "../NFToolEditorModule.h"
#include <Widgets/Layout/SScrollBox.h>
#include <SEditorViewport.h>
#include <Framework/Commands/UIAction.h>
#include <Framework/Commands/UICommandList.h>
#include <Framework/Application/SlateApplication.h>
#include "StatusBarSubsystem.h"

#define LOCTEXT_NAMESPACE "TabTool"

void STabTool::Construct(const FArguments& InArgs, const TSharedRef<SDockTab>& OwnerTab)
{
    Tool = InArgs._Tool;
    Name = InArgs._Name;
    DisplayName = InArgs._DisplayName;
    ToolTip = InArgs._ToolTip;
    LayoutName = TEXT("TabTool_Layout_v1.0");

    TabToolCommandList = MakeShareable(new FUICommandList);
    BindCommands();
    RegisterMenus();
    Initialize(OwnerTab);
}

void STabTool::Initialize(const TSharedRef<SDockTab>& OwnerTab)
{
    FModuleManager::Get().LoadModuleChecked("StatusBar");
    
    TabManager = FGlobalTabmanager::Get()->NewTabManager(OwnerTab);
    TSharedRef<FWorkspaceItem> TestMenuBarGroup = TabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("TestMenuBarGroupName", "TestMenuBar"));

    TabManager->SetAllowWindowMenuBar(true);

    /** 
     * Registering the tab spawners of tabs inside of this tab.
     * Need to be done here as this require the specific LocalWorkspaceMenuGroup
     * For a specific widget defined below
     */
    if (TabTool* TabTool = Tool.Get())
    {
        for (int32 i = 0; i < TabTool->InternalPanelPtrs.Num(); ++i)
        {
            FNFToolTabBase* ToolTab = TabTool->InternalPanelPtrs[i].Get();

            FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
                ToolTab->GetTabName(),
                FOnSpawnTab::CreateRaw(ToolTab, &FNFToolTabBase::SpawnTab)
            )   .SetDisplayName(ToolTab->GetTabDisplayName())
                .SetTooltipText(ToolTab->GetTabToolTip())
                .SetGroup(TestMenuBarGroup)
                .SetMenuType(ETabSpawnerMenuType::Enabled);
        }
    }

    /** create tab layout */
    const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout(LayoutName)
        ->AddArea
        (
            FTabManager::NewPrimaryArea()
            ->SetOrientation(Orient_Horizontal)
            ->Split
            (
                FTabManager::NewStack()
                ->AddTab(FTabToolTabIds::TabToolInnerTestPanelId, ETabState::OpenedTab)
                ->SetHideTabWell(true)
                ->SetSizeCoefficient(1.0f)
            )
        );

    /** Create and initialize a menu bar pull down menu that allows for restoration of closed windows. */
    FMenuBarBuilder WindowMenuBarBuilder = FMenuBarBuilder(TSharedPtr<FUICommandList>());

    WindowMenuBarBuilder.AddPullDownMenu(
        LOCTEXT("Test Menu Bar Label", "TestMenuBar"),
        FText::GetEmpty(),
        FNewMenuDelegate::CreateStatic(&STabTool::FillTestMenuBar, TabManager),
        "TestMenuBarHook"
    );

    TSharedRef<SWidget> WindowMenuWidget = WindowMenuBarBuilder.MakeWidget();

    /** Widgets + Layout come together here for the final product of the tabtools */
    ChildSlot
    [
        SNew(SVerticalBox) +
        SVerticalBox::Slot()
        .Padding(FMargin(0.f, 0.f, 0.f, 2.f))
        .AutoHeight()
        [
            SNew(STabToolToolBar, TabToolCommandList.ToSharedRef())
        ] +
        SVerticalBox::Slot()
        .Padding(FMargin(4.f, 2.f, 4.f, 2.f))
        .FillHeight(1.0f)
        [
            TabManager->RestoreFrom(Layout, TSharedPtr<SWindow>()).ToSharedRef()
        ] +
        SVerticalBox::Slot()
        .Padding(FMargin(0.f, 2.f, 0.f, 0.f))
        .AutoHeight()
        [
            GEditor->GetEditorSubsystem<UStatusBarSubsystem>()->MakeStatusBarWidget(TEXT("TabTool.StatusBar"), OwnerTab)
        ]
    ];

    // Tell the tab manager about the menu bar widget since this is not something that can be added trough the childslot.
    TabManager->SetMenuMultiBox(WindowMenuBarBuilder.GetMultiBox(), WindowMenuWidget);
}

void STabTool::FillTestMenuBar(FMenuBuilder& MenuBuilder, const TSharedPtr<FTabManager> TabManager)
{
    if (!TabManager.IsValid())
    {
        return;
    }

#if !WITH_EDITOR
    FGlobalTabmanager::Get()->PopulateTabSpawnerMenu(MenuBuilder, FNFToolEditorModule::Get().GetNFTabToolCategory());
#endif //!WITH_EDITOR

    TabManager->PopulateLocalTabSpawnerMenu(MenuBuilder);
}

void STabTool::BindCommands()
{
    FTabToolCommands::Register();

    const FTabToolCommands& Commands = FTabToolCommands::Get();

    TabToolCommandList->MapAction(
        Commands.TestCommand,
        FExecuteAction::CreateSP(this, &STabTool::HandleTestCommandActionExecute),
        FCanExecuteAction::CreateSP(this, &STabTool::HandleTestCommandActionCanExecute)
    );
}

void STabTool::RegisterMenus()
{
}

void STabTool::HandleTestCommandActionExecute()
{
    UE_LOG(LogNFToolEditor, Warning, TEXT("TestCommand fired!"));
}

bool STabTool::HandleTestCommandActionCanExecute()
{
    return true;
}

#undef LOCTEXT_NAMESPACE