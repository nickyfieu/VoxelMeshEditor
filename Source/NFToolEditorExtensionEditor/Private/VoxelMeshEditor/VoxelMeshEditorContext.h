// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <UObject/Object.h>
#include "VoxelMeshEditorContext.generated.h"

class SVMEEditorViewportToolBar;

/**
 * Holds all the voxel mesh editor context classes 
 */
UCLASS()
class NFTOOLEDITOREXTENSIONEDITOR_API UVMEEditorViewportToolBarContext : public UObject
{
    GENERATED_BODY()
public:
    TWeakPtr<SVMEEditorViewportToolBar> ToolBarWidget;
};