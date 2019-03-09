#pragma once
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "DZAlgeoInterSection_FunLib.generated.h"

UCLASS()
class UDZAlgeoInterSection_FunLib : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "DZToolkit|DZAlgeoInterSection")
	static void CalcSceneCom_InterScectionBounds(USceneComponent *pInterComA, USceneComponent * pInterComB, bool CheckTypeInRadius, FBoxSphereBounds & outInterBounds, bool & isInterSection);
};