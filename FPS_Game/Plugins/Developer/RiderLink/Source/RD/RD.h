#pragma once

#include "Logging/LogMacros.h"
#include "Logging/LogVerbosity.h"
#include "Modules/ModuleInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(FLogRDModule, Log, All);

class FRDModule : public IModuleInterface
{
public:
	FRDModule() = default;
	~FRDModule() = default;
};
