// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Component/DZProjectileMovementComponent.h"
#include "EngineDefines.h"
#include "GameFramework/DamageType.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/WorldSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogProjectileMovement, Log, All);


UDZProjectileMovementComponent::UDZProjectileMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, isSalfMove(true)
	, isUseTelepMove(false)
	, ExtendChackStep(20.0f)
	, ExtendChackTimeScale(1.0f)
{
	
}


void UDZProjectileMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	mOQParams = FCollisionObjectQueryParams();
	for (int i=0 ; i< CheckCollisionChannel.Num(); i++ )
	{
		mOQParams.AddObjectTypesToQuery(CheckCollisionChannel[i]);
	}
	mQParams = FCollisionQueryParams();
	TArray<UPrimitiveComponent*> ingCom;
	TArray<UActorComponent*>   vTempFile = UpdatedComponent->GetOwner()->GetComponentsByClass(UPrimitiveComponent::StaticClass());
	for (int i = 0; i < vTempFile.Num(); i++)
	{
		ingCom.Add(Cast<UPrimitiveComponent>(vTempFile[i]));
	}
	mQParams.AddIgnoredComponents(ingCom);
}


void UDZProjectileMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (HasStoppedSimulation() || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	UMovementComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(UpdatedComponent))
	{
		return;
	}

	AActor* ActorOwner = UpdatedComponent->GetOwner();
	if (!ActorOwner || !CheckStillInWorld())
	{
		return;
	}

	if (UpdatedComponent->IsSimulatingPhysics())
	{
		return;
	}

	float RemainingTime = DeltaTime;
	uint32 NumBounces = 0;
	int32 Iterations = 0;
	FHitResult Hit(1.f);

	while (RemainingTime >= MIN_TICK_TIME*ExtendChackTimeScale && (Iterations < MaxSimulationIterations+ExtendChackStep) && !ActorOwner->IsPendingKill() && !HasStoppedSimulation())
	{
		Iterations++;

		// subdivide long ticks to more closely follow parabolic trajectory
		const float TimeTick = ShouldUseSubStepping() ? GetSimulationTimeStep(RemainingTime, Iterations) : RemainingTime;
		RemainingTime -= TimeTick;

		Hit.Time = 1.f;
		const FVector OldVelocity = Velocity;
		const FVector MoveDelta = ComputeMoveDelta(OldVelocity, TimeTick);

		const FQuat NewRotation = (bRotationFollowsVelocity && !OldVelocity.IsNearlyZero(0.01f)) ? OldVelocity.ToOrientationQuat() : UpdatedComponent->GetComponentQuat();

		if (isSalfMove)
		{
			if (UWorld* MyWorld = GetWorld())
			{
				TArray<FHitResult> vTraceHits;
				
				MyWorld->LineTraceMultiByObjectType(vTraceHits, UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentLocation() + MoveDelta, mOQParams, mQParams);
				//MyWorld->OverlapMultiByObjectType(Hits, UpdatedComponent->GetComponentLocation() + MoveDelta, FQuat::Identity,Params, FCollisionShape::MakeSphere(0.1f));
				if (vTraceHits.Num()>0)
				{
					FVector vMoveDelta = vTraceHits[0].Location - UpdatedComponent->GetComponentLocation();
					SafeMoveUpdatedComponent(vMoveDelta, NewRotation, true, Hit, isUseTelepMove ? ETeleportType::TeleportPhysics : ETeleportType::None);
					//}
					//SafeMoveUpdatedComponent(UpdatedComponent->GetComponentLocation() + MoveDelta - , NewRotation, true, Hit, isUseTelepMove ? ETeleportType::TeleportPhysics : ETeleportType::None);
					//Velocity = FVector(0, 0, 0);
					return;
				}
			}
		
		}


		if (bShouldBounce)
		{
			SafeMoveUpdatedComponent(MoveDelta, NewRotation, true, Hit, isUseTelepMove ? ETeleportType::TeleportPhysics : ETeleportType::None);
		}
		else
		{
			// If we can't bounce, then we shouldn't adjust if initially penetrating, because that should be a blocking hit that causes a hit event and stop simulation.
			TGuardValue<EMoveComponentFlags> ScopedFlagRestore(MoveComponentFlags, MoveComponentFlags | MOVECOMP_NeverIgnoreBlockingOverlaps);
			MoveUpdatedComponent(MoveDelta, NewRotation, true, &Hit, isUseTelepMove?ETeleportType::TeleportPhysics:ETeleportType::None);
		}

		
		// If we hit a trigger that destroyed us, abort.
		if (ActorOwner->IsPendingKill() || HasStoppedSimulation())
		{
			return;
		}

		// Handle hit result after movement
		if (!Hit.bBlockingHit)
		{
			PreviousHitTime = 1.f;
			bIsSliding = false;

			// Only calculate new velocity if events didn't change it during the movement update.
			if (Velocity == OldVelocity)
			{
				Velocity = ComputeVelocity(Velocity, TimeTick);
			}
		}
		else
		{
			// Only calculate new velocity if events didn't change it during the movement update.
			if (Velocity == OldVelocity)
			{
				// re-calculate end velocity for partial time
				Velocity = (Hit.Time > KINDA_SMALL_NUMBER) ? ComputeVelocity(OldVelocity, TimeTick * Hit.Time) : OldVelocity;
			}

			// Handle blocking hit
			float SubTickTimeRemaining = TimeTick * (1.f - Hit.Time);
			const EHandleBlockingHitResult HandleBlockingResult = HandleBlockingHit(Hit, TimeTick, MoveDelta, SubTickTimeRemaining);
			if (HandleBlockingResult == EHandleBlockingHitResult::Abort || HasStoppedSimulation())
			{
				break;
			}
			else if (HandleBlockingResult == EHandleBlockingHitResult::Deflect)
			{
				NumBounces++;
				HandleDeflection(Hit, OldVelocity, NumBounces, SubTickTimeRemaining);
				PreviousHitTime = Hit.Time;
				PreviousHitNormal = ConstrainNormalToPlane(Hit.Normal);
			}
			else if (HandleBlockingResult == EHandleBlockingHitResult::AdvanceNextSubstep)
			{
				// Reset deflection logic to ignore this hit
				PreviousHitTime = 1.f;
			}
			else
			{
				// Unhandled EHandleBlockingHitResult
				checkNoEntry();
			}


			// A few initial bounces should add more time and iterations to complete most of the simulation.
			if (NumBounces <= 2 && SubTickTimeRemaining >= MIN_TICK_TIME)
			{
				RemainingTime += SubTickTimeRemaining;
				Iterations--;
			}
		}
	}

	UpdateComponentVelocity();
	
}
