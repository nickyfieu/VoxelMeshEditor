// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

class FVoxelMeshEditorStyle : public FSlateStyleSet
{
public:

    static void Initialize();
    static void Shutdown();

    static const FVoxelMeshEditorStyle& GetInstance();

private:

    FVoxelMeshEditorStyle();

private:
    static FName StyleName;
    static TSharedPtr<FVoxelMeshEditorStyle> Instance;

    FButtonStyle Button;

    const FVector2D Icon8x8 = FVector2D(8.0f, 8.0f);
    const FVector2D Icon16x16 = FVector2D(16.0f, 16.0f);
    const FVector2D Icon32x32 = (32.0f, 32.0f);
    const FVector2D Icon64x64 = (64.0f, 64.0f);
    const FVector2D Icon128x128 = (128.0f, 128.0f);
    const FVector2D Icon256x256 = (246.0f, 246.0f);

    const FVector2D Icon20x20 = FVector2D(20.0f, 20.0f);
    const FVector2D Icon30x30 = FVector2D(30.0f, 30.0f);
    const FVector2D Icon40x40 = FVector2D(40.0f, 40.0f);
    const FVector2D Icon50x50 = FVector2D(50.0f, 50.0f);
    const FVector2D Icon60x60 = FVector2D(60.0f, 60.0f);
    const FVector2D Icon70x70 = FVector2D(70.0f, 70.0f);
    const FVector2D Icon80x80 = FVector2D(80.0f, 80.0f);
    const FVector2D Icon90x90 = FVector2D(80.0f, 80.0f);
    const FVector2D Icon100x100 = FVector2D(100.0f, 100.0f);

    const FVector2D Icon40x20 = FVector2D(40.0f, 20.0f);
    const FVector2D Icon60x30 = FVector2D(60.0f, 30.0f);
    const FVector2D Icon80x40 = FVector2D(80.0f, 40.0f);
    const FVector2D Icon100x50 = FVector2D(100.0f, 50.0f);

    const FVector2D Icon20x40 = FVector2D(20.0f, 40.0f);
    const FVector2D Icon30x60 = FVector2D(30.0f, 60.0f);
    const FVector2D Icon40x80 = FVector2D(40.0f, 80.0f);
    const FVector2D Icon50x100 = FVector2D(50.0f, 100.0f);
};