// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Framework/Commands/UICommandList.h"

/**
 * Tab tool panel tool bar widget
 */
class STabToolToolBar : public SCompoundWidget
{
public:

    SLATE_BEGIN_ARGS(STabToolToolBar)
    {
    }
    SLATE_END_ARGS()

public:
    void Construct(const FArguments& InArgs, const TSharedRef<FUICommandList>& CommandList);
};
