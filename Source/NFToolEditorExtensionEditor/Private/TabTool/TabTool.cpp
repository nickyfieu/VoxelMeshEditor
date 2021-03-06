// Copyright © 2021 Nicky Fieu, All Rights Reserved.


#include "TabTool/TabTool.h"
#include "TabTool/STabTool.h"
#include "TabTool/TabToolStyle.h"
#include "TabTool/TabToolPanel.h"
#include "../NFToolEditorModule.h"
#include "Widgets/Docking/SDockTab.h"
#include "Interfaces/IMainFrameModule.h"

#define LOCTEXT_NAMESPACE "TabTool"

const FName FTabToolTabIds::TabToolId = "TabToolWindow";

const FName MainFrame = FName(TEXT("MainFrame"));

void TabTool::AddModuleListeners()
{
    InternalPanelPtrs.Add(MakeShareable(new TabToolPanel()));
    ModuleListeners.Add(InternalPanelPtrs[0].ToSharedRef());
}

TabTool::~TabTool()
{

}

void TabTool::OnStartupModule()
{
    Super::OnStartupModule();
    INFToolsModuleInterface::StartupModule();

    FNFToolEditorModule::Get().AddMenuExtension(
        FName("NFAssetTools"),
        FMenuExtensionDelegate::CreateRaw(
            this,
            &Super::MakeMenuEntry
        ),
        EExtensionHook::After
    );
}

void TabTool::OnShutdownModule()
{
    Super::OnShutdownModule();
    INFToolsModuleInterface::ShutdownModule();

    FTabToolStyle::Shutdown();
}

void TabTool::Initialize()
{
    TabName = FTabToolTabIds::TabToolId;
    TabDisplayName = FText::FromString("Tab Tool Window");
    TabToolTip = FText::FromString("Testing Tab Tool Tip");

    FTabToolStyle::Initialize();
}

void TabTool::InvokeTab(FName SubTabToActivate)
{
    if (WeakTabTool.IsValid())
    {
        if (SubTabToActivate != NAME_None)
        {
            WeakTabTool.Pin()->GetTabManager()->TryInvokeTab(SubTabToActivate);
        }
    }
}

TSharedRef<SDockTab> TabTool::SpawnTab(const FSpawnTabArgs& SpawnTabArgs)
{
    TSharedPtr<SDockTab> TabToolTab;
    SAssignNew(TabToolTab, SDockTab)
        .TabRole(ETabRole::MajorTab);

    TSharedPtr<STabTool> TabTool;
    SAssignNew(TabTool, STabTool, TabToolTab.ToSharedRef())
        .Tool(SharedThis(this))
        .Name(TabName)
        .DisplayName(TabDisplayName)
        .ToolTip(TabToolTip);

    WeakTabTool = TabTool;

    TabToolTab->SetContent(TabTool.ToSharedRef());

    return TabToolTab.ToSharedRef();
}

#undef LOCTEXT_NAMESPACE 
