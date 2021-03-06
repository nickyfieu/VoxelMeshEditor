// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../Data/InstancedStructArray.h"
#include "CustomStructBpFunctionLibrary.generated.h"

UCLASS()
class NFGAMEUTILITIES_API UCustomStructBpFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
    /**
     * Returns an array filled with "UCustomStructs" of the given "Type"
     * Returns an empty array when "Type" is none
     * Automatically casts to the given "Type", Only when this is a static value
     */
    UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType="Type", DynamicOutputParam="OutStructs"))
        static void GetAllStructsOfType(const FInstancedStructArray& InstancedStructArray, const TSubclassOf<UCustomStruct> Type, TArray<UCustomStruct*>& OutStructs);
    
    /**
     * Returns the first found "UCustomStruct" of the given "Type"
     * Returns nullptr when no "UCustomStruct" of the given "Type" was found
     * Automatically casts to the given "Type", Only when this is a static value
     */
    UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType="Type"))
        static UCustomStruct* GetStructOfType(const FInstancedStructArray& InstancedStructArray, const TSubclassOf<UCustomStruct> Type, bool& IsValid);
    
    /**
     * Returns if this struct holds any structs of teh given "Type"
     */
    UFUNCTION(BlueprintCallable)
        static bool HasStructOfType(const FInstancedStructArray& InstancedStructArray, const TSubclassOf<UCustomStruct> Type);

};
