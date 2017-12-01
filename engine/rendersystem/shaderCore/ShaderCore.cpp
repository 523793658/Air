#include "Context.h"
#include "rendersystem/shaderCore/ShaderCore.hpp"

namespace Air
{
	void ShaderCompilerOutput::generateOutputHash()
	{
		size_t hash = 0;
		const std::vector<uint8_t>& code = mShaderCode.getReadAccess();
		uint32_t shaderCodeSize = mShaderCode.getShaderCodeSize();
		hash = boost::hash_range(code.begin(), code.end());
		mParameterMap.updateHash(hash);
		mHash = hash;
	}

}