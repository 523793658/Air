#ifndef _Air_Render_Device_Caps_H_
#define _Air_Render_Device_Caps_H_
#include <functional>
#include "PreDeclare.h"
#include "ElementFormat.h"
#include <boost/operators.hpp>
namespace Air
{
	enum TessellationMethod
	{
		TM_Hardware,
		TM_No
	};

	struct ShaderModel : boost::less_than_comparable<ShaderModel, boost::equality_comparable<ShaderModel>>
	{
		uint8_t major_ver : 6;
		uint8_t minor_ver : 2;
		ShaderModel()
			:major_ver(0), minor_ver(0)
		{

		}
		ShaderModel(uint8_t major, uint8_t minor)
			:major_ver(major), minor_ver(minor)
		{

		}

		uint32_t getFullVersion() const
		{
			return (major_ver << 2) | minor_ver;
		}

		bool operator < (ShaderModel const & rhs) const
		{
			return this->getFullVersion() < rhs.getFullVersion();
		}

		bool operator == (ShaderModel const & rhs) const
		{
			return this->getFullVersion() == rhs.getFullVersion();
		}

	};

struct RenderDeviceCaps
{
	ShaderModel mMaxShaderModel;
	uint32_t mMaxTextureWidth;
	uint32_t mMaxTextureHeight;
	uint32_t mMaxTextureDepth;
	uint32_t mMaxTextureCubeSize;
	uint32_t mMaxTextureArrayLength;
	uint8_t mMaxVertexTextureUnits;
	uint8_t mMaxPixelTextureUnits;
	uint8_t mMaxGeometryTextureUntis;
	uint8_t mMaxSimultaneousRts;
	uint8_t mMaxSimultaneousUavs;
	uint8_t mMaxVertexStreams;
	uint8_t mMaxTextureAnisotropy;

	bool mIstbdr : 1;
	bool mHWInstancingSupport : 1;
	bool mInstanceIdSupport : 1;
	bool mStreamOuputSupport : 1;
	bool mAlphaToCoverageSupport : 1;
	bool mPrimitiveRestartSupport : 1;
	bool mMultithreadRenderingSupport : 1;
	bool mMultithreadResCreatingSupport : 1;
	bool mMRTIndependentBitDepthSupport : 1;
	bool mStandarDerivativesSupport : 1;
	bool mShaderTextureLodSupport : 1;
	bool mLogicOpSupport : 1;
	bool mIndependentBlendSupport : 1;
	bool mDepthTextureSupport;
	bool mFPColorSupport : 1;
	bool mPackToRGBARequired : 1;
	bool mDrawIndirectSupport : 1;
	bool mNoOverwriteSupport : 1;
	bool mFullNoptTextureSupport : 1;
	bool mRenderToTextureArraySupport : 1;
	bool mLoadFromBufferSupport : 1;

	bool mGSSupport : 1;
	bool mCSSupport : 1;
	bool mHSSupport : 1;
	bool mDSSupport : 1;
	TessellationMethod mTessMethod;

	std::function<bool(ElementFormat)> vertexFormatSupport;
	std::function<bool(ElementFormat)> textureFormatSupport;
	std::function<bool(ElementFormat, uint32_t, uint32_t)> rendertargetFormatSupport;
};










}
#endif