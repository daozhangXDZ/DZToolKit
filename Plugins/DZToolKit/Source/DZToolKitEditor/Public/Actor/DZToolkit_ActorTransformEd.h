#pragma once
#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"
#define LOCTEXT_NAMESPACE "FDZToolKitEdModeToolkit"
struct DZToolkit_ActorTransformEd
{
	static bool IsWidgetEnabled()
	{
		if (GEditor->GetSelectedActors()->Num() >= 1)
		{
			for (int32 i = 0; i < GEditor->GetSelectedActors()->Num(); i++)
			{
				UObject* vObj = GEditor->GetSelectedActors()->GetSelectedObject(i);
				AActor* vAc = Cast<AActor>(vObj);
				if (vAc != nullptr)
				{
					return true;
				}
			}
		}
		return false;
	}

	static bool isShowHanldeListActorBtn()
	{
		int32 minShowCount = 0;
		if (GEditor->GetSelectedActors()->Num() >= 2)
		{
			for (int32 i = 0; i < GEditor->GetSelectedActors()->Num(); i++)
			{
				UObject* vObj = GEditor->GetSelectedActors()->GetSelectedObject(i);
				AActor* vAc = Cast<AActor>(vObj);
				if (vAc != nullptr)
				{
					minShowCount++;
				}
				if (minShowCount>=2)
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
			SNew(SVerticalBox).IsEnabled_Static(&DZToolkit_ActorTransformEd::IsWidgetEnabled)
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Left).Padding(1)
			[
				SNew(STextBlock).AutoWrapText(true).Text(LOCTEXT("Actor Transform Tool", "Actor Transform Tool"))
			]
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(2)
			[
				SNew(STextBlock).AutoWrapText(true).Text(LOCTEXT("-------------------", "-------------------"))
			]
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(1)
			[
				SNew(SVerticalBox).IsEnabled_Static(&DZToolkit_ActorTransformEd::isShowHanldeListActorBtn)
				+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(1)
				[
					SNew(STextBlock).AutoWrapText(true).Text(LOCTEXT("Actor Align", "Actor Align"))
				]
				+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(1)
				[
					SNew(SButton).Text(LOCTEXT("UP", "UP"))
					.OnClicked_Static(&DZToolkit_ActorTransformEd::AlignActorUp)
				]
				+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(1)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().AutoWidth().HAlign(HAlign_Center).Padding(1)
					[
						SNew(SButton).Text(LOCTEXT("Left", "Left"))
						.OnClicked_Static(&DZToolkit_ActorTransformEd::AlignActorLeft)
					]
					+ SHorizontalBox::Slot().AutoWidth().HAlign(HAlign_Center).Padding(1)
					[
						SNew(SButton).Text(LOCTEXT("Center", "Center"))
						.OnClicked_Static(&DZToolkit_ActorTransformEd::AlignActorCenter)
					]
					+ SHorizontalBox::Slot().AutoWidth().HAlign(HAlign_Center).Padding(1)
					[
						SNew(SButton).Text(LOCTEXT("Right", "Right"))
						.OnClicked_Static(&DZToolkit_ActorTransformEd::AlignActorRight)
					]
				]
				+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(1)
				[
						SNew(SButton).Text(LOCTEXT("Down", "Down"))
						.OnClicked_Static(&DZToolkit_ActorTransformEd::AlignActorDown)
				]
			]
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(1)
			[
					SNew(STextBlock).AutoWrapText(true).Text(LOCTEXT("-------------------", "-------------------"))
			]
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(2)
			[
				SNew(SVerticalBox).IsEnabled_Static(&DZToolkit_ActorTransformEd::isShowHanldeListActorBtn)
				+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(1)
				[
					SNew(STextBlock).AutoWrapText(true).Text(LOCTEXT("ActorLst Align", "ActorLst Align"))
				]
				+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(1)
				[
					SNew(SButton).Text(LOCTEXT("SlActorBound", "SlActorBound"))
					.OnClicked_Static(&DZToolkit_ActorTransformEd::SnapActorBounds)
				]
			]
			;
	}

	static FReply AlignActorUp()
	{
		if (GEditor->GetSelectedActors()->Num() >= 2)
		{
			FVector CurrBounds;
			FVector CurrOrigin;
			FVector ProBoundsTop;
			for (int32 i = 0; i < GEditor->GetSelectedActors()->Num(); i++)
			{
				UObject* vObj = GEditor->GetSelectedActors()->GetSelectedObject(i);
				AActor* vAc = Cast<AActor>(vObj);
				if (vAc != nullptr)
				{
					vAc->GetActorBounds(false, CurrOrigin, CurrBounds);
					if (i==0)
					{
						ProBoundsTop = CurrOrigin + FVector(0,0, CurrBounds.Z);
					}
					else 
					{
						if ( CurrOrigin.Z + CurrBounds.Z > ProBoundsTop.Z )
						{
							ProBoundsTop = CurrOrigin + FVector(0, 0, CurrBounds.Z);
						}
					}
				}
			}
			FVector vposTemp;
			for (int32 i = 0; i < GEditor->GetSelectedActors()->Num(); i++)
			{
				UObject* vObj = GEditor->GetSelectedActors()->GetSelectedObject(i);
				AActor* vAc = Cast<AActor>(vObj);
				if (vAc != nullptr)
				{
					vAc->GetActorBounds(false, CurrOrigin, CurrBounds);
					vposTemp = vAc->GetActorLocation();
					vAc->SetActorLocation(FVector(vposTemp.X, vposTemp.Y, ProBoundsTop.Z - CurrBounds.Z));
				}
			}
		}
		return FReply::Handled();
	}

