#ifndef _MonoScriptEngine_H_
#define _MonoScriptEngine_H_
#pragma once
#include "script_engine/include/ScriptEngine.hpp"
#include "mono/jit/jit.h"

namespace Air
{
	class MonoScriptEngine : public ScriptEngine
	{
	public:
		virtual void initialize();

	private:
		MonoDomain *mDomain;
	};
}
#endif
