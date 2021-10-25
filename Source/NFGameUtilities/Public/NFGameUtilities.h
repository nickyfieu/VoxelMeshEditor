// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Styling/SlateStyle.h"

class IAssetTypeActions;

class FNFGameUtilitiesModule : public IModuleInterface
{
public:

    TSharedPtr<FSlateStyleSet> CustomSlateStyle;
    TArray<TSharedPtr<IAssetTypeActions>> CustomAssetTypeActions;

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
