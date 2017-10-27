#ifndef _RenderStateObject_H_
#define _RenderStateObject_H_
#pragma once
#include "boost/noncopyable.hpp"
#include "boost/limits.hpp"
#include "PreDeclare.h"
namespace Air
{
	enum PolygonMode
	{
		PM_Point,
		PM_Line,
		PM_Fill
	};
	enum ShadeMode
	{
		SM_Flat,
		SM_Gouraud

	};

	enum CullMode
	{
		CM_None,
		CM_Front,
		CM_Back
	};

	enum BlendOperation
	{
		BOP_Add		= 1,
		BOP_Sub		= 2,
		BOP_Rev_Sub	= 3,
		BOP_Min		= 4,
		BOP_Max		= 5
	};

	enum AlphaBlendFactor
	{
		ABF_Zero,
		ABF_One,
		ABF_Src_Alpha,
		ABF_Dst_Alpha,
		ABF_Inv_Src_Alpha,
		ABF_Inv_Dst_Alpha,
		ABF_Src_Color,
		ABF_Dst_Color,
		ABF_Inv_Src_Color,
		ABF_Inv_Dst_Color,
		ABF_Src_Alpha_Sat,
		ABF_Blend_Factor,
		ABF_Inv_Blend_Factor,
		ABF_Src1_Alpha,
		ABF_Inv_Src1_Alpha,
		ABF_Src1_Color,
		ABF_Inv_Src1_Color
	};

	enum CompareFunction
	{
		CF_AlwaysFail,
		CF_AlwaysPass,
		CF_Less,
		CF_LessEqual,
		CF_Equal,
		CF_NotEqual,
		CF_GreaterEqual,
		CF_Greater
	};

	enum StencilOperation
	{
		SOP_Keep,
		SOP_Zero,
		SOP_Replace,
		SOP_Incr,
		SOP_Decr,
		SOP_Invert,
		SOP_Incr_Wrap,
		SOP_Decr_Wrap
	};

	enum ColorMask
	{
		CMASK_Red		= 1UL << 0,
		CMASK_Green		= 1UL << 1,
		CMASK_Blue		= 1UL << 2,
		CMASK_Alpha		= 1UL << 3,
		CMASK_ALL		= CMASK_Red | CMASK_Green | CMASK_Blue | CMASK_Alpha
	};

	enum TexAddressingMode
	{
		TAM_Wrap,
		TAM_Mirror,
		TAM_Clamp,
		TAM_Border
	};
	
	enum TexFilterOp
	{
		TFOE_Mip_Point = 0x0,
		TFOE_MiP_Linear = 0x1,
		TFOE_Mag_Point = 0x0,
		TFOE_Mag_Linear = 0x2,
		TFOE_Min_Point = 0x0,
		TFOE_Min_Linear = 0x4,
		TFOE_Anisotropic = 0x8,
		TFOE_Comparison = 0x10,

		TFO_Min_Mag_Mip_Point				= TFOE_Min_Point | TFOE_Mag_Point | TFOE_Mip_Point,
		TFO_Min_Mag_Point_Mip_Linear		= TFOE_Min_Point | TFOE_Mag_Point | TFOE_MiP_Linear,
		TFO_Min_Point_Mag_Linear_Mip_Point	= TFOE_Min_Point | TFOE_Mag_Linear | TFOE_Mip_Point,
		TFO_Min_Linear_Mag_Mip_Point		= TFOE_Min_Linear | TFOE_Mag_Point | TFOE_Mip_Point,
		TFO_Min_Mag_Linear_Mip_Point		= TFOE_Min_Linear | TFOE_Mag_Linear | TFOE_Mip_Point,
		TFO_Min_Linear_Mag_Point_Mip_Linear = TFOE_Min_Linear | TFOE_Mag_Point | TFOE_MiP_Linear,
		TFO_Min_Point_Mag_Mip_Linear		= TFOE_Min_Point | TFOE_Mag_Linear | TFOE_MiP_Linear,
		TFO_Min_Mag_Mip_Linear				= TFOE_Min_Linear | TFOE_Mag_Linear | TFOE_MiP_Linear,

		TFO_Anisotropic						= TFOE_Anisotropic,

		TFO_Cmp_Min_Mag_Mip_Point = TFOE_Comparison | TFO_Min_Mag_Mip_Point,
		TFO_Cmp_Min_Mag_Point_Mip_Linear = TFOE_Comparison | TFO_Min_Mag_Point_Mip_Linear,
		TFO_Cmp_Min_Point_Mag_Linear_Mip_Point = TFOE_Comparison | TFO_Min_Point_Mag_Linear_Mip_Point,
		TFO_Cmp_Min_Point_Mag_Mip_Linear = TFOE_Comparison | TFO_Min_Point_Mag_Mip_Linear,
		TFO_Cmp_Min_Linear_Mag_Mip_Point = TFOE_Comparison | TFO_Min_Linear_Mag_Mip_Point,
		TFO_Cmp_Min_Linear_Mag_Point_Mip_Linear = TFOE_Comparison | TFO_Min_Linear_Mag_Point_Mip_Linear,
		TFO_Cmp_Min_Mag_Linear_Mip_Point = TFOE_Comparison | TFO_Min_Mag_Linear_Mip_Point,
		TFO_Cmp_Min_Mag_Mip_Linear = TFOE_Comparison | TFO_Min_Mag_Mip_Linear,
		TFO_Cmp_Anisotropic = TFOE_Comparison | TFO_Anisotropic
	};

