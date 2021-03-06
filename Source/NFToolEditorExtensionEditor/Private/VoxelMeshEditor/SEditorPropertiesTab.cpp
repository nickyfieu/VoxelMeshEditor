// Copyright © 2021 Nicky Fieu, All Rights Reserved.


#include "SEditorPropertiesTab.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Functionality/VoxelMeshEditorActor.h"
#include "Functionality/VoxelMeshEditorGridActor.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Colors/SColorPicker.h"

#define LOCTEXT_NAMESPACE "VoxelMeshEditor"

void SVMEEditorPropertiesTab::Construct(const FArguments& InArgs)
{
    ChildSlot
    [
        SNew(SBox)
        [
            SNew(SVerticalBox) +
            SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SBorder)
                .Padding(FMargin(8.f,2.f,8.f,2.f))
                [
                    SNew(SVerticalBox) +
                    SVerticalBox::Slot()
                    .VAlign(VAlign_Center)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString("Grid Size"))
                        .Justification(ETextJustify::Center)
                    ]
                ]
            ]+
            SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SHorizontalBox) +
                SHorizontalBox::Slot()
                .Padding(FMargin(4.f, 2.f, 4.f, 2.f))
                .HAlign(HAlign_Left)
                .AutoWidth()
                .FillWidth(0.35f)
                [
                    SNew(SVerticalBox) +
                    SVerticalBox::Slot()
                    .VAlign(VAlign_Center)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString("Edit Grid Size"))
                        .Justification(ETextJustify::Left)
                    ]
                ] +
                SHorizontalBox::Slot()
                .Padding(FMargin(4.f, 2.f, 4.f, 2.f))
                .HAlign(HAlign_Left)
                [
                    SNew(SCheckBox)
                    .ToolTipText(FText::FromString("Toggle on to be able to edit the grid size. Warning! changing these could possibly result in the loss of voxels!"))
                    .OnCheckStateChanged(this, &SVMEEditorPropertiesTab::SetCanEditGridSizeState)
                ]
            ] +
            SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SBox)
                .Visibility(this, &SVMEEditorPropertiesTab::GetCanEditGrid)
                [
                    SNew(SVerticalBox) +
                    SVerticalBox::Slot()
                    [
                        SNew(SHorizontalBox) +
                        SHorizontalBox::Slot()
                        .Padding(FMargin(4.f, 2.f, 4.f, 2.f))
                        .HAlign(HAlign_Right)
                        .AutoWidth()
                        .FillWidth(0.35f)
                        [
                            SNew(SVerticalBox) +
                            SVerticalBox::Slot()
                            .VAlign(VAlign_Center)
                            [
                                SNew(STextBlock)
                                .Text(FText::FromString("X "))
                                .Justification(ETextJustify::Right)
                            ]
                        ] +
                        SHorizontalBox::Slot()
                        .Padding(FMargin(4.f, 2.f, 4.f, 2.f))
                        [
                            SNew(SNumericEntryBox<int32>)
                            .MinValue(1)
                            .MinSliderValue(1)
                            .MaxSliderValue(128)
                            .SliderExponent(1)
                            .AllowSpin(true)
                            .Value(this, &SVMEEditorPropertiesTab::GetGridSizeX)
                            .OnValueChanged(this, &SVMEEditorPropertiesTab::SetGridSizeX)
                            .OnValueCommitted(this, &SVMEEditorPropertiesTab::OnCommitGridSizeX)
                            .ToolTipText(FText::FromString("Sets the grid size in the X direction.\n To go above 128 manually enter the number."))
                        ] 
                    ] + 
                    SVerticalBox::Slot()
                    [
                        SNew(SHorizontalBox) +
                        SHorizontalBox::Slot()
                        .Padding(FMargin(4.f, 2.f, 4.f, 2.f))
                        .HAlign(HAlign_Right)
                        .AutoWidth()
                        .FillWidth(0.35f)
                        [
                            SNew(SVerticalBox) +
                            SVerticalBox::Slot()
                            .VAlign(VAlign_Center)
                            [
                                SNew(STextBlock)
                                .Text(FText::FromString("Y "))
                                .Justification(ETextJustify::Right)
                            ]
                        ] +
                        SHorizontalBox::Slot()
                        .Padding(FMargin(4.f, 2.f, 4.f, 2.f))
                        [
                            SNew(SNumericEntryBox<int32>)
                            .MinValue(1)
                            .MinSliderValue(1)
                            .MaxSliderValue(128)
                            .SliderExponent(1)
                            .AllowSpin(true)
                            .Value(this, &SVMEEditorPropertiesTab::GetGridSizeY)
                            .OnValueChanged(this, &SVMEEditorPropertiesTab::SetGridSizeY)
                            .OnValueCommitted(this, &SVMEEditorPropertiesTab::OnCommitGridSizeY)
                            .ToolTipText(FText::FromString("Sets the grid size in the Y direction.\n To go above 128 manually enter the number."))
                        ] 
                    ] +
                    SVerticalBox::Slot()
                    [
                        SNew(SHorizontalBox) +
                        SHorizontalBox::Slot()
                        .Padding(FMargin(4.f, 2.f, 4.f, 2.f))
                        .HAlign(HAlign_Right)
                        .AutoWidth()
                        .FillWidth(0.35f)
                        [
                            SNew(SVerticalBox) +
                            SVerticalBox::Slot()
                            .VAlign(VAlign_Center)
                            [
                                SNew(STextBlock)
                                .Text(FText::FromString("Z "))
                                .Justification(ETextJustify::Right)
                            ]
                        ] +
                        SHorizontalBox::Slot()
                        .Padding(FMargin(4.f, 2.f, 4.f, 2.f))
                        [
                            SNew(SNumericEntryBox<int32>)
                            .MinValue(1)
                            .MinSliderValue(1)
                            .MaxSliderValue(128)
                            .SliderExponent(1)
                            .AllowSpin(true)
                            .Value(this, &SVMEEditorPropertiesTab::GetGridSizeZ)
                            .OnValueChanged(this, &SVMEEditorPropertiesTab::SetGridSizeZ)
                            .OnValueCommitted(this, &SVMEEditorPropertiesTab::OnCommitGridSizeZ)
                            .ToolTipText(FText::FromString("Sets the grid size in the Z direction.\n To go above 128 manually enter the number."))
                        ] 
                    ]
                ]
            ] +
            SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SHorizontalBox) +
                SHorizontalBox::Slot()
                .Padding(FMargin(4.f, 2.f, 4.f, 2.f))
                .HAlign(HAlign_Left)
                .AutoWidth()
                .FillWidth(0.35f)
                [
                    SNew(SVerticalBox) +
                    SVerticalBox::Slot()
                    .VAlign(VAlign_Center)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString("Voxel Size"))
                        .Justification(ETextJustify::Left)
                    ]
                ] + 
                SHorizontalBox::Slot()
                .Padding(FMargin(4.f, 2.f, 4.f, 2.f))
                [
                    SNew(SNumericEntryBox<float>)
                    .MinValue(1.f)
                    .MaxValue(1000.f)
                    .MinSliderValue(1.f)
                    .MaxSliderValue(1000.f)
                    .SliderExponent(1.f)
                    .AllowSpin(true)
                    .Value(this, &SVMEEditorPropertiesTab::GetVoxelSize)
                    .OnValueChanged(this, &SVMEEditorPropertiesTab::SetVoxelSize)
                ]
            ] +
            SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SHorizontalBox) +
                SHorizontalBox::Slot()
                .Padding(FMargin(4.f, 2.f, 4.f, 2.f))
                .HAlign(HAlign_Left)
                .AutoWidth()
                .FillWidth(0.35f)
                [
                    SNew(SVerticalBox) +
                    SVerticalBox::Slot()
                    .VAlign(VAlign_Center)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString("Grid Line Width"))
                        .Justification(ETextJustify::Left)
                    ]
                ] + 
                SHorizontalBox::Slot()
                .Padding(FMargin(4.f, 2.f, 4.f, 2.f))
                [
                    SNew(SNumericEntryBox<float>)
                    .MinValue(0.001f)
                    .MaxValue(100.f)
                    .MinSliderValue(0.001f)
                    .MaxSliderValue(100.f)
                    .SliderExponent(1.f)
                    .AllowSpin(true)
                    .Value(this, &SVMEEditorPropertiesTab::GetGridLineWidth)
                    .OnValueChanged(this, &SVMEEditorPropertiesTab::SetGridLineWidth)
                ]
            ] +
            SVerticalBox::Slot()
            .VAlign(VAlign_Bottom)
            [
                SNew(SVerticalBox) +
                SVerticalBox::Slot()
                .AutoHeight()
                .VAlign(VAlign_Bottom)
                [
                    SNew(SBorder)
                    .Padding(FMargin(8.f,2.f,8.f,2.f))
                    [
                        SNew(SVerticalBox) +
                        SVerticalBox::Slot()
                        .VAlign(VAlign_Center)
                        [
                            SNew(STextBlock)
                            .Text(FText::FromString("Color Picker"))
                            .Justification(ETextJustify::Center)
                        ]
                    ]
                ] +
                SVerticalBox::Slot()
                .AutoHeight()
                .VAlign(VAlign_Bottom)
                [
                    SNew(SColorPicker)
                    .OnColorCommitted(this, &SVMEEditorPropertiesTab::SetBrushColor)
                ]
            ]
        ]
    ];
}

