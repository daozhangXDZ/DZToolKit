#pragma once
#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"
#include "Components/SplineComponent.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Public/DrawDebugHelpers.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "FDZToolKitEdModeToolkit"
struct SplineEdFunLibLocals
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
				USplineComponent* SplineCom = Cast<USplineComponent>(vAc->GetComponentByClass(USplineComponent::StaticClass()));
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
			SNew(SVerticalBox).IsEnabled_Static(&SplineEdFunLibLocals::IsWidgetEnabled)
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Left).Padding(5)
			[
				SNew(STextBlock).AutoWrapText(true).Text(LOCTEXT("Spline Extend Tool", "select SplineComponent"))
			]
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(2)
			[
				SNew(STextBlock).AutoWrapText(true).Text(LOCTEXT("-------------------", "-------------------"))
			]
		+ SVerticalBox::Slot().HAlign(HAlign_Left).AutoHeight()
			[
				SNew(SButton).Text(LOCTEXT("Spline Aling Mesh", "Spline Aling Mesh"))
				.IsEnabled_Static(&SplineEdFunLibLocals::isShowAlighBtn)
			.OnClicked_Static(&SplineEdFunLibLocals::AlignSplineBtnClick)
			]
		+ SVerticalBox::Slot().HAlign(HAlign_Left).AutoHeight()
			[
				SNew(SButton).Text(LOCTEXT("Spline Aline Lanscape", "Spline Aline Lanscape"))
				.OnClicked_Static(&SplineEdFunLibLocals::AlignSplineBtnClick)
			];
		//];
	}

	static bool isShowAlighBtn()
	{
		if (GEditor->GetSelectedActors()->Num() >= 2)
		{
			for (int32 i = 0; i < GEditor->GetSelectedActors()->Num(); i++)
			{
				UObject* vObj = GEditor->GetSelectedActors()->GetSelectedObject(i);
				AActor* vAc = Cast<AActor>(vObj);
				if (vAc == nullptr)
				{
					continue;
				}
				USplineComponent* SplineCom = Cast<USplineComponent>(vAc->GetComponentByClass(USplineComponent::StaticClass()));
				if (SplineCom != nullptr)
				{
					return true;
				}
			}
		}
		return false;
	}

	static FReply AlignSplineBtnClick()
	{
		USplineComponent* SplineCom = nullptr;
		AActor* SplineTarget = nullptr;
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
				USplineComponent* vSplineCom = Cast<USplineComponent>(vAc->GetComponentByClass(USplineComponent::StaticClass()));
				if (vSplineCom != nullptr)
				{
					SplineCom = vSplineCom;
				}
				else
				{
					SplineTarget = vAc;
				}
			}
		}
		if (SplineTarget != nullptr && SplineCom != nullptr)
		{
			FTransform vSplineTrans = SplineCom->GetComponentTransform();
			int32 vSpliCount = SplineCom->GetNumberOfSplinePoints();
			float vMaxDistance = SplineCom->GetSplineLength();
			float vSeq = vMaxDistance / (	(vSpliCount < 10)?20.0f:50.0f	);
			float vCurrDistance = 0;
			TArray<FVector> vPoints;
			vPoints.Reset(vSpliCount);

			FCollisionQueryParams ccq(FName(TEXT("CombatTrace")), true, NULL);
			ccq.bTraceComplex = true;
			ccq.bTraceAsyncScene = false;
			ccq.bReturnPhysicalMaterial = false;
			ccq.AddIgnoredActor(SplineCom->GetOwner());
			while(vCurrDistance<= vMaxDistance)
			{
				FHitResult  vHitResult;
				bool isCheckSuccess = false;
				FVector vStartPos = SplineCom->GetWorldLocationAtDistanceAlongSpline(vCurrDistance);
				FVector vRightPos = SplineCom->GetRightVectorAtDistanceAlongSpline(vCurrDistance, ESplineCoordinateSpace::World);
				FVector vUpPos = SplineCom->GetUpVectorAtDistanceAlongSpline(vCurrDistance, ESplineCoordinateSpace::World);
				FVector vEndPosRight = vStartPos + vRightPos * 300;
				if (
					SplineCom->GetWorld()->LineTraceSingleByChannel(vHitResult, vStartPos, vEndPosRight,ECollisionChannel::ECC_Visibility, ccq)
					&& vHitResult.GetActor() == SplineTarget
					)
				{
					isCheckSuccess = true;
				}
				else
				{
					vEndPosRight = vStartPos + vRightPos * -300;
					if (SplineCom->GetWorld()->LineTraceSingleByChannel(vHitResult, vStartPos, vEndPosRight, ECollisionChannel::ECC_Visibility, ccq)
						&& vHitResult.GetActor() == SplineTarget
						)
					{
						isCheckSuccess = true;
					}
					else
					{   
						FVector vEndPosUp = vStartPos + vUpPos * 300;
						if (SplineCom->GetWorld()->LineTraceSingleByChannel(vHitResult, vStartPos, vEndPosRight, ECollisionChannel::ECC_Visibility, ccq)
							&& vHitResult.GetActor() == SplineTarget
							)
						{
							isCheckSuccess = true;
						}
						else
						{
							vEndPosUp = vStartPos + vUpPos * -300;
							if (SplineCom->GetWorld()->LineTraceSingleByChannel(vHitResult, vStartPos, vEndPosRight, ECollisionChannel::ECC_Visibility, ccq)
								&& vHitResult.GetActor() == SplineTarget
								)
							{
								isCheckSuccess = true;
							}
						}
					}
				}

				if (isCheckSuccess)
				{
					//FVector vReaLoc = UKismetMathLibrary::InverseTransformLocation(vSplineTrans, vHitResult.Location);
					vPoints.Add(vHitResult.Location);
					DrawDebugSphere(SplineCom->GetWorld(), vHitResult.Location, 10, 10, FColor::Red, false, 20.f);
				}
				vCurrDistance += vSeq;
			}
		
			vSpliCount = vPoints.Num();
			#pragma region Do Check Points,if Collision between two point,
			if (vSpliCount >= 2)
			{
				/*
				FCollisionQueryParams ccqTwoPoint(FName(TEXT("CombatTrace")), true, NULL);
				ccq.bTraceComplex = true;
				ccq.bTraceAsyncScene = false;
				ccq.bReturnPhysicalMaterial = false;
				FHitResult vHitResult;
				int32 vCheckIndex = 0;
				int32 vTotal = vPoints.Num();
				while (vCheckIndex < vTotal)
				{
					if (SplineCom->GetWorld()->LineTraceSingleByChannel(vHitResult, vPoints[vCheckIndex], vPoints[vCheckIndex+1], ECollisionChannel::ECC_Visibility, ccq))
					{
						vPoints.Insert(vHitResult.Location, vCheckIndex+1);
						vTotal++;
					}
					vCheckIndex++;
				}
				*/
			#pragma endregion
			SplineCom->ClearSplinePoints(true);
			SplineCom->SetSplinePoints(vPoints, ESplineCoordinateSpace::World, true);
			#pragma region Do Change Point Tangents At The End of
				for (int32 i = 0; i < vSpliCount - 1; i++)
				{
					FVector vInArriveTangent = FVector(0, 0, 0);
					SplineCom->SetTangentsAtSplinePoint(i, vInArriveTangent, vInArriveTangent, ESplineCoordinateSpace::Local, true);
				}
			#pragma endregion
			}
			
		}
		return FReply::Handled();
	}
};
#undef LOCTEXT_NAMESPACE