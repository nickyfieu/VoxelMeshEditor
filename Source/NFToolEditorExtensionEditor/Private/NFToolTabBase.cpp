// Copyright © 2021 Nicky Fieu, All Rights Reserved.


#include "NFToolTabBase.h"
#include "NFToolEditorModule.h"

#define LOCTEXT_NAMESPACE "FNFToolEditor"

FNFToolTabBase::~FNFToolTabBase()
{

}

void FNFToolTabBase::OnStartupModule()
{
    Initialize();

    FTabSpawnerEntry& TabSpawner = FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
        TabName,
        FOnSpawnTab::CreateRaw(this, &FNFToolTabBase::SpawnTab)
    )   .SetDisplayName(TabDisplayName)
        .SetTooltipText(TabToolTip);
       
    TabSpawner.SetGroup(FNFToolEditorModule::Get().GetNFTabToolCategory());
}

void FNFToolTabBase::OnShutdownModule()
{
    FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TabName);
}

void FNFToolTabBase::Initialize()
{
    // Overwritable function
}

void FNFToolTabBase::MakeMenuEntry(FMenuBuilder& MenuBuilder)
{
    FGlobalTabmanager::Get()->PopulateTabSpawnerMenu(MenuBuilder, TabName);
}

void FNFToolTabBase::InvokeTab(FName SubTabToActivate)
{
    FGlobalTabmanager::Get()->TryInvokeTab(SubTabToActivate);
}

TSharedRef<SDockTab> FNFToolTabBase::SpawnTab(const FSpawnTabArgs& SpawnTabArgs)
{
    return SNew(SDockTab);
}

#undef LOCTEXT_NAMESPACE