	static FReply AlignActorDown()
	{
		if (GEditor->GetSelectedActors()->Num() >= 2)
		{
			FVector CurrBounds;
			FVector CurrOrigin;
			FVector ProBoundsDown;
			for (int32 i = 0; i < GEditor->GetSelectedActors()->Num(); i++)
			{
				UObject* vObj = GEditor->GetSelectedActors()->GetSelectedObject(i);
				AActor* vAc = Cast<AActor>(vObj);
				if (vAc != nullptr)
				{
					vAc->GetActorBounds(false, CurrOrigin, CurrBounds);
					if (i == 0)
					{
						ProBoundsDown = CurrOrigin - FVector(0, 0, CurrBounds.Z);
					}
					else
					{
						if ( CurrOrigin.Z + CurrBounds.Z < ProBoundsDown.Z)
						{
							ProBoundsDown = CurrOrigin - FVector(0, 0, CurrBounds.Z);
						}
					}
				}
			}
			FVector vposTemp;
			for (int32 i = 0; i < GEditor->GetSelectedActors()->Num(); i++)
			{
				UObject* vObj = GEditor->GetSelectedActors()->GetSelectedObject(i);
				AActor* vAc = Cast<AActor>(vObj);
				if (vAc != nullptr)
				{
					vAc->GetActorBounds(false, CurrOrigin, CurrBounds);
					vposTemp = vAc->GetActorLocation();
					vAc->SetActorLocation(FVector(vposTemp.X, vposTemp.Y, ProBoundsDown.Z + CurrBounds.Z));
				}
			}
		}
		return FReply::Handled();
	}

	static FReply AlignActorLeft()
	{
		if (GEditor->GetSelectedActors()->Num() >= 2)
		{
			FVector CurrBounds;
			FVector CurrOrigin;
			FVector ProBoundsLeft;
			for (int32 i = 0; i < GEditor->GetSelectedActors()->Num(); i++)
			{
				UObject* vObj = GEditor->GetSelectedActors()->GetSelectedObject(i);
				AActor* vAc = Cast<AActor>(vObj);
				if (vAc != nullptr)
				{
					vAc->GetActorBounds(false, CurrOrigin, CurrBounds);
					if (i == 0)
					{
						ProBoundsLeft = CurrOrigin - FVector(0, CurrBounds.Y, 0);
					}
					else
					{
						if (CurrOrigin.Z + CurrBounds.Z < ProBoundsLeft.Z )
						{
							ProBoundsLeft = CurrOrigin - FVector(0, CurrBounds.Y, 0);
						}
					}
				}
			}
			FVector vposTemp;
			for (int32 i = 0; i < GEditor->GetSelectedActors()->Num(); i++)
			{
				UObject* vObj = GEditor->GetSelectedActors()->GetSelectedObject(i);
				AActor* vAc = Cast<AActor>(vObj);
				if (vAc != nullptr)
				{
					vAc->GetActorBounds(false, CurrOrigin, CurrBounds);
					vposTemp = vAc->GetActorLocation();
					vAc->SetActorLocation(FVector(vposTemp.X, ProBoundsLeft.Y + CurrBounds.Y, vposTemp.Z));
				}
			}
		}
		return FReply::Handled();
	}

	static FReply AlignActorCenter()
	{
		if (GEditor->GetSelectedActors()->Num() >= 2)
		{
			FVector CurrBounds;
			FVector CurrOrigin;
			FVector ProBoundsCenter;
			for (int32 i = 0; i < GEditor->GetSelectedActors()->Num(); i++)
			{
				UObject* vObj = GEditor->GetSelectedActors()->GetSelectedObject(i);
				AActor* vAc = Cast<AActor>(vObj);
				if (vAc != nullptr)
				{
					vAc->GetActorBounds(false, CurrOrigin, CurrBounds);
					if (i == 0)
					{
						ProBoundsCenter = CurrOrigin;
					}
					else
					{
						vAc->SetActorLocation(ProBoundsCenter);
					}
				}
			}
			
		}
		return FReply::Handled();
	}

	static FReply AlignActorRight()
	{
		if (GEditor->GetSelectedActors()->Num() >= 2)
		{
			FVector CurrBounds;
			FVector CurrOrigin;
			FVector ProBoundsRight;
			for (int32 i = 0; i < GEditor->GetSelectedActors()->Num(); i++)
			{
				UObject* vObj = GEditor->GetSelectedActors()->GetSelectedObject(i);
				AActor* vAc = Cast<AActor>(vObj);
				if (vAc != nullptr)
				{
					vAc->GetActorBounds(false, CurrOrigin, CurrBounds);
					if (i == 0)
					{
						ProBoundsRight = CurrOrigin + FVector(0, CurrBounds.Y, 0);
					}
					else
					{
						if (CurrOrigin.Z + CurrBounds.Z > ProBoundsRight.Z)
						{
							ProBoundsRight = CurrOrigin + FVector(0, CurrBounds.Y, 0);
						}
					}
				}
			}
			FVector vposTemp;
			for (int32 i = 0; i < GEditor->GetSelectedActors()->Num(); i++)
			{
				UObject* vObj = GEditor->GetSelectedActors()->GetSelectedObject(i);
				AActor* vAc = Cast<AActor>(vObj);
				if (vAc != nullptr)
				{
					vAc->GetActorBounds(false, CurrOrigin, CurrBounds);
					vposTemp = vAc->GetActorLocation();
					vAc->SetActorLocation(FVector(vposTemp.X, ProBoundsRight.Y - CurrBounds.Y, vposTemp.Z));
				}
			}
		}
		return FReply::Handled();
	}

	static FReply SnapActorBounds()
	{
		return FReply::Handled();
	}


	static FReply AlignSplineBtnClick()
	{
		return FReply::Handled();
	}
};
#undef LOCTEXT_NAMESPACE