SVMEEditorPropertiesTab::~SVMEEditorPropertiesTab()
{

}

TOptional<int32> SVMEEditorPropertiesTab::GetGridSizeX() const
{
    if (!GridRef) return 0;

    return GridRef->GridScaleX;
}

TOptional<int32> SVMEEditorPropertiesTab::GetGridSizeY() const
{
    if (!GridRef) return 0;

    return GridRef->GridScaleY;
}

TOptional<int32> SVMEEditorPropertiesTab::GetGridSizeZ() const
{
    if (!GridRef) return 0;

    return GridRef->GridScaleZ;
}

TOptional<float> SVMEEditorPropertiesTab::GetVoxelSize() const
{
    if (!GridRef) return 0.f;

    return GridRef->VoxelScale;
}

TOptional<float> SVMEEditorPropertiesTab::GetGridLineWidth() const
{
    if (!GridRef) return 0.f;

    return GridRef->GridLineHalfWidth * 2.f;
}

EVisibility SVMEEditorPropertiesTab::GetCanEditGrid() const
{
    if (CanEditGrid == ECheckBoxState::Checked)
    {
        return EVisibility::Visible;
    }

    return EVisibility::Collapsed;
}

void SVMEEditorPropertiesTab::SetGridSizeX(int32 NewValue)
{
    if (!GridRef) return;

    GridRef->GridScaleX = NewValue;
    GridRef->OnGridScaleChanged();
}