	enum LogicOperation
	{
		LOP_Clear,
		LOP_Set,
		LOP_Copy,
		LOP_CopyInverted,
		LOP_Noop,
		LOP_Invert,
		LOP_And,
		LOP_NAnd,
		LOP_Or,
		LOP_NOR,
		LOP_XOR,
		LOP_Equiv,
		LOP_AndReverse,
		LOP_AndInverted,
		LOP_OrReverse,
		LOP_OrInverted
	};

#ifdef AIR_HAS_STRUCT_PACK
#pragma pack(push, 1)
#endif
	struct AIR_CORE_API RasterizerStateDesc
	{
		PolygonMode mPolygonMode{ PM_Fill };
		ShadeMode mShadeMode{ SM_Gouraud };
		CullMode	mCullMode{ CM_Back };
		bool		mFrontFaceCCW{ false };
		float		mPolygonOffsetFactor{ 0.0f };
		float		mPolygonOffsetUnits{ 0.0f };
		bool		mDepthClipEnable{ true };
		bool		mScissorEnable{ false };
		bool		mMultiSampleEnabel{ true };
		RasterizerStateDesc() 
		{
		};
		friend bool operator<(RasterizerStateDesc const & lhs, RasterizerStateDesc const & rhs);
	};

	struct AIR_CORE_API DepthStencilStateDesc
	{
		bool			mDepthEnable{ true };
		bool			mDepthWriteEnable{ true };
		CompareFunction	mDepthFunc{ CF_Less };
		bool			mFrontStencilEnable{ false };
		CompareFunction mFrontStencilFunc{ CF_AlwaysPass };
		uint16_t		mFrontStencilRef{ 0 };
		uint16_t		mFrontstenCilReadMask{ 0xFFFF };
		uint16_t		mFrontStencilWriteMask{ 0xFFFF };
		StencilOperation mFrontStencilFail{ SOP_Keep };
		StencilOperation mFrontStencilDepthFail{ SOP_Keep };
		StencilOperation mFrontStencilPass{ SOP_Keep };
		bool			mBackStencilEnable{ false };
		CompareFunction	mBackStencilFunc{ CF_AlwaysPass };
		uint16_t		mBackStencilRef{ 0 };
		uint16_t		mBackStencilReadMask{ 0xFFFF };
		uint16_t		mBackStencilWriteMask{ 0xFFFF };
		StencilOperation mBackStencilFail{ SOP_Keep };
		StencilOperation mBackStencilDepthFail{ SOP_Keep };
		StencilOperation mBackStencilPass{ SOP_Keep };
		DepthStencilStateDesc()
		{

		}
		friend bool operator<(DepthStencilStateDesc const & lhs, DepthStencilStateDesc const & rhs);
	};

	struct AIR_CORE_API BlendStateDesc
	{
		Color						mBlendFactor{ 1, 1, 1, 1, };
		uint32_t					mSampleMask{ 0xFFFFFFFF };
		bool						mAlphaToCoverageEnable{ false };
		bool						mIndependentBlendEnable{ false };
		std::array<bool, 8>			mBlendEnable;
		std::array<bool, 8>			mLogicOpEnable;
		std::array<BlendOperation, 8>	mBlendOp;
		std::array<AlphaBlendFactor, 8>	mSrcBlend;
		std::array<AlphaBlendFactor, 8> mDstBlend;
		std::array<BlendOperation, 8>	mBlendOpAlpha;
		std::array<AlphaBlendFactor, 8> mSrcBlendAlpha;
		std::array<AlphaBlendFactor, 8> mDstBlendAlpha;
		std::array<LogicOperation, 8>	mLogicOp;
		std::array<uint8_t, 8>			mColorWriteMask;
		BlendStateDesc();
		friend bool operator<(BlendStateDesc const & lhs, BlendStateDesc const & rhs);
	};

	struct AIR_CORE_API SamplerStateDesc
	{
		Color				mBorderColor{ 0, 0, 0, 0 };
		TexAddressingMode	mAddrModeU{ TAM_Wrap };
		TexAddressingMode	mAddrModeV{ TAM_Wrap };
		TexAddressingMode	mAddrModeW{ TAM_Wrap };
		TexFilterOp			mFilter{ TFO_Min_Mag_Mip_Point };
		uint8_t				mMaxAnisotropy{ 16 };
		float				mMinLod{ 0 };
		float				mMaxLod;
		float				mMiPMapLoadBias{ 0 };
		CompareFunction		mCmpFunc{ CF_AlwaysFail };
		SamplerStateDesc();
		friend bool operator<(SamplerStateDesc const & lhs, SamplerStateDesc const &rhs);
	};

#ifdef AIR_HAS_STRUCT_PACK
#pragma pack(pop)
#endif


	class AIR_CORE_API RenderStateObject : boost::noncopyable
	{
	public:
		explicit RenderStateObject(RasterizerStateDesc const & rs_desc,
			DepthStencilStateDesc const & dss_desc, BlendStateDesc const & bs_sesc);
		virtual ~RenderStateObject();
		RasterizerStateDesc const & getRasterizerStateDesc() const;
		DepthStencilStateDesc const & getDepthStencilStateDesc() const;
		BlendStateDesc const & getBlendStateDesc() const;
		virtual void active() = 0;
	protected:
		RasterizerStateDesc		mRSDesc;
		DepthStencilStateDesc	mDSSDesc;
		BlendStateDesc			mBSDesc;
	};

	class AIR_CORE_API SamplerStateObject : boost::noncopyable
	{
	public:
		explicit SamplerStateObject(SamplerStateDesc const & ss_desc);
		virtual ~SamplerStateObject();
		SamplerStateDesc const & getDesc() const;
	protected:
		SamplerStateDesc mDesc;
	};
}

#endif
