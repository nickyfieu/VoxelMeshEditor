// Copyright © 2021 Nicky Fieu, All Rights Reserved.


#include "TabTool/TabToolPanel.h"
#include "TabTool/TabTool.h"

#include "../NFToolEditorModule.h"

#define LOCTEXT_NAMESPACE "TabTool"

const FName FTabToolTabIds::TabToolInnerTestPanelId = "TabToolPanel_1";

TabToolPanel::~TabToolPanel()
{

}

void TabToolPanel::OnStartupModule()
{
    // Ignoring inherited functionality
    // Since this would not allow for the correct spawning behavior to happen
    // Super::OnStartupModule();
    Initialize();
}

void TabToolPanel::OnShutdownModule()
{
    Super::OnShutdownModule();

    FGlobalTabmanager::Get()->UnregisterTabSpawner(TabName);
}

void TabToolPanel::Initialize()
{
    TabName = FTabToolTabIds::TabToolInnerTestPanelId;
    TabDisplayName = FText::FromString("Test panel");
    TabToolTip = FText::FromString("Tool tip of the panel inside of the tab tool window.");
}

void TabToolPanel::InvokeTab(FName SubTabToActivate)
{
    Super::InvokeTab(SubTabToActivate);
}

TSharedRef<SDockTab> TabToolPanel::SpawnTab(const FSpawnTabArgs& SpawnTabArgs)
{
    TSharedPtr<SDockTab> TabToolPanelTab;
    SAssignNew(TabToolPanelTab, SDockTab)
        .TabRole(ETabRole::PanelTab);

    TSharedPtr<SWidget> PanelContent;
    SAssignNew(PanelContent, STextBlock)
        .Text(FText::FromString("Test Text"));
     
    TabToolPanelTab.Get()->SetContent(PanelContent.ToSharedRef());

    return TabToolPanelTab.ToSharedRef();
}

#undef LOCTEXT_NAMESPACE 
