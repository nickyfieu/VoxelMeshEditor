// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/InputChord.h"
#include "EditorStyleSet.h"
#include "Framework/Commands/Commands.h"

#define LOCTEXT_NAMESPACE "TabTool"

class FTabToolCommands : public TCommands<FTabToolCommands>
{
public:

    FTabToolCommands()
        : TCommands<FTabToolCommands>(
            "TabTool",
            NSLOCTEXT("Contexts", "NFToolEditor.TabTool", "NFToolEditor.TabTool Plugin"),
            NAME_None,
            FEditorStyle::GetStyleSetName()
          )
    {
    }

    // TCommands Interface
    virtual void RegisterCommands() override
    {
        //UI_COMMAND(...)
        UI_COMMAND(TestCommand, "TestButton", "Prints a message to the log", EUserInterfaceActionType::Button, FInputChord());
    }
    // End of TCommands Interface

public:
    TSharedPtr<FUICommandInfo> TestCommand;
};

#undef LOCTEXT_NAMESPACE