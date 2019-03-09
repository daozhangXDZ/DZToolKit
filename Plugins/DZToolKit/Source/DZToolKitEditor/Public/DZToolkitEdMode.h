#pragma once
#include "CoreMinimal.h"
#include "EdMode.h"

class FDZToolKitEdMode : public FEdMode
{
public:
	const static FEditorModeID EM_DZToolKitEdModeID;
public:
	FDZToolKitEdMode();
	virtual ~FDZToolKitEdMode();

	virtual void Enter() override;
	virtual void Exit() override;
	//virtual void Tick(FEditorViewportClient* viewportClinet, float deltaTime) override;
	//virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	//virtual void ActorSelectionChangeNotify() override;
	bool UsesToolkits() const override;
};