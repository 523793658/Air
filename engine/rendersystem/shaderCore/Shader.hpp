#ifndef _Shader_H_
#define _Shader_H_
#pragma once
namespace Air
{

class ShaderPipelineType
{
public:
	enum class EShaderTypeForDynamicCast : uint32_t
	{
		STDC_Global,
		STDC_Material,
		STDC_MeshMaterial
	};

	FORCEINLINE wchar_t const * getName() const
	{
		return mName;
	}
	
private:
	const wchar_t* mName;
	std::string mTypeName;
	const wchar_t* mSourceFilename;
	const wchar_t* mFunctionName;
	uint32_t mFrequency;
	uint32_t mHashIndex;
	EShaderTypeForDynamicCast mShaderTypeForDynamicCast;

};


class AIR_CORE_API FShaderType
{
public:

};

}

#endif
