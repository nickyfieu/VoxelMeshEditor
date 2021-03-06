// Copyright © 2021 Nicky Fieu, All Rights Reserved.

#include "TabTool/STabToolToolBar.h"
#include "TabTool/TabToolCommands.h"
#include "TabTool/TabToolStyle.h"
#include "NFToolEditorModule.h"
#include "SlateOptMacros.h"
#include "ToolMenus.h"
#include <Styling/ToolBarStyle.h>

    /*
        FNFToolEditorModule& NFToolEditorModule = FModuleManager::LoadModuleChecked<FNFToolEditorModule>("NFToolEditorExtensionEditor");
        FToolMenuContext MenuContext{ InCommandList, NFToolEditorModule.GetTabToolExtensibilityManager()->GetAllExtenders() };

        struct Local
        {
            static const FSlateBrush* GetTestSaveIconBrush()
            {
                if (const ISlateStyle* Style = FSlateStyleRegistry::FindSlateStyle(FModuleStyleIds::NFToolEditorModuleId))
                {
                    return (*Style).GetBrush("NFToolEditor.TabTool.TestSaveImage");
                }
        
                return FCoreStyle::GetCoreStyle().GetBrush("NFToolEditor.TabTool.TestSaveImage");
            }
        };
        
        return  SNew(SOverlay) +
            SOverlay::Slot()
            [
                SNew(SHorizontalBox) +
                SHorizontalBox::Slot()
                .AutoWidth()
                .VAlign(VAlign_Center)
                .HAlign(HAlign_Center)
                [
                    SNew(SImage)
                    .Image_Static(&Local::GetTestSaveIconBrush)
                ]
            ];
     */

#define LOCTEXT_NAMESPACE "TabTool"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void STabToolToolBar::Construct(const FArguments& InArgs, const TSharedRef<FUICommandList>& CommandList)
{

    /* Create the toolbar */
    FToolBarBuilder Toolbar(CommandList, FMultiBoxCustomization::None);
    {   /* Create the toolbar layout */
        Toolbar.AddToolBarButton(
            FTabToolCommands::Get().TestCommand,
            NAME_None,
            LOCTEXT("Test", "Test"),
            LOCTEXT("TestCommandTooltip", "Test Command Tooltip"),
            FSlateIcon(FTabToolStyle::GetInstance().GetStyleSetName(), "TabTool.TestSaveImage")
        );
    }

    //FNFToolEditorModule& NFToolEditorModule = FNFToolEditorModule::Get();
    //FToolMenuContext MenuContext(CommandList, NFToolEditorModule.GetTabToolExtensibilityManager().Get()->GetAllExtenders());
    //UTabToolMenuContext* TabToolMenuContext = NewObject<UTabToolMenuContext>();
    //TabToolMenuContext->MainTab = MainTab;
    //MenuContext.AddObject(TabToolMenuContext);
    //
    ///* Create the toolbar (other version) */
    //UToolMenu* TabToolToolBar = UToolMenus::Get()->RegisterMenu("TabTool.TabToolToolBar.TestSection");
    //TabToolToolBar->StyleName = "TabTool";
    //{   /** Create toolbar entries */
    //    { /** Section Test_A */
    //        FToolMenuSection& Section = TabToolToolBar->AddSection("Test_A");
    //
    //        Section.AddEntry(
    //            FToolMenuEntry::InitToolBarButton(
    //                FTabToolCommands::Get().TestCommand,
    //                TAttribute<FText>(),
    //                TAttribute<FText>(),
    //                FSlateIcon(FTabToolStyle::GetInstance().GetStyleSetName(), "TabTool.TestSaveImage"),
    //                NAME_None,
    //                FName("Test")
    //            )
    //        );
    //    }
    //}

    ChildSlot
    [
        SNew(SBorder)
        .Padding(2.5f)
        [
            SNew(SOverlay) +
            SOverlay::Slot()
            [
                SNew(SImage)
                .Image(&FAppStyle::Get().GetWidgetStyle<FToolBarStyle>("SlimToolBar").BackgroundBrush)
            ] +
            SOverlay::Slot()
            [
                SNew(SHorizontalBox) +
                SHorizontalBox::Slot()
                .Padding(7.0f, 0.0f, 0.0f, 0.0f)
                .AutoWidth()
                [
                    Toolbar.MakeWidget()
                ] +
                SHorizontalBox::Slot()
                .AutoWidth()
                [
                    SNew(SSpacer)
                    .Size(FVector2D(120.f, 1.0f))
                ]// +
                //SHorizontalBox::Slot()
                //.AutoWidth()
                //.HAlign(HAlign_Left)
                //[
                //    UToolMenus::Get()->GenerateWidget("TabTool.TabToolToolBar.TestSection", MenuContext)
                //]
            ]
        ]
    ];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE