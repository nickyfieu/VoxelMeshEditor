// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "UnrealEd.h"

#include "AssetTypeActions_Base.h"

#include "CustomEventFactory.generated.h"

UCLASS(Blueprintable, hidecategories=(Object))
class UCustomEvent : public UObject//, public FTickableGameObject
{
    GENERATED_BODY()
public:
};

UCLASS(hidecategories=(Object))
class UCustomEventFactory : public UFactory
{
    GENERATED_UCLASS_BODY()

public:
    virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
class FCustomEventActions : public FAssetTypeActions_Base
{
public:
    FCustomEventActions(EAssetTypeCategories::Type Category);

    // IAssetTypeActions interface
    virtual uint32 GetCategories() override;
    virtual FText GetName() const override;
    virtual FColor GetTypeColor() const override;
    virtual UClass* GetSupportedClass() const override;
    // End of IAssetTypeActions interface

private:
    EAssetTypeCategories::Type CustomEventCategory;
};