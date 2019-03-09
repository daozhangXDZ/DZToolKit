// Copyright 2018 azhecheng, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"
DECLARE_LOG_CATEGORY_EXTERN(DZToolKitRuntime, Log, All);
class FDZToolKitRuntimeModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};