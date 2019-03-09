#include "DZToolKitEditor.h"
#include "DZToolKitEdMode.h"

#define LOCTEXT_NAMESPACE "FDZToolkitEditorModule"
void FDZToolKitEditorModule::StartupModule()
{
	FEditorModeRegistry::Get().RegisterMode<FDZToolKitEdMode>(
		FDZToolKitEdMode::EM_DZToolKitEdModeID,
		LOCTEXT("DZToolkitEdModeName", "DZToolkitEdMode"),
		FSlateIcon(),
		true
		);
}

void FDZToolKitEditorModule::ShutdownModule()
{
	FEditorModeRegistry::Get().UnregisterMode(FDZToolKitEdMode::EM_DZToolKitEdModeID);
}
#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FDZToolKitEditorModule, DZToolKitEditor)