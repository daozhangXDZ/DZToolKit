#include "DZAlgeoInterSection_FunLib.h"

UDZAlgeoInterSection_FunLib::UDZAlgeoInterSection_FunLib(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void UDZAlgeoInterSection_FunLib::CalcSceneCom_InterScectionBounds(USceneComponent *pInterComA, USceneComponent * pInterComB, bool CheckTypeInRadius, FBoxSphereBounds & outInterBounds, bool & isInterSection)
{
	FTransform vAworldTransform = pInterComA->GetComponentTransform();
	FTransform vBworldTransform = pInterComB->GetComponentTransform();
	FBoxSphereBounds vABounds_World = pInterComA->CalcBounds(vAworldTransform);
	FBoxSphereBounds vBBounds_World = pInterComB->CalcBounds(vBworldTransform);
	isInterSection = false;
	if ( !CheckTypeInRadius)
	{
		FBox vABoxWorld = vABounds_World.GetBox();
		FBox VBBoxWorld = vBBounds_World.GetBox();
		if (vABoxWorld.Intersect(VBBoxWorld))
		{
			isInterSection = true;
			//	// otherwise they overlap
			//// so find overlapping box
			//	FVector MinVector, MaxVector;

			//	MinVector.X = FMath::Max(Min.X, Other.Min.X);
			//	MaxVector.X = FMath::Min(Max.X, Other.Max.X);

			//	MinVector.Y = FMath::Max(Min.Y, Other.Min.Y);
			//	MaxVector.Y = FMath::Min(Max.Y, Other.Max.Y);

			//	MinVector.Z = FMath::Max(Min.Z, Other.Min.Z);
			//	MaxVector.Z = FMath::Min(Max.Z, Other.Max.Z);

			//	return FBox(MinVector, MaxVector);
			outInterBounds = vABounds_World.GetBox().Overlap(vBBounds_World.GetBox());
		}
	
	}
	else
	{
		bool isInSphere = (vABounds_World.Origin - vBBounds_World.Origin).SizeSquared() 
			<= FMath::Square(FMath::Max(0.f, vABounds_World.SphereRadius + vBBounds_World.SphereRadius + KINDA_SMALL_NUMBER));
		if( FBoxSphereBounds::SpheresIntersect(vABounds_World, vBBounds_World) )
		{
			isInterSection = true;
			FVector vOriginDir = (vBBounds_World.Origin - vABounds_World.Origin).GetSafeNormal();
			FVector vOutPoint;
			FMath::SphereDistToLine(vBBounds_World.Origin, vBBounds_World.SphereRadius, vABounds_World.Origin, vOriginDir, vOutPoint);
			outInterBounds.Origin = vOutPoint;
			////TODO: outInterBounds.SphereRadius = ;
			////TODO: Use this Algeothm
			////http://mathworld.wolfram.com/Sphere-SphereIntersection.html  
			////outInterBounds = vABounds_World.GetSphere(). 
		}
	}
}