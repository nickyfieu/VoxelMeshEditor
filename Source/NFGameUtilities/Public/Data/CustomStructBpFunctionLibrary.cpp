// Copyright © 2021 Nicky Fieu, All Rights Reserved.


#include "../Data/CustomStructBpFunctionLibrary.h"
#include "../Data/CustomStruct.h"


void UCustomStructBpFunctionLibrary::GetAllStructsOfType(const FInstancedStructArray& InstancedStructArray, const TSubclassOf<UCustomStruct> Type, TArray<UCustomStruct*>& OutStructs)
{
    if (!Type)
    {
        UE_LOG(LogTemp, Warning, TEXT("[InstancedStructArray] GetAllStructsOfType - Invalid Type!"));
        return;
    }

    for (UCustomStruct* InstancedStruct : InstancedStructArray.InstancedStructs)
    {
        if (InstancedStruct->IsValidLowLevelFast() && InstancedStruct->IsA(Type))
        {
            OutStructs.Add(InstancedStruct);
        }
    }
}

UCustomStruct* UCustomStructBpFunctionLibrary::GetStructOfType(const FInstancedStructArray& InstancedStructArray, const TSubclassOf<UCustomStruct> Type, bool& IsValid)
{
    IsValid = false;

    if (!Type)
    {
        UE_LOG(LogTemp, Warning, TEXT("[InstancedStructArray] GetStructOfType - Invalid Type!"));
        return nullptr;
    }

    for (UCustomStruct* InstancedStruct : InstancedStructArray.InstancedStructs)
    {
        if (InstancedStruct->IsValidLowLevelFast() && InstancedStruct->IsA(Type))
        {
            IsValid = true;
            return InstancedStruct;
        }
    }

    return nullptr;
}

bool UCustomStructBpFunctionLibrary::HasStructOfType(const FInstancedStructArray& InstancedStructArray, const TSubclassOf<UCustomStruct> Type)
{
    if (!Type)
    {
        UE_LOG(LogTemp, Warning, TEXT("[InstancedStructArray] HasStructOfType - Invalid Type!"));
        return false;
    }

    for (UCustomStruct* InstancedStruct : InstancedStructArray.InstancedStructs)
    {
        if (InstancedStruct->IsValidLowLevelFast() && InstancedStruct->IsA(Type))
        {
            return true;
        }
    }

    return false;
}
