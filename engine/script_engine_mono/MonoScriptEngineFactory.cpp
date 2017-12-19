#include "basic/include/Util.h"
#include "script_engine_mono/MonoScriptEngine.hpp"
#include "script_engine_mono/MonoScriptEngineFactory.hpp"
void makeScriptEngineFactory(std::unique_ptr<Air::ScriptEngine>& ptr)
{
	ptr = Air::MakeUniquePtr<Air::MonoScriptEngine>();
}