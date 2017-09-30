#ifndef _ShaderObject_H_
#define _ShaderObject_H_
#pragma once
#include <boost/noncopyable.hpp>
#include "Engine.h"

#include "rendersystem/include/RenderLayout.hpp"


namespace Air
{
	enum RenderEffectDataType
	{
		REDT_bool = 0,
		REDT_string,
		REDT_texture1D,
		REDT_texture2D,
		REDT_texture3D,
		REDT_textureCUBE,
		REDT_texture1DArray,
		REDT_texture2DArray,
		REDT_texture3DArray,
		REDT_textureCUBEArray,
		REDT_sampler,
		REDT_shader,
		REDT_uint,
		REDT_uint2,
		REDT_uint3,
		REDT_uint4,
		REDT_int,
		REDT_int2,
		REDT_int3,
		REDT_int4,
		REDT_float,
		REDT_float2,
		REDT_float2x2,
		REDT_float2x3,
		REDT_float2x4,
		REDT_float3,
		REDT_float3x2,
		REDT_float3x3,
		REDT_float3x4,
		REDT_float4,
		REDT_float4x2,
		REDT_float4x3,
		REDT_float4x4,
		REDT_buffer,
		REDT_structured_buffer,
		REDT_byte_address_buffer,
		REDT_rw_buffer,
		REDT_rw_structured_buffer,
		REDT_rw_texture1D,
		REDT_rw_texture2D,
		REDT_rw_texture3D,
		REDT_rw_texture1DArray,
		REDT_rw_texture2DArray,
		REDT_rw_byte_address_buffer,
		REDT_append_structured_buffer,
		REDT_consume_structured_buffer
	};



	struct ShaderDesc
	{
		ShaderDesc()
		{

		}
		std::string mProfile;
		std::string mFunctionName;
		uint64_t mMacrosHash;
		uint32_t mTechPassType;
#ifdef AIR_HAS_STRUCT_PACK
#pragma pack(push, 1)
#endif
		struct StreamOutputDecl
		{
			VertexElementUsage mUsage;
			uint8_t mUsageIndex;
			uint8_t mStartComponent;
			uint8_t mComponentCount;
			uint8_t mSlot;
			friend bool operator == (StreamOutputDecl const & lhs, StreamOutputDecl const & rhs)
			{
				return (lhs.mUsage == rhs.mUsage) && (lhs.mUsageIndex == rhs.mUsageIndex) && (lhs.mStartComponent == rhs.mStartComponent) && (lhs.mComponentCount == rhs.mComponentCount) && (lhs.mSlot == rhs.mSlot);
			}
			friend bool operator != (StreamOutputDecl const & lhs, StreamOutputDecl const & rhs)
			{
				return !(lhs == rhs);
			}
		};
#ifdef AIR_HAS_STRUCT_PACK
#pragma pack(pop)
#endif
		std::vector<StreamOutputDecl> mSODecl;
		friend bool operator == (ShaderDesc const & lhs, ShaderDesc const & rhs)
		{
			return (lhs.mProfile == rhs.mProfile) && (lhs.mFunctionName == rhs.mFunctionName) && (lhs.mMacrosHash == rhs.mMacrosHash) && (lhs.mSODecl == rhs.mSODecl);
		}

		friend bool operator != (ShaderDesc const & lhs, ShaderDesc const & rhs)
		{
			return !(lhs == rhs);
		}
	};

	class AIR_CORE_API ShaderObject : boost::noncopyable
	{
	public:
		enum ShaderType
		{
			ST_VertexShader,
			ST_PixelShader,
			ST_GeometryShader,
			ST_ComputeShader,
			ST_HullShader,
			ST_DomainShader,
			ST_NumShaderTypes
		};

	public:
		ShaderObject()
		{

		}
		virtual ~ShaderObject()
		{

		}

		virtual bool attachNativeShader(ShaderType type, RenderEffect const & effect, std::array<uint32_t, ST_NumShaderTypes> const & shader_desc_ids, std::vector<uint8_t> const & native_shader_block) = 0;

		virtual bool streamIn(ResIdentifierPtr const & res, ShaderType type, RenderEffect const & effect, std::array<uint32_t, ST_NumShaderTypes> const & shader_desc_ids) = 0;

		virtual void streamOut(std::ostream & os, ShaderType type)= 0;

		virtual void attachShader(ShaderType type, RenderEffect const & effect, RenderTechnique const & tech, RenderPass const & pass, std::array<uint32_t, ST_NumShaderTypes> const & shader_desc_ids) = 0;

		virtual void attachShader(ShaderType type, RenderEffect const & effect, RenderTechnique const & tech, RenderPass const & pass, ShaderObjectPtr const & shared_so) = 0;

		virtual void linkShaders(RenderEffect const & effect) = 0;

		bool hasDiscard() const
		{
			return mHasDiscard;
		}

		bool isvalidate() const
		{
			return mIsvalidate;
		}
		bool hasTessellation() const
		{
			return mHasTessellation;
		}


		virtual ShaderObjectPtr clone(RenderEffect const & effect) = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;

	protected:
		std::vector<uint8_t> compileToDXBC(ShaderType type, RenderEffect const & effect, RenderTechnique const & tech, RenderPass const & pass, std::vector<std::pair<char const *, char const *>> const & api_special_macros, char const * func_name, char const * shader_profile, uint32_t flags);

		void reflectionDXBC(std::vector<uint8_t> const & code, void** reflector);

		std::vector<uint8_t> stripDXBC(std::vector<uint8_t> const & code, uint32_t strip_flags);
	protected:
		std::array<bool, ST_NumShaderTypes> mIsShaderValidate;
		bool mIsvalidate{ false };
		bool mHasDiscard{ false };
		bool mHasTessellation{ false };
		uint32_t mCSBlockSizeX{ 0 };
		uint32_t mCSBlockSizeY{ 0 };
		uint32_t mCSBlockSizeZ{ 0 };
	};
}

#endif
