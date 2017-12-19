#ifndef _MonoScriptEngineFactory_H_
#define _MonoScriptEngineFactory_H_
#pragma once
#include "Context.h"

#ifdef AIR_SCRIPT_ENGINE_MONO_SOURCE
#define MONO_CORE_API AIR_SYMBOL_EXPORT
#else
#define MONO_CORE_API AIR_SYMBOL_IMPORT
#endif
#include "script_engine_mono/MonoScriptEngine.hpp"
extern "C"
{
	MONO_CORE_API void makeScriptEngineFactory(std::unique_ptr<Air::ScriptEngine>& ptr);
}

#endif
