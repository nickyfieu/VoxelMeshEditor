// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NFToolsInterfaces.h"

class FNFToolTabBase : public INFToolsModuleListenerInterface, public TSharedFromThis<FNFToolTabBase>
{
public:
    virtual ~FNFToolTabBase();

    // INFToolsModuleListenerInterface interface
    virtual void OnStartupModule() override;
    virtual void OnShutdownModule() override;
    // End of INFToolsModuleListenerInterface interface

    /**
     * Initialize should be used for data initialization
     * Will always be called before anything in OnStartupModule
     * Need to call Super::OnStartupModule() in child::OnStartupModule !!!
     */
    virtual void Initialize();
    virtual void MakeMenuEntry(FMenuBuilder& MenuBuilder);
    virtual TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& SpawnTabArgs);
    virtual void InvokeTab(FName SubTabToActivate = NAME_None);

    FName GetTabName() const
    {
        return TabName;
    }

    FText GetTabDisplayName() const
    {
        return TabDisplayName;
    }

    FText GetTabToolTip() const 
    {
        return TabToolTip;
    }

public:
    typedef FNFToolTabBase Super;

protected:
    FName TabName;
    FText TabDisplayName;
    FText TabToolTip;
};
