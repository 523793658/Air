#include "basic/include/Basic.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "basic/include/Log.hpp"
namespace Air
{
	bool GenericPlatFormProcess::supportsMultithreading()
	{
		static bool bSupportsMultithreading = true;
		return bSupportsMultithreading;
	}

	bool GenericPlatFormProcess::isProcRunning(ProcHandle & handle)
	{
		logInfo("FGenericPlatformProcess::IsProcRunning not implemented on this platform");
		return false;
	}

	void GenericPlatFormProcess::waitForProc(ProcHandle & handle)
	{
	}

	bool GenericPlatFormProcess::getProcReturnCode(ProcHandle & handle, int32_t * returnCode)
	{
		return true;
	}

	void GenericPlatFormProcess::sleep(float time)
	{

	}

	void GenericPlatFormProcess::closeProc(ProcHandle& handle)
	{

	}

	void GenericPlatFormProcess::terminateProc(ProcHandle & handle, bool killTree /* = false */)
	{
		
	}
	static std::string Generic_ShaderDir;
	const std::string_view GenericPlatFormProcess::shaderDir()
	{
		if (Generic_ShaderDir.size() == 0)
		{
			Generic_ShaderDir = "assets/shader";
		}
		return Generic_ShaderDir;
	}
	void GenericPlatFormProcess::setShaderDir(const std::string_view path)
	{
		if (path.size() != 0)
		{
			Generic_ShaderDir = std::string(path);
		}
		{
			Generic_ShaderDir = "";
		}
	}
}