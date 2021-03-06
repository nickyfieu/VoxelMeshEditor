// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "UnrealEd.h"
#include "VoxelMeshDataFactory.generated.h"

/**
 * 
 */
UCLASS()
class UVoxelMeshDataFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	UVoxelMeshDataFactory(const FObjectInitializer& ObjectInitializer);

	// UFactory Interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	// End of UFactory Interface
};
