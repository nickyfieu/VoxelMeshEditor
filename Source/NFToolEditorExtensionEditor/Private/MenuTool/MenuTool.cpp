// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#include "MenuTool/MenuTool.h"
#include "../NFToolEditorModule.h"
#include "Engine/Selection.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealEd.h"

#define LOCTEXT_NAMESPACE "MenuTool"

// ----------------------- //
// MenuTool implementation //
// ----------------------- //

MenuTool::~MenuTool()
{
}

void MenuTool::OnStartupModule()
{
    CommandList = MakeShareable(new FUICommandList);

    MenuToolCommands::Register();

    MapCommands();

    FNFToolEditorModule::Get().AddMenuExtension(
        FName("NFAssetTools"),
        FMenuExtensionDelegate::CreateRaw(this, &MenuTool::MakeAssetToolsMenuEntries),
        EExtensionHook::Position::After,
        CommandList
    );

    FNFToolEditorModule::Get().AddMenuExtension(
        FName("NFDebugTools"),
        FMenuExtensionDelegate::CreateRaw(this, &MenuTool::MakeDebugToolsMenuEntries),
        EExtensionHook::Position::After,
        CommandList
    );
}

void MenuTool::OnShutdownModule()
{
    MenuToolCommands::Unregister();
}

void MenuTool::MakeAssetToolsMenuEntries(FMenuBuilder& MenuBuilder)
{
    //MenuBuilder.AddMenuEntry(MenuToolCommands::Get().AssetToolTestCommand);
}

void MenuTool::MakeDebugToolsMenuEntries(FMenuBuilder& MenuBuilder)
{
    MenuBuilder.AddSubMenu(
        FText::FromString("Tag utilities"),
        FText::FromString("Utility tools doing stuf with tags."),
        FNewMenuDelegate::CreateSP(this, &MenuTool::MakeTagUtilitySubMenu)
    );
}

void MenuTool::MakeTagUtilitySubMenu(FMenuBuilder& MenuBuilder)
{
    TSharedRef<SWidget> AddTagWidget =
        SNew(SHorizontalBox) +
        SHorizontalBox::Slot()
        .AutoWidth()
        .VAlign(VAlign_Center)
        [
            SNew(SBox)
            .WidthOverride(125.f)
            [
                SNew(SEditableTextBox)
                .MinDesiredWidth(125.f)
                .Text(this, &MenuTool::GetTagUtilityText)
                .OnTextCommitted(this, &MenuTool::OnTagUtilityTextCommitted)
            ]
        ] +
        SHorizontalBox::Slot()
        .AutoWidth()
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Center)
        [
            SNew(SBox)
            .WidthOverride(125.f)
            [
                SNew(SButton)
                .HAlign(HAlign_Center)
                .Text(FText::FromString("Add Tag"))
                .OnClicked(this, &MenuTool::AddTag)
            ]
        ];
    MenuBuilder.AddWidget(AddTagWidget, FText::FromString(""));

    TSharedRef<SWidget> RemoveTagWidget =
        SNew(SHorizontalBox) +
        SHorizontalBox::Slot()
        .AutoWidth()
        .VAlign(VAlign_Center)
        [
            SNew(SBox)
            .WidthOverride(125.f)
        ] +
        SHorizontalBox::Slot()
        .AutoWidth()
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Center)
        [
            SNew(SBox)
            .WidthOverride(125.f)
            [
                SNew(SButton)
                .HAlign(HAlign_Center)
                .Text(FText::FromString("Remove Tag"))
                .OnClicked(this, &MenuTool::RemoveTag)
            ]
        ];
    MenuBuilder.AddWidget(RemoveTagWidget, FText::FromString(""));

    TSharedRef<SWidget> SelectTagWidget =
        SNew(SHorizontalBox) +
        SHorizontalBox::Slot()
        .AutoWidth()
        .VAlign(VAlign_Center)
        [
            SNew(SBox)
            .WidthOverride(125.f)
        ] +
        SHorizontalBox::Slot()
        .AutoWidth()
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Center)
        [
            SNew(SBox)
            .WidthOverride(125.f)
            [
                SNew(SButton)
                .HAlign(HAlign_Center)
                .Text(FText::FromString("Select Tag"))
                .OnClicked(this, &MenuTool::SelectTag)
            ]
        ];
    MenuBuilder.AddWidget(SelectTagWidget, FText::FromString(""));
}

