#include "script_engine_mono/MonoScriptEngine.hpp"
namespace Air
{
	void MonoScriptEngine::initialize()
	{
		char* info = mono_get_runtime_build_info();
		mDomain = mono_jit_init("MonoScriptEngine");
	}
}