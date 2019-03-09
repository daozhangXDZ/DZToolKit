// Copyright 2018 azhecheng, Inc. All Rights Reserved.
#include "DZToolKitRuntime.h"
#include "DZToolKitRuntimePrivatePCH.h"

#define LOCTEXT_NAMESPACE "FAnimIKModule"

void FDZToolKitRuntimeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FDZToolKitRuntimeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDZToolKitRuntimeModule, DZToolKitRuntime)
DEFINE_LOG_CATEGORY(DZToolKitRuntime);