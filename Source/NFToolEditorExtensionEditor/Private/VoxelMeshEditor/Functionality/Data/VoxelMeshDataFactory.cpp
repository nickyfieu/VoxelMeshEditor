// Copyright © 2021 Nicky Fieu, All Rights Reserved.


#include "VoxelMeshDataFactory.h"
#include "VoxelMeshData.h"
#include "Editor.h"

UVoxelMeshDataFactory::UVoxelMeshDataFactory(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    SupportedClass = UVoxelMeshData::StaticClass();
    bCreateNew = false; // turned off for import
    bEditAfterNew = false; // turned off for import
}

UObject* UVoxelMeshDataFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    return NewObject<UVoxelMeshData>(InParent, Class, Name, Flags | RF_Transactional);
}