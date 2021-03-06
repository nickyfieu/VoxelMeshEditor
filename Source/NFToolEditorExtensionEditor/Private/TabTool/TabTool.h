// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../NFToolTabBase.h"

struct FTabToolTabIds
{
    static const FName TabToolId;
    static const FName TabToolInnerTestPanelId;
};

class STabTool;
class TabToolPanel;

class TabTool : public FNFToolTabBase, public INFToolsModuleInterface
{
public:
    virtual ~TabTool();
    // FNFToolTabBase Interface
    virtual void OnStartupModule() override;
    virtual void OnShutdownModule() override;
    virtual void Initialize() override;
    virtual void InvokeTab(FName SubTabToActivate = NAME_None) override;
    virtual TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& SpawnTabArgs) override;
    // End of FNFToolTabBase Interface

    // INFToolsModuleInterface Interface
    virtual void AddModuleListeners() override;
    // End of INFToolsModuleInterface Interface
    
public:
    TSharedPtr<TabToolPanel> TabToolPanelPtr;

    TArray<TSharedPtr<FNFToolTabBase>> InternalPanelPtrs;


private:
    TWeakPtr<STabTool> WeakTabTool;
};
