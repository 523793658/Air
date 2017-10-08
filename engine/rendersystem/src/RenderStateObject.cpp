#include "Engine.h"
#include "rendersystem/include/RenderStateObject.hpp"
namespace Air
{
	bool operator<(RasterizerStateDesc const & lhs, RasterizerStateDesc const & rhs)
	{
		return std::memcmp(&lhs, &rhs, sizeof(lhs)) < 0;
	}

	bool operator<(DepthStencilStateDesc const & lhs, DepthStencilStateDesc const & rhs)
	{
		return std::memcmp(&lhs, &rhs, sizeof(lhs)) < 0;
	}

	bool operator < (BlendStateDesc const & lhs, BlendStateDesc const & rhs)
	{
		return std::memcmp(&lhs, &rhs, sizeof(lhs)) < 0;
	}

	BlendStateDesc::BlendStateDesc()
	{
		mBlendEnable.fill(false);
		mLogicOpEnable.fill(false);
		mBlendOp.fill(BOP_Add);
		mSrcBlend.fill(ABF_One);
		mDstBlend.fill(ABF_Zero);
		mBlendOpAlpha.fill(BOP_Add);
		mSrcBlendAlpha.fill(ABF_One);
		mDstBlendAlpha.fill(ABF_Zero);
		mColorWriteMask.fill(CMASK_ALL);
		mLogicOp.fill(LOP_Noop);
	}


	SamplerStateDesc::SamplerStateDesc()
		:mMaxLod(std::numeric_limits<float>::max())
	{

	}

	SamplerStateObject::SamplerStateObject(SamplerStateDesc const & ss_desc)
	{
		mDesc = ss_desc;
	}
	SamplerStateObject::~SamplerStateObject()
	{

	}
	SamplerStateDesc const & SamplerStateObject::getDesc() const
	{
		return mDesc;
	}

	bool operator < (SamplerStateDesc const & lhs, SamplerStateDesc const & rhs)
	{
		return std::memcmp(&lhs, &rhs, sizeof(lhs));
	}

	RenderStateObject::RenderStateObject(RasterizerStateDesc const & rs_desc,
		DepthStencilStateDesc const & dss_desc, BlendStateDesc const & bs_sesc) : mRSDesc(rs_desc), mDSSDesc(dss_desc), mBSDesc(bs_sesc)
	{

	}
	RenderStateObject::~RenderStateObject()
	{

	}
	RasterizerStateDesc const & RenderStateObject::getRasterizerStateDesc() const
	{
		return mRSDesc;
	}
	DepthStencilStateDesc const & RenderStateObject::getDepthStencilStateDesc() const
	{
		return mDSSDesc;
	}
	BlendStateDesc const & RenderStateObject::getBlendStateDesc() const
	{
		return mBSDesc;
	}
}