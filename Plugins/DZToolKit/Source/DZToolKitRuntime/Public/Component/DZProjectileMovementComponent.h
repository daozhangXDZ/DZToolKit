// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameFrameWork/ProjectileMovementComponent.h"
#include "Engine/EngineTypes.h"
#include "CollisionQueryParams.h"
#include "DZProjectileMovementComponent.generated.h"

/**
 * Edit Tick Do  ProCheck Collision beform ProjectMove
 */
UCLASS(ClassGroup=Movement, meta=(BlueprintSpawnableComponent), ShowCategories=(Velocity))
class DZTOOLKITRUNTIME_API UDZProjectileMovementComponent : public UProjectileMovementComponent
{
	GENERATED_UCLASS_BODY()

	/** The magnitude of our acceleration towards the homing target. Overall velocity magnitude will still be limited by MaxSpeed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DZMove)
	bool isUseTelepMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DZMove)
	bool isSalfMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DZMove)
	int ExtendChackStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DZMove)
	float ExtendChackTimeScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DZMove)
	TArray<TEnumAsByte<ECollisionChannel>> CheckCollisionChannel;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	FCollisionObjectQueryParams mOQParams;
	FCollisionQueryParams mQParams;
};



