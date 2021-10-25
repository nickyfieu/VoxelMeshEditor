// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/* Resources used:
 * https://lxjk.github.io/2019/10/01/How-to-Make-Tools-in-U-E.html
 * https://www.youtube.com/watch?v=zg_VstBxDi8&list=PLu1LAfUwLp4Jx5DkSBwPyPCc6jiHfI0dA&index=2&t=310s
 */

class INFToolsModuleListenerInterface
{
public:
    virtual void OnStartupModule() {};
    virtual void OnShutdownModule() {};
};

class INFToolsModuleInterface : public IModuleInterface
{
public:
    // IModuleInterface Interface
    void StartupModule() override
    {
        if (!IsRunningCommandlet())
        {
            AddModuleListeners();
            for (int32 i = 0; i < ModuleListeners.Num(); ++i)
            {
                ModuleListeners[i]->OnStartupModule();
            }
        }
    }

    void ShutdownModule() override
    {
        for (int32 i = 0; i < ModuleListeners.Num(); ++i)
        {
            ModuleListeners[i]->OnShutdownModule();
        }
        ModuleListeners.Empty();
    }
    // End of IModuleInterface Interface

    virtual void AddModuleListeners() 
    {
        // Overwritable function
    }

public:
    TArray<TSharedRef<INFToolsModuleListenerInterface>> ModuleListeners;
};