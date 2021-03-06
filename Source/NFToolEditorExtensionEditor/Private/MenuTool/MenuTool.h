// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#pragma once

#include "NFToolsInterfaces.h"
#include "CoreMinimal.h"

class MenuTool : public INFToolsModuleListenerInterface, public TSharedFromThis<MenuTool>
{
public:
	virtual ~MenuTool();

    // INFToolsModuleListenerInterface Interface
    virtual void OnStartupModule() override;
    virtual void OnShutdownModule() override;
    // End of INFToolsModuleListenerInterface Interface

    void MakeAssetToolsMenuEntries(FMenuBuilder& MenuBuilder);
    void MakeDebugToolsMenuEntries(FMenuBuilder& MenuBuilder);
    void MakeTagUtilitySubMenu(FMenuBuilder& MenuBuilder);

    FText GetTagUtilityText() const;

protected:
    void MapCommands();
    //void AssetToolTestCommand();
    void OnTagUtilityTextCommitted(const FText& InTag, ETextCommit::Type CommitInfo);
    FReply AddTag();
    FReply RemoveTag();
    FReply SelectTag();

protected:
    TSharedPtr<FUICommandList> CommandList;
    FName TagUtility;
};

class MenuToolCommands : public TCommands<MenuToolCommands>
{
public:
    MenuToolCommands();

    virtual void RegisterCommands() override;

public:
    TSharedPtr<FUICommandInfo> AssetToolTestCommand;
    TSharedPtr<FUICommandInfo> DebugToolSubMenuTestCommand1;
    TSharedPtr<FUICommandInfo> DebugToolSubMenuTestCommand2;
};
