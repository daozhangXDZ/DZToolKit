#pragma once
#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"
#define LOCTEXT_NAMESPACE "FDZToolKitEdModeToolkit"
struct LightEdFunLibLocals
{
	static bool IsWidgetEnabled()
	{
		if (GEditor->GetSelectedActors()->Num() >= 1)
		{
			for (int32 i = 0; i < GEditor->GetSelectedActors()->Num(); i++)
			{
				UObject* vObj = GEditor->GetSelectedActors()->GetSelectedObject(i);
				AActor* vAc = Cast<AActor>(vObj);
				if (vAc == nullptr)
				{
					continue;
				}
				UDirectionalLightComponent* SplineCom = Cast<UDirectionalLightComponent>(vAc->GetComponentByClass(UDirectionalLightComponent::StaticClass()));
				if (SplineCom != nullptr)
				{
					return true;
				}
			}
		}
		return false;
	}

	static TSharedRef<SWidget> CreateContent()
	{
		return
			/*SNew(SVerticalBox)
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Left).Padding(50)
			[*/
			SNew(SVerticalBox).IsEnabled_Static(&LightEdFunLibLocals::IsWidgetEnabled)
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Left).Padding(5)
			[
				SNew(STextBlock).AutoWrapText(true).Text(LOCTEXT("Light Extend Tool", "Light Extend Tool"))
			]
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(2)
			[
				SNew(STextBlock).AutoWrapText(true).Text(LOCTEXT("-------------------", "-------------------"))
			]
		+ SVerticalBox::Slot().HAlign(HAlign_Left).AutoHeight()
			[
				SNew(SButton).Text(LOCTEXT("Lanscape Shadow", "Lanscape Shadow"))
			]
		+ SVerticalBox::Slot().HAlign(HAlign_Left).AutoHeight()
			[
				SNew(SButton).Text(LOCTEXT("Sanny Light0", "Sanny Light0"))
			]
		+SVerticalBox::Slot().HAlign(HAlign_Left).AutoHeight()
			[
				SNew(SButton).Text(LOCTEXT("Sanny Light1", "Sanny Light1"))
			]
		+SVerticalBox::Slot().HAlign(HAlign_Left).AutoHeight()
			[
				SNew(SButton).Text(LOCTEXT("Sanny Light2", "Sanny Light2"))
			]
		+SVerticalBox::Slot().HAlign(HAlign_Left).AutoHeight()
			[
				SNew(SButton).Text(LOCTEXT("Sanny Light3", "Sanny Light3"))
			] 
		+ SVerticalBox::Slot().HAlign(HAlign_Left).AutoHeight()
			[
				SNew(SButton).Text(LOCTEXT("Sanny Light2", "Sanny Light2"))
			]
		+ SVerticalBox::Slot().HAlign(HAlign_Left).AutoHeight()
			[
				SNew(SButton).Text(LOCTEXT("Sanny Light2", "Sanny Light2"))
			]
		+ SVerticalBox::Slot().HAlign(HAlign_Left).AutoHeight()
			[
				SNew(SButton).Text(LOCTEXT("Sanny Light2", "Sanny Light2"))
			]
		+ SVerticalBox::Slot().HAlign(HAlign_Left).AutoHeight()
			[
				SNew(SButton).Text(LOCTEXT("Sanny Light2", "Sanny Light2"))
			]
		+ SVerticalBox::Slot().HAlign(HAlign_Left).AutoHeight()
			[
				SNew(SButton).Text(LOCTEXT("Sanny Light2", "Sanny Light2"))
			];
		//];
	}


	static FReply AlignSplineBtnClick()
	{
		return FReply::Handled();
	}
};
#undef LOCTEXT_NAMESPACE