void MenuTool::MapCommands()
{
    // Maps the asset tool test command
    //CommandList->MapAction(                                                 //Map action layout:
    //    MenuToolCommands::Get().AssetToolTestCommand,                       //  UI command info
    //    FExecuteAction::CreateSP(this, &MenuTool::AssetToolTestCommand),    //  Action to execute
    //    FCanExecuteAction()                                                 //  Action repeat mode
    //);
}

//void MenuTool::AssetToolTestCommand()
//{
//    UE_LOG(LogNFToolEditor, Log, TEXT("Function fired! :D next up make this pop up an editor window like material graph!"));
//}

FText MenuTool::GetTagUtilityText() const
{
    return FText::FromName(TagUtility);
}

void MenuTool::OnTagUtilityTextCommitted(const FText& InTag, ETextCommit::Type CommitInfo)
{
    FString input = InTag.ToString();
    TagUtility = FName(input);
}

FReply MenuTool::AddTag()
{
    if (!TagUtility.IsNone())
    {
        // Wrap this function around a transaction so it will support the undo action!
        const FScopedTransaction Transaction(FText::FromString("Add Tag"));

        for (FSelectionIterator It(GEditor->GetSelectedActorIterator()); It; ++It)
        {
            AActor* SelectedActor = Cast<AActor>(*It);
            if (!SelectedActor->Tags.Contains(TagUtility))
            {
                SelectedActor->Modify();
                SelectedActor->Tags.Add(TagUtility);
            }
        }
    }
    return FReply::Handled();
}

FReply MenuTool::RemoveTag()
{
    if (!TagUtility.IsNone())
    {
        // Wrap this function around a transaction so it will support the undo action!
        const FScopedTransaction Transaction(FText::FromString("Remove Tag"));

        for (FSelectionIterator It(GEditor->GetSelectedActorIterator()); It; ++It)
        {
            AActor* SelectedActor = Cast<AActor>(*It);
            if (SelectedActor->Tags.Contains(TagUtility))
            {
                SelectedActor->Modify();
                SelectedActor->Tags.Remove(TagUtility);
            }
        }
    }
    return FReply::Handled();
}

FReply MenuTool::SelectTag()
{
    // Deselecting all current selections
    GEngine->Exec(GEditor->GetEditorWorldContext().World(), TEXT("select none"));

    if (!TagUtility.IsNone())
    {
        // Wrap this function around a transaction so it will support the undo action!
        const FScopedTransaction Transaction(FText::FromString("Select Tag"));


        TArray<AActor*> AllLevelActors;
        UGameplayStatics::GetAllActorsOfClass(GEditor->GetEditorWorldContext().World(), AActor::StaticClass(), AllLevelActors);

        for (int32 i{}; i < AllLevelActors.Num(); ++i)
        {
            if (AllLevelActors[i]->ActorHasTag(TagUtility))
            {
                GEditor->SelectActor(AllLevelActors[i], true, true);
            }
        }
    }
    return FReply::Handled();
}

// ------------------------------- //
// MenuToolCommands implementation //
// ------------------------------- //

MenuToolCommands::MenuToolCommands()
    :TCommands<MenuToolCommands>(
         "MenuToolCommands",                        // Lookup name
         FText::FromString("MenuToolCommands"),     // Display name
         NAME_None,                                 // Lookup name of parrent
         FEditorStyle::GetStyleSetName()            // Icon style
     )
{
}

void MenuToolCommands::RegisterCommands()
{
    //UI_COMMAND(
    //    AssetToolTestCommand,
    //    "AssetToolTestCommand",
    //    "Should open the soon to be created asset tool!",
    //    EUserInterfaceActionType::Button,
    //    FInputGesture()
    //);

    UI_COMMAND(
        DebugToolSubMenuTestCommand1,
        "SubMenu Command 2",
        "Some Test Command",
        EUserInterfaceActionType::Button,
        FInputGesture()
    );

    UI_COMMAND(
        DebugToolSubMenuTestCommand2,
        "SubMenu Command 2",
        "Some Test Command",
        EUserInterfaceActionType::Button,
        FInputGesture()
    );
}

#undef LOCTEXT_NAMESPACE 