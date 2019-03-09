// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "DZToolKitEdModeToolkit.h"
#include "DZToolkitEdMode.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"

#include "Spline/DZToolKitSplineEd.h"
#include "Light/DZToolKit_LightEd.h"
#include "Actor/DZToolkit_ActorTransformEd.h"
#define LOCTEXT_NAMESPACE "FDZToolKitEdModeToolkit"


FDZToolKitEdModeToolkit::FDZToolKitEdModeToolkit()
{
}

void FDZToolKitEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Left)
		.Padding(5)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Left).Padding(1)
			[
				SNew(STextBlock).AutoWrapText(true).Text(LOCTEXT("GYVR_LevelToolKit", "GYVR_LevelToolKit"))
			]
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Left).Padding(5)
			[
				SplineEdFunLibLocals::CreateContent()
			]
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Left).Padding(5)
			[
				DZToolkit_ActorTransformEd::CreateContent()
			]
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Left).Padding(5)
			[
				LightEdFunLibLocals::CreateContent()
			]
		];
	FModeToolkit::Init(InitToolkitHost);
}

FName FDZToolKitEdModeToolkit::GetToolkitFName() const
{
	return FName("DZToolKitEdMode");
}

FText FDZToolKitEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("DZToolKitEdModeToolkit", "DisplayName", "DZToolKitEdModeToolkit Tool");
}

class FEdMode* FDZToolKitEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FDZToolKitEdMode::EM_DZToolKitEdModeID);
}

#undef LOCTEXT_NAMESPACE
