#include "DZToolKitEdMode.h"
#include "DZToolKitEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FDZToolKitEdMode::EM_DZToolKitEdModeID = TEXT("EM_DZToolkitEdMode");
FDZToolKitEdMode::FDZToolKitEdMode()
	:FEdMode()
{

}

FDZToolKitEdMode::~FDZToolKitEdMode()
{
}

void FDZToolKitEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FDZToolKitEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FDZToolKitEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FDZToolKitEdMode::UsesToolkits() const
{
	return true;
}