void SVMEEditorPropertiesTab::SetGridSizeY(int32 NewValue)
{
    if (!GridRef) return;

    GridRef->GridScaleY = NewValue;
    GridRef->OnGridScaleChanged();
}

void SVMEEditorPropertiesTab::SetGridSizeZ(int32 NewValue)
{
    if (!GridRef) return;

    GridRef->GridScaleZ = NewValue;
    GridRef->OnGridScaleChanged();
}

// Done only once commiting a value so we dont overwrite the buffer each value change
void SVMEEditorPropertiesTab::OnCommitGridSizeX(int32 NewValue, ETextCommit::Type CommitType)
{
    if (!EditorRef) return;

    EditorRef->GridScaleX = NewValue;
    EditorRef->OnGridScaleChanged();
}

// Done only once commiting a value so we dont overwrite the buffer each value change
void SVMEEditorPropertiesTab::OnCommitGridSizeY(int32 NewValue, ETextCommit::Type CommitType)
{
    if (!EditorRef) return;

    EditorRef->GridScaleY = NewValue;
    EditorRef->OnGridScaleChanged();
}

// Done only once commiting a value so we dont overwrite the buffer each value change
void SVMEEditorPropertiesTab::OnCommitGridSizeZ(int32 NewValue, ETextCommit::Type CommitType)
{
    if (!EditorRef) return;

    EditorRef->GridScaleZ = NewValue;
    EditorRef->OnGridScaleChanged();
}

void SVMEEditorPropertiesTab::SetVoxelSize(float NewValue)
{
    if (!GridRef) return;
    if (!EditorRef) return;

    GridRef->VoxelScale = NewValue;
    EditorRef->VoxelScale = NewValue;
    GridRef->OnVoxelScaleChanged();
    EditorRef->OnVoxelScaleChanged();
}

void SVMEEditorPropertiesTab::SetGridLineWidth(float NewValue)
{
    if (!GridRef) return;

    GridRef->GridLineHalfWidth = NewValue / 2.f;
    GridRef->OnGridLineValueChanged();
}

void SVMEEditorPropertiesTab::SetCanEditGridSizeState(ECheckBoxState NewState)
{
    CanEditGrid = NewState;
}

void SVMEEditorPropertiesTab::SetBrushColor(FLinearColor NewColor)
{
    if (!EditorRef) return;

    EditorRef->BrushColor = NewColor;
    EditorRef->BrushColor.A = 1.f;
}

#undef LOCTEXT_NAMESPACE 
