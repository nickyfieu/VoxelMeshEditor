// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Data/CustomStruct.h"
#include "../Data/InstancedStructArray.h"
#include "../EventSystem/CustomEventFactory.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ModifierEvent.generated.h"

UCLASS(Within = ModifierEvent)
class NFGAMEUTILITIES_API UModifierData : public UCustomStruct
{
    GENERATED_BODY()
};

UCLASS()
class NFGAMEUTILITIES_API UModifierEvent : public UCustomEvent
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
        FInstancedStructArray ModifierData{};
};

UCLASS(ClassGroup=(CustomEventSystem), meta=(BlueprintSpawnableComponent))
class NFGAMEUTILITIES_API UCustomEventComponent : public UActorComponent
{
    GENERATED_BODY()

public:

};