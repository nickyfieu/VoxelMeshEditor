// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InstancedStructArray.generated.h"

USTRUCT(BlueprintType, meta=(ShowOnlyInnerProperties))
struct FInstancedStructArray
{
	GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, NoClear, meta=(Category="Data"))
        TArray<class UCustomStruct*> InstancedStructs;
};
