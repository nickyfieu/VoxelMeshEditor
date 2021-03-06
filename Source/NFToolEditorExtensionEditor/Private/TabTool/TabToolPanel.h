// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../NFToolTabBase.h"
#include "../NFToolsInterfaces.h"

class TabToolPanel : public FNFToolTabBase
{
public:
    virtual ~TabToolPanel();
    // FNFToolTabBase Interface
    virtual void OnStartupModule() override;
    virtual void OnShutdownModule() override;
    virtual void Initialize() override;
    virtual void InvokeTab(FName SubTabToActivate = NAME_None) override;
    virtual TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& SpawnTabArgs) override;
    // End of FNFToolTabBase Interface
};