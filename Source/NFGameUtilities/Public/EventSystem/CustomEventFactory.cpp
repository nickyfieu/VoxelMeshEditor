// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#include "CustomEventFactory.h"
#include "NFGameUtilities.h"

UCustomEventFactory::UCustomEventFactory(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    SupportedClass = UCustomEvent::StaticClass();
    bCreateNew = true;
    bEditAfterNew = true;
}

UObject* UCustomEventFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    UCustomEvent* NewCustomEvent = NewObject<UCustomEvent>(InParent, Class, Name, Flags | RF_Transactional);
    return NewCustomEvent;
}

FCustomEventActions::FCustomEventActions(EAssetTypeCategories::Type Category)
    : CustomEventCategory(Category)
{
}

uint32 FCustomEventActions::GetCategories()
{
    return CustomEventCategory;
}

FText FCustomEventActions::GetName() const
{
    return FText::FromString("Event");
}

FColor FCustomEventActions::GetTypeColor() const
{
    return FColor(65, 105, 225);
}

UClass* FCustomEventActions::GetSupportedClass() const
{
    return UCustomEvent::StaticClass();
}