// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <SEditorViewport.h>
#include <Framework/Application/SlateApplication.h>

// VME => voxel mesh editor
class NFTOOLEDITOREXTENSIONEDITOR_API SVMEEditorPropertiesTab : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SVMEEditorPropertiesTab)
    {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    ~SVMEEditorPropertiesTab();

    class AVoxelMeshEditorActor* EditorRef;
    class AVoxelMeshEditorGridActor* GridRef;

protected:
    TOptional<int32> GetGridSizeX() const;
    TOptional<int32> GetGridSizeY() const;
    TOptional<int32> GetGridSizeZ() const;

    TOptional<float> GetVoxelSize() const;
    TOptional<float> GetGridLineWidth() const;

    EVisibility GetCanEditGrid() const;

    void SetGridSizeX(int32 NewValue);
    void SetGridSizeY(int32 NewValue);
    void SetGridSizeZ(int32 NewValue);

    void OnCommitGridSizeX(int32 NewValue, ETextCommit::Type CommitType);
    void OnCommitGridSizeY(int32 NewValue, ETextCommit::Type CommitType);
    void OnCommitGridSizeZ(int32 NewValue, ETextCommit::Type CommitType);

    void SetVoxelSize(float NewValue);
    void SetGridLineWidth(float NewValue);

    void OnCommitVoxelSize(float NewValue, ETextCommit::Type CommitType);

    void SetCanEditGridSizeState(ECheckBoxState NewState);
    
    void SetBrushColor(FLinearColor NewColor);

private:
    ECheckBoxState CanEditGrid;
};
