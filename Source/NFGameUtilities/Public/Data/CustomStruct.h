// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CustomStruct.generated.h"

// Essentially a wrap around the whole inheriting from BlueprintStructs is impossible scenario....
UCLASS(Abstract, Blueprintable, meta=(Category = "Data", DontUseGenericSpawnObject))
class NFGAMEUTILITIES_API UCustomStruct : public UObject
{
	GENERATED_BODY()
};
