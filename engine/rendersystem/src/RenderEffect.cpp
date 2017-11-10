#include <boost/functional/hash.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>

#include "Context.h"
#include "SingletonManager.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "basic/include/ErrorHanding.hpp"
#include "basic/include/ResIdentifier.h"
#include "basic/include/Log.hpp"
#include "core/include/ResLoader.h"
#include "basic/include/XMLDom.hpp"
#include "rendersystem/include/RenderStateObject.hpp"
#include "rendersystem/include/RenderEffect.hpp"
#include <map>


namespace
{
	using namespace Air;
	uint32_t const AIR_VERSION = 0x0110;
	std::mutex singleton_mutex;

	class type_define {
	public:
		static type_define& getInstance()
		{
			if (!mInstance)
			{
				std::lock_guard<std::mutex> lock(singleton_mutex);
				if (!mInstance)
				{
					mInstance = MakeUniquePtr<type_define>();
				}
			}
			return *mInstance;
		}
		uint32_t getTypeCode(std::string_view name) const
		{
			size_t const name_hash = boost::hash_range(name.begin(), name.end());
			for (uint32_t i = 0; i < mTypesHash.size(); ++i)
			{
				if (mTypesHash[i] == name_hash)
				{
					return i;
				}
			}
			AIR_UNREACHABLE("Invalid type name");
		}

		std::string const & getTypeName(uint32_t code) const
		{
			if (code < mTypes.size())
			{
				return mTypes[code];
			}
			AIR_UNREACHABLE("Invalid type code");
		}

		type_define()
		{
			mTypes.push_back("bool");
			mTypes.push_back("string");
			mTypes.push_back("texture1D");
			mTypes.push_back("texture2D");
			mTypes.push_back("texture3D");
			mTypes.push_back("textureCUBE");
			mTypes.push_back("texture1DArray");
			mTypes.push_back("texture2DArray");
			mTypes.push_back("texture3DArray");
			mTypes.push_back("textureCUBEArray");
			mTypes.push_back("sampler");
			mTypes.push_back("shader");
			mTypes.push_back("uint");
			mTypes.push_back("uint2");
			mTypes.push_back("uint3");
			mTypes.push_back("uint4");
			mTypes.push_back("int");
			mTypes.push_back("int2");
			mTypes.push_back("int3");
			mTypes.push_back("int4");
			mTypes.push_back("float");
			mTypes.push_back("float2");
			mTypes.push_back("float2x2");
			mTypes.push_back("float2x3");
			mTypes.push_back("float2x4");
			mTypes.push_back("float3");
			mTypes.push_back("float3x2");
			mTypes.push_back("float3x3");
			mTypes.push_back("float3x4");
			mTypes.push_back("float4");
			mTypes.push_back("float4x2");
			mTypes.push_back("float4x3");
			mTypes.push_back("float4x4");
			mTypes.push_back("buffer");
			mTypes.push_back("structured_buffer");
			mTypes.push_back("byte_address_buffer");
			mTypes.push_back("rw_buffer");
			mTypes.push_back("rw_structured_buffer");
			mTypes.push_back("rw_texture1D");
			mTypes.push_back("rw_texture2D");
			mTypes.push_back("rw_texture3D");
			mTypes.push_back("rw_texture1DArray");
			mTypes.push_back("rw_texture2DArray");
			mTypes.push_back("rw_byte_address_buffer");
			mTypes.push_back("append_structured_buffer");
			mTypes.push_back("consume_structured_buffer");

			mTypesHash.resize(mTypes.size());
			for (size_t i = 0; i < mTypes.size(); ++i)
			{
				mTypesHash[i] = RT_HASH(mTypes[i].c_str());
			}
		}

	private:
		std::vector<std::string> mTypes;
		std::vector<size_t> mTypesHash;
		static std::unique_ptr<type_define> mInstance;
	};
	std::unique_ptr<type_define> type_define::mInstance;

	class constant_buffer_type_define
	{
	public:
		static constant_buffer_type_define& getInstance()
		{
			if (!mInstance)
			{
				std::lock_guard<std::mutex> lock(singleton_mutex);
				if (!mInstance)
				{
					mInstance = MakeUniquePtr<constant_buffer_type_define>();
				}
			}
			return *mInstance;
		}

		ConstantBufferType fromStr(std::string_view name) const
		{

			size_t name_hash = boost::hash_range(name.begin(), name.end());
			for (uint32_t i = 0; i < mCBTSHash.size(); ++i)
			{
				if (name_hash == mCBTSHash[i])
				{
					return static_cast<ConstantBufferType>(i);
				}
			}
			AIR_UNREACHABLE("invalid constbuffer type.");
		}

		constant_buffer_type_define()
		{
			mCBTSHash.push_back(CT_HASH("CBT_Object"));
			mCBTSHash.push_back(CT_HASH("CBT_Frame"));
			mCBTSHash.push_back(CT_HASH("CBT_Global"));
		}
	private:
		std::vector<size_t> mCBTSHash;
		static std::unique_ptr<constant_buffer_type_define> mInstance;
	private:

	};
	std::unique_ptr<constant_buffer_type_define> constant_buffer_type_define::mInstance;

	class shade_mode_define
	{
	public:
		static shade_mode_define& getInstance()
		{
			if (!mInstance)
			{
				std::lock_guard<std::mutex> lock(singleton_mutex);
				if (!mInstance)
				{
					mInstance = MakeUniquePtr<shade_mode_define>();
				}
			}
			return*mInstance;
		}
		ShadeMode fromStr(std::string_view name) const
		{
			size_t const name_hash = boost::hash_range(name.begin(), name.end());
			for (uint32_t i = 0; i < mSMSHash.size(); ++i)
			{
				if (mSMSHash[i] == name_hash)
				{
					return static_cast<ShadeMode>(i);
				}
			}
			AIR_UNREACHABLE("Invalid ShadeMode name");
		}
		shade_mode_define()
		{
			mSMSHash.push_back(CT_HASH("flat"));
			mSMSHash.push_back(CT_HASH("gouraud"));
		}

	private:
		std::vector<size_t> mSMSHash;
		static std::unique_ptr<shade_mode_define> mInstance;
	};
	std::unique_ptr<shade_mode_define> shade_mode_define::mInstance;

	class compare_function_define
	{
	public:
		static compare_function_define& getInstance()
		{
			if (!mInstance)
			{
				std::lock_guard<std::mutex> lock(singleton_mutex);
				if (!mInstance)
				{
					mInstance = MakeUniquePtr<compare_function_define>();
				}
			}
			return *mInstance;
		}

		compare_function_define()
		{
			mCFSHash.push_back(CT_HASH("always_fail"));
			mCFSHash.push_back(CT_HASH("always_pass"));
			mCFSHash.push_back(CT_HASH("less"));
			mCFSHash.push_back(CT_HASH("less_equal"));
			mCFSHash.push_back(CT_HASH("equal"));
			mCFSHash.push_back(CT_HASH("not_equal"));
			mCFSHash.push_back(CT_HASH("greater_equal"));
			mCFSHash.push_back(CT_HASH("greater"));
		}
		CompareFunction fromStr(std::string_view name)
		{
			size_t const name_hash = boost::hash_range(name.begin(), name.end());
			for (size_t i = 0; i < mCFSHash.size(); i++)
			{
				if (mCFSHash[i] == name_hash)
				{
					return static_cast<CompareFunction>(i);
				}
			}
			AIR_UNREACHABLE("Invalid CompareFuntion name");
		}

	private:
		static std::unique_ptr<compare_function_define> mInstance;
		std::vector<size_t> mCFSHash;
	};
	std::unique_ptr<compare_function_define> compare_function_define::mInstance;

	class texture_filter_mode_define
	{
	public:
		static texture_filter_mode_define& getInstance()
		{
			if (!mInstance)
			{
				std::lock_guard<std::mutex> lock(singleton_mutex);
				if (!mInstance)
				{
					mInstance = MakeUniquePtr<texture_filter_mode_define>();
				}
			}
			return *mInstance;
		}
		TexFilterOp  fromStr(std::string name) const
		{
			int cmp;
			std::string_view f;
			if (0 == name.find("cmp_"))
			{
				cmp = 1;
				f = name.substr(4);
			}
			else
			{
				cmp = 0;
				f = name;
			}
			size_t const f_hash = boost::hash_range(f.begin(), f.end());
			for (uint32_t i = 0; i < mTFMHash.size(); ++i)
			{
				if (mTFMHash[i] == f_hash)
				{
					return static_cast<TexFilterOp>((cmp << 4) + i);
				}
			}
			if (CT_HASH("anisotropic") == f_hash)
			{
				return static_cast<TexFilterOp>((cmp << 4) + TFO_Anisotropic);
			}
			AIR_UNREACHABLE("Invalid TexFilterOp name");
		}

		texture_filter_mode_define()
		{
			mTFMHash.push_back(CT_HASH("min_mag_mip_point"));
			mTFMHash.push_back(CT_HASH("min_mag_point_mip_linear"));
			mTFMHash.push_back(CT_HASH("min_point_mag_linear_mip_point"));
			mTFMHash.push_back(CT_HASH("min_point_mag_mip_linear"));
			mTFMHash.push_back(CT_HASH("min_linear_mag_mip_point"));
			mTFMHash.push_back(CT_HASH("min_linear_mag_point_mip_linear"));
			mTFMHash.push_back(CT_HASH("min_mag_linear_mip_point"));
			mTFMHash.push_back(CT_HASH("min_mag_mip_linear"));
		}
	private:
		static std::unique_ptr<texture_filter_mode_define> mInstance;
		std::vector<size_t> mTFMHash;
	};
	std::unique_ptr<texture_filter_mode_define> texture_filter_mode_define::mInstance;

	class cull_mode_define
	{
	public:
		static cull_mode_define& getInstance()
		{
			if (!mInstance)
			{
				std::lock_guard<std::mutex> lock(singleton_mutex);
				if (!mInstance)
				{
					mInstance = MakeUniquePtr<cull_mode_define>();
				}
			}
			return *mInstance;
		}
		CullMode fromStr(std::string name) const
		{
			size_t const name_hash = boost::hash_range(name.begin(), name.end());
			for (size_t i = 0; i < mCMSHash.size(); ++i)
			{
				if (mCMSHash[i] == name_hash)
				{
					return static_cast<CullMode>(i);
				}
			}
			AIR_UNREACHABLE("Invalid CullMode name");
		}
		cull_mode_define()
		{
			mCMSHash.push_back(CT_HASH("none"));
			mCMSHash.push_back(CT_HASH("front"));
			mCMSHash.push_back(CT_HASH("back"));

		}
	private:
		static std::unique_ptr<cull_mode_define> mInstance;
		std::vector<size_t> mCMSHash;
	};
	std::unique_ptr<cull_mode_define> cull_mode_define::mInstance;

	class polygon_mode_define
	{
	public:
		static polygon_mode_define& getInstance()
		{
			if (!mInstance)
			{
				std::lock_guard<std::mutex> lock(singleton_mutex);
				if (!mInstance)
				{
					mInstance = MakeUniquePtr<polygon_mode_define>();
				}
			}
			return *mInstance;
		}
		PolygonMode fromStr(std::string_view name) const
		{
			size_t const name_hash = boost::hash_range(name.begin(), name.end());
			for (size_t i = 0; i < mPMSHash.size(); ++i)
			{
				if (mPMSHash[i] == name_hash)
				{
					return static_cast<PolygonMode>(i);
				}
			}
			AIR_UNREACHABLE("Invalid PolygonMode name!");
		}

		polygon_mode_define()
		{
			mPMSHash.push_back(CT_HASH("point"));
			mPMSHash.push_back(CT_HASH("line"));
			mPMSHash.push_back(CT_HASH("fill"));
		}
	private:
		static std::unique_ptr<polygon_mode_define> mInstance;
		std::vector<size_t> mPMSHash;
	};
	std::unique_ptr<polygon_mode_define> polygon_mode_define::mInstance;

	class alpha_blend_factor_define
	{
	public:
		static alpha_blend_factor_define& getInstance()
		{
			if (!mInstance)
			{
				std::lock_guard<std::mutex> lock(singleton_mutex);
				if (!mInstance)
				{
					mInstance = MakeUniquePtr<alpha_blend_factor_define>();
				}
			}
			return *mInstance;
		}
		AlphaBlendFactor fromStr(std::string_view name) const
		{
			size_t name_hash = boost::hash_range(name.begin(), name.end());
			for (uint32_t i = 0; i < mABFSHash.size(); ++i)
			{
				if (mABFSHash[i] == name_hash)
				{
					return static_cast<AlphaBlendFactor>(i);
				}
			}
			AIR_UNREACHABLE("Invalid AlphaBlendFactor name");
		}
		alpha_blend_factor_define()
		{
			mABFSHash.push_back(CT_HASH("zero"));
			mABFSHash.push_back(CT_HASH("one"));
			mABFSHash.push_back(CT_HASH("src_alpha"));
			mABFSHash.push_back(CT_HASH("dst_alpha"));
			mABFSHash.push_back(CT_HASH("inv_src_alpha"));
			mABFSHash.push_back(CT_HASH("inv_dst_alpha"));
			mABFSHash.push_back(CT_HASH("src_color"));
			mABFSHash.push_back(CT_HASH("dst_color"));
			mABFSHash.push_back(CT_HASH("inv_src_color"));
			mABFSHash.push_back(CT_HASH("inv_dst_color"));
			mABFSHash.push_back(CT_HASH("src_alpha_sat"));
			mABFSHash.push_back(CT_HASH("blend_factor"));
			mABFSHash.push_back(CT_HASH("inv_blend_factor"));
			mABFSHash.push_back(CT_HASH("src1_alpha"));
			mABFSHash.push_back(CT_HASH("inv_src1_alpha"));
			mABFSHash.push_back(CT_HASH("src1_color"));
			mABFSHash.push_back(CT_HASH("inv_src1_color"));
		}
	private:
		static std::unique_ptr<alpha_blend_factor_define> mInstance;
		std::vector<size_t> mABFSHash;
	};
	std::unique_ptr<alpha_blend_factor_define> alpha_blend_factor_define::mInstance;

	class blend_operation_define
	{
	public:
		static blend_operation_define& instance()
		{
			if (!mInstance)
			{
				std::lock_guard<std::mutex> lock(singleton_mutex);
				if (!mInstance)
				{
					mInstance = MakeUniquePtr<blend_operation_define>();
				}
			}
			return *mInstance;
		}

		BlendOperation fromStr(std::string_view name) const
		{
			size_t const name_hash = boost::hash_range(name.begin(), name.end());
			for (uint32_t i = 0; i < mBOPSHash.size(); ++i)
			{
				if (mBOPSHash[i] == name_hash)
				{
					return static_cast<BlendOperation>(i + 1);
				}
			}

			AIR_UNREACHABLE("Invalid BlendOperation name");
		}

		blend_operation_define()
		{
			mBOPSHash.push_back(CT_HASH("add"));
			mBOPSHash.push_back(CT_HASH("sub"));
			mBOPSHash.push_back(CT_HASH("rev_sub"));
			mBOPSHash.push_back(CT_HASH("min"));
			mBOPSHash.push_back(CT_HASH("max"));
		}

	private:
		std::vector<size_t> mBOPSHash;

		static std::unique_ptr<blend_operation_define> mInstance;
	};
	std::unique_ptr<blend_operation_define> blend_operation_define::mInstance;

	class stencil_operation_define
	{
	public:
		static stencil_operation_define& getInstance()
		{
			if (!mInstance)
			{
				std::lock_guard<std::mutex> lock(singleton_mutex);
				if (!mInstance)
				{
					mInstance = MakeUniquePtr<stencil_operation_define>();
				}
			}
			return *mInstance;
		}

		StencilOperation fromStr(std::string_view name) const
		{
			size_t const name_hash = boost::hash_range(name.begin(), name.end());
			for (uint32_t i = 0; i < mSOPSHash.size(); ++i)
			{
				if (mSOPSHash[i] == name_hash)
				{
					return static_cast<StencilOperation>(i);
				}
			}

			AIR_UNREACHABLE("Invalid StencilOperation name");
		}

		stencil_operation_define()
		{
			mSOPSHash.push_back(CT_HASH("keep"));
			mSOPSHash.push_back(CT_HASH("zero"));
			mSOPSHash.push_back(CT_HASH("replace"));
			mSOPSHash.push_back(CT_HASH("incr"));
			mSOPSHash.push_back(CT_HASH("decr"));
			mSOPSHash.push_back(CT_HASH("invert"));
			mSOPSHash.push_back(CT_HASH("incr_wrap"));
			mSOPSHash.push_back(CT_HASH("decr_wrap"));
		}

	private:
		std::vector<size_t> mSOPSHash;

		static std::unique_ptr<stencil_operation_define> mInstance;
	};
	std::unique_ptr<stencil_operation_define> stencil_operation_define::mInstance;

	class texture_addr_mode_define
	{
	public:
		static texture_addr_mode_define& getInstance()
		{
			if (!mInstance)
			{
				std::lock_guard<std::mutex> lock(singleton_mutex);
				if (!mInstance)
				{
					mInstance = MakeUniquePtr<texture_addr_mode_define>();
				}
			}
			return *mInstance;
		}

		TexAddressingMode fromStr(std::string_view name) const
		{
			size_t const name_hash = boost::hash_range(name.begin(), name.end());
			for (uint32_t i = 0; i < mTAMSHash.size(); ++i)
			{
				if (mTAMSHash[i] == name_hash)
				{
					return static_cast<TexAddressingMode>(i);
				}
			}

			AIR_UNREACHABLE("Invalid TexAddressingMode name");
		}

		texture_addr_mode_define()
		{
			mTAMSHash.push_back(CT_HASH("wrap"));
			mTAMSHash.push_back(CT_HASH("mirror"));
			mTAMSHash.push_back(CT_HASH("clamp"));
			mTAMSHash.push_back(CT_HASH("border"));
		}

	private:
		std::vector<size_t> mTAMSHash;

		static std::unique_ptr<texture_addr_mode_define> mInstance;
	};
	std::unique_ptr<texture_addr_mode_define> texture_addr_mode_define::mInstance;

	class logic_operation_define
	{
	public:
		static logic_operation_define& getInstance()
		{
			if (!mInstance)
			{
				std::lock_guard<std::mutex> lock(singleton_mutex);
				if (!mInstance)
				{
					mInstance = MakeUniquePtr<logic_operation_define>();
				}
			}
			return *mInstance;
		}

		LogicOperation fromStr(std::string_view name) const
		{
			size_t const name_hash = boost::hash_range(name.begin(), name.end());
			for (uint32_t i = 0; i < mLOPSHash.size(); ++i)
			{
				if (mLOPSHash[i] == name_hash)
				{
					return static_cast<LogicOperation>(i);
				}
			}

			AIR_UNREACHABLE("Invalid LogicOperation name");
		}

		logic_operation_define()
		{
			mLOPSHash.push_back(CT_HASH("clear"));
			mLOPSHash.push_back(CT_HASH("set"));
			mLOPSHash.push_back(CT_HASH("copy"));
			mLOPSHash.push_back(CT_HASH("copy_inverted"));
			mLOPSHash.push_back(CT_HASH("noop"));
			mLOPSHash.push_back(CT_HASH("invert"));
			mLOPSHash.push_back(CT_HASH("and"));
			mLOPSHash.push_back(CT_HASH("nand"));
			mLOPSHash.push_back(CT_HASH("or"));
			mLOPSHash.push_back(CT_HASH("nor"));
			mLOPSHash.push_back(CT_HASH("xor"));
			mLOPSHash.push_back(CT_HASH("equiv"));
			mLOPSHash.push_back(CT_HASH("and_reverse"));
			mLOPSHash.push_back(CT_HASH("and_inverted"));
			mLOPSHash.push_back(CT_HASH("or_reverse"));
			mLOPSHash.push_back(CT_HASH("or_inverted"));
		}

	private:
		std::vector<size_t> mLOPSHash;

		static std::unique_ptr<logic_operation_define> mInstance;
	};
	std::unique_ptr<logic_operation_define> logic_operation_define::mInstance;

	class domain_type_define
	{
	public:
		static domain_type_define& getInstance()
		{
			if (!mInstance)
			{
				std::lock_guard<std::mutex> lock(singleton_mutex);
				if (!mInstance)
				{
					mInstance = MakeUniquePtr<domain_type_define>();
				}
			}
			return *mInstance;
		}

		DomainType fromStr(std::string_view name) const
		{
			size_t const name_hash = boost::hash_range(name.begin(), name.end());
			switch (name_hash)
			{
			case CT_HASH("surface"):
				return DT_Surface;

			case CT_HASH("deferredDecal"):
				return DT_DeferredDecal;

			case CT_HASH("lightFunction"):
				return DT_LightFunction;

			case CT_HASH("postProcess"):
				return DT_PostProcess;

			case CT_HASH("userInterface"):
				return DT_UserInterface;
			default:
				break;
			}
			AIR_UNREACHABLE("Invalid DomainType name");
		}

		domain_type_define()
		{
		}
	private:
		static std::unique_ptr<domain_type_define> mInstance;
	};
	std::unique_ptr<domain_type_define> domain_type_define::mInstance;

	class blend_mode_type_define
	{
	public:
		static blend_mode_type_define& getInstance()
		{
			if (!mInstance)
			{
				std::lock_guard<std::mutex> lock(singleton_mutex);
				if (!mInstance)
				{
					mInstance = MakeUniquePtr<blend_mode_type_define>();
				}
			}
			return *mInstance;
		}

		BlendModeType fromStr(std::string_view name) const
		{
			size_t const name_hash = boost::hash_range(name.begin(), name.end());
			switch (name_hash)
			{
			case CT_HASH("opaque"):
				return BMT_Opaque;

			case CT_HASH("masked"):
				return BMT_Masked;

			case CT_HASH("translucent"):
				return BMT_Translucent;

			case CT_HASH("additive"):
				return BMT_Additive;

			case CT_HASH("modulate"):
				return BMT_Modulate;

			case CT_HASH("alphaComposite"):
				return BMT_AlphaComposite;

			default:
				break;
			}
			AIR_UNREACHABLE("Invalid BlendModeType name");
		}

		blend_mode_type_define()
		{
		}
	private:
		static std::unique_ptr<blend_mode_type_define> mInstance;
	};
	std::unique_ptr<blend_mode_type_define> blend_mode_type_define::mInstance;




	class shading_model_type_define
	{
	public:
		static shading_model_type_define& getInstance()
		{
			if (!mInstance)
			{
				std::lock_guard<std::mutex> lock(singleton_mutex);
				if (!mInstance)
				{
					mInstance = MakeUniquePtr<shading_model_type_define>();
				}
			}
			return *mInstance;
		}

		ShadingModelType fromStr(std::string_view name) const
		{
			size_t const name_hash = boost::hash_range(name.begin(), name.end());
			switch (name_hash)
			{
			case CT_HASH("unlit"):
				return SMT_Unlit;

			case CT_HASH("defaultLit"):
				return SMT_DefaultLit;

			case CT_HASH("subsurface"):
				return SMT_Subsurface;

			case CT_HASH("preintegratedSkin"):
				return SMT_PreintegratedSkin;

			case CT_HASH("clearCoat"):
				return SMT_ClearCoat;

			case CT_HASH("subsurfaceProfile"):
				return SMT_SubsurfaceProfile;

			case CT_HASH("twoSidedFoliage"):
				return SMT_TwoSidedFoliage;

			case CT_HASH("hair"):
				return SMT_Hair;
				
			case CT_HASH("cloth"):
				return SMT_Cloth;

			case CT_HASH("eye"):
				return SMT_Eye;
			default:
				break;
			}
			AIR_UNREACHABLE("Invalid ShadingModelType name");
		}

		shading_model_type_define()
		{
		}
	private:
		static std::unique_ptr<shading_model_type_define> mInstance;
	};
	std::unique_ptr<shading_model_type_define> shading_model_type_define::mInstance;
	
	



#if AIR_IS_DEV_PLATFORM
	bool BoolFromStr(std::string_view name)
	{
		if (("true" == name) || ("1" == name))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	template <typename T, int N>
	Vector_T<T, N> Vector3FromStr(std::string_view str)
	{
		size_t right = str.find_first_of(')');
		size_t left = str.find_first_of('(');
		std::string_view ss = str.substr(left + 1);
		Vector_T<T, N> vec;
		if ((right != -1 )&& (left != -1))
		{
			for (int i = 0; i < N - 1; i++)
			{
				size_t index = ss.find_first_of(',');
				std::string vstr = std::string(ss.substr(0, index));
				ss = ss.substr(index + 1);
				boost::trim(vstr);
				vec[i] = boost::lexical_cast<T>(vstr);
			}
			std::string vstr = std::string(ss.substr(0, ss.find_first_of(')')));
			boost::trim(vstr);
			vec[N - 1] = boost::lexical_cast<T>(vstr);
		}
		return vec;
	}

	int get_index(XMLNodePtr const & node)
	{
		int index = 0; 
		XMLAttributePtr attr = node->getAttrib("index");
		if (attr)
		{
			index = attr->getValueInt();
		}
		return index;
	}

	std::string get_profile(XMLNodePtr const & node)
	{
		XMLAttributePtr attr = node->getAttrib("profile");
		if (attr)
		{
			return attr->getValueString();
		}
		else
		{
			return "auto";
		}
	}

	std::string get_func_name(XMLNodePtr const & node)
	{
		std::string value = node->getAttribString("value");
		return value.substr(0, value.find("("));
	}


	std::unique_ptr<RenderVariable> read_var(XMLNodePtr const & node, uint32_t type, uint32_t array_size)
	{
		std::unique_ptr<RenderVariable> var;
		XMLAttributePtr attr;
		switch (type)
		{
		case REDT_bool:
			if (0 == array_size)
			{
				attr = node->getAttrib("value");
				bool tmp = false;
				if (attr)
				{
					std::string value_str = attr->getValueString();
					tmp = BoolFromStr(value_str);
				}
				var = MakeUniquePtr<RenderVariableBool>();
				*var = tmp;
			}
			break;
		case REDT_uint:
			if (0 == array_size)
			{
				attr = node->getAttrib("value");
				uint32_t tmp = 0;
				if (attr)
				{
					tmp = attr->getValueInt();
				}
				var = MakeUniquePtr<RenderVariableUInt>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableUIntArray>();
				XMLNodePtr value_node = node->getFirstNode("value");
				if (value_node)
				{
					value_node = value_node->getFirstNode();
					if (value_node && (XNT_CData == value_node->getType()))
					{
						std::string value_str = value_node->getValueString();
						std::vector<std::string> strs;
						boost::algorithm::split(strs, value_str, boost::is_any_of(","));
						std::vector<uint32_t> init_val(std::min(array_size, static_cast<uint32_t>(strs.size())), 0);
						for (size_t index = 0; index < init_val.size(); ++index)
						{
							if (index < strs.size())
							{
								boost::algorithm::trim(strs[index]);
								init_val[index] = boost::lexical_cast<uint32_t>(strs[index]);
							}
						}
						*var = init_val;
					}
				}
			}
			break;
		case REDT_int:
			if (0 == array_size)
			{
				attr = node->getAttrib("value");
				int32_t tmp = 0;
				if (attr)
				{
					tmp = attr->getValueInt();
				}
				var = MakeUniquePtr<RenderVariableInt>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableIntArray>();
				XMLNodePtr value_node = node->getFirstNode("value");
				if (value_node)
				{
					value_node = value_node->getFirstNode();
					if (value_node && (XNT_CData == value_node->getType()))
					{
						std::string value_str = value_node->getValueString();
						std::vector<std::string> strs;
						boost::algorithm::split(strs, value_str, boost::is_any_of(","));
						std::vector<int32_t> init_val(std::min(array_size, static_cast<uint32_t>(strs.size())), 0);
						for (size_t index = 0; index < init_val.size(); ++index)
						{
							if (index < strs.size())
							{
								boost::algorithm::trim(strs[index]);
								init_val[index] = boost::lexical_cast<int32_t>(strs[index]);
							}
						}
						*var = init_val;
					}
				}
			}
			break;
		case REDT_string:
		{
			attr = node->getAttrib("value");
			std::string tmp;
			if (attr)
			{
				tmp = attr->getValueString();
			}
			var = MakeUniquePtr<RenderVariableString>();
			*var = tmp;
		}
		break;
		case REDT_texture1D:
		case REDT_texture2D:
		case REDT_texture3D:
		case REDT_textureCUBE:
		case REDT_texture1DArray:
		case REDT_texture2DArray:
		case REDT_texture3DArray:
		case REDT_textureCUBEArray:
		case REDT_rw_texture1D:
		case REDT_rw_texture2D:
		case REDT_rw_texture3D:
		case REDT_rw_texture1DArray:
		case REDT_rw_texture2DArray:
			var = MakeUniquePtr<RenderVariableTexture>();
			*var = TexturePtr();
			attr = node->getAttrib("elem_type");
			if (attr)
			{
				*var = attr->getValueString();
			}
			else
			{
				*var = std::string("float4");
			}
			break;
		case REDT_sampler:
		{
			SamplerStateDesc desc;
			for (XMLNodePtr state_node = node->getFirstNode("state"); state_node; state_node = state_node->getNextSibling("state"))
			{
				std::string name = state_node->getAttrib("name")->getValueString();
				size_t const name_hash = RT_HASH(name.c_str());
				if (CT_HASH("filtering") == name_hash)
				{
					std::string value_str = state_node->getAttrib("value")->getValueString();
					desc.mFilter = texture_filter_mode_define::getInstance().fromStr(value_str);
				}
				else if (CT_HASH("address_u") == name_hash)
				{
					std::string value_str = state_node->getAttribString("value");
					desc.mAddrModeU = texture_addr_mode_define::getInstance().fromStr(value_str);
				}
				else if (CT_HASH("address_v") == name_hash)
				{
					std::string value_str = state_node->getAttribString("value");
					desc.mAddrModeV = texture_addr_mode_define::getInstance().fromStr(value_str);
				}
				else if (CT_HASH("address_w") == name_hash)
				{
					std::string value_str = state_node->getAttribString("value");
					desc.mAddrModeW = texture_addr_mode_define::getInstance().fromStr(value_str);
				}
				else if (CT_HASH("max_anisotropy") == name_hash)
				{
					desc.mMaxAnisotropy = static_cast<uint8_t>(state_node->getAttribUInt("value"));
				}
				else if (CT_HASH("min_lod") == name_hash)
				{
					desc.mMinLod = state_node->getAttribFloat("value");
				}
				else if (CT_HASH("max_lod") == name_hash)
				{
					desc.mMaxLod = state_node->getAttribFloat("value");
				}
				else if (CT_HASH("mip_map_lod_bias") == name_hash)
				{
					desc.mMiPMapLoadBias = state_node->getAttribFloat("value");
				}
				else if (CT_HASH("cmp_func") == name_hash)
				{
					std::string value_str = state_node->getAttribString("value");
					desc.mCmpFunc = compare_function_define::getInstance().fromStr(value_str);
				}
				else if (CT_HASH("border_clr") == name_hash)
				{
					desc.mBorderColor.r() = state_node->getAttribFloat("r");
					desc.mBorderColor.g() = state_node->getAttribFloat("g");
					desc.mBorderColor.b() = state_node->getAttribFloat("b");
					desc.mBorderColor.a() = state_node->getAttribFloat("a");

				}
				else
				{
					AIR_UNREACHABLE("Invalid sampler state name");
				}
			}
			var = MakeUniquePtr<RenderVariableSampler>();
			*var = SingletonManager::getRenderFactoryInstance().MakeSamplerStateObject(desc);
		}
		break;
		case REDT_shader:
		{
			ShaderDesc desc;
			desc.mProfile = get_profile(node);
			desc.mFunctionName = get_func_name(node);
			var = MakeUniquePtr<RenderVariableShader>();
			*var = desc;
		}

			break;
		case REDT_float:
			if (0 == array_size)
			{
				float tmp = 0;
				attr = node->getAttrib("value");
				if (attr)
				{
					tmp = attr->getValueFloat();
				}
				var = MakeUniquePtr<RenderVariableFloat>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableFloatArray>();
				XMLNodePtr value_node = node->getFirstNode("value");
				if (value_node)
				{
					value_node = value_node->getFirstNode();
					if (value_node && (XNT_CData == value_node->getType()))
					{
						std::string value_str = value_node->getValueString();
						std::vector<std::string> strs;
						boost::algorithm::split(strs, value_str, boost::is_any_of(","));
						std::vector<float> init_val(std::min(array_size, static_cast<uint32_t>(strs.size())), 0.0f);
						for (size_t index = 0; index < init_val.size(); ++index)
						{
							if (index < strs.size())
							{
								boost::algorithm::trim(strs[index]);
								init_val[index] = boost::lexical_cast<float>(strs[index]);
							}
						}
						*var = init_val;
					}
				}
			}
			break;
		case REDT_uint2:
			if (0 == array_size)
			{
				uint2 tmp(0, 0);
				attr = node->getAttrib("x");
				if (attr)
				{
					tmp.x() = attr->getValueUInt();
				}
				attr = node->getAttrib("y");
				if (attr)
				{
					tmp.y() = attr->getValueUInt();
				}
				var = MakeUniquePtr<RenderVariableUInt2>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableInt2Array>();
				XMLNodePtr value_node = node->getFirstNode("value");
				if (value_node)
				{
					value_node = value_node->getFirstNode();
					if (value_node && (XNT_CData == value_node->getType()))
					{
						std::string value_str = value_node->getValueString();
						std::vector<std::string> strs;
						boost::algorithm::split(strs, value_str, boost::is_any_of(","));
						std::vector<uint2> init_val(std::min(array_size, static_cast<uint32_t>((strs.size() + 1) / 2)), int2(0, 0));
						for (size_t index = 0; index < init_val.size(); ++index)
						{
							for (size_t j = 0; j < 2; ++j)
							{
								if (index * 2 + j < strs.size())
								{

									boost::algorithm::trim(strs[index * 2 + j]);
									init_val[index][j] = boost::lexical_cast<uint32_t>(strs[index * 2 + j]);
								}
							}
						}
						*var = init_val;
					}
				}
			}
			break;


		case REDT_uint3:
			if (0 == array_size)
			{
				uint3 tmp(0, 0, 0);
				attr = node->getAttrib("x");
				if (attr)
				{
					tmp.x() = attr->getValueUInt();
				}
				attr = node->getAttrib("y");
				if (attr)
				{
					tmp.y() = attr->getValueUInt();
				}
				attr = node->getAttrib("z");
				if (attr)
				{
					tmp.z() = attr->getValueUInt();
				}

				var = MakeUniquePtr<RenderVariableUInt3>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableInt3Array>();

				XMLNodePtr value_node = node->getFirstNode("value");
				if (value_node)
				{
					value_node = value_node->getFirstNode();
					if (value_node && (XNT_CData == value_node->getType()))
					{
						std::string value_str = value_node->getValueString();
						std::vector<std::string> strs;
						boost::algorithm::split(strs, value_str, boost::is_any_of(","));
						std::vector<uint3> init_val(std::min(array_size, static_cast<uint32_t>((strs.size() + 2) / 3)), int3(0, 0, 0));
						for (size_t index = 0; index < init_val.size(); ++index)
						{
							for (size_t j = 0; j < 3; ++j)
							{
								if (index * 3 + j < strs.size())
								{
									boost::algorithm::trim(strs[index * 3 + j]);
									init_val[index][j] = boost::lexical_cast<uint32_t>(strs[index * 3 + j]);
								}
							}
						}
						*var = init_val;
					}
				}
			}
			break;

		case REDT_uint4:
			if (0 == array_size)
			{
				uint4 tmp(0, 0, 0, 0);
				attr = node->getAttrib("x");
				if (attr)
				{
					tmp.x() = attr->getValueUInt();
				}
				attr = node->getAttrib("y");
				if (attr)
				{
					tmp.y() = attr->getValueUInt();
				}
				attr = node->getAttrib("z");
				if (attr)
				{
					tmp.z() = attr->getValueUInt();
				}
				attr = node->getAttrib("w");
				if (attr)
				{
					tmp.w() = attr->getValueUInt();
				}

				var = MakeUniquePtr<RenderVariableUInt4>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableInt4Array>();

				XMLNodePtr value_node = node->getFirstNode("value");
				if (value_node)
				{
					value_node = value_node->getFirstNode();
					if (value_node && (XNT_CData == value_node->getType()))
					{
						std::string value_str = value_node->getValueString();
						std::vector<std::string> strs;
						boost::algorithm::split(strs, value_str, boost::is_any_of(","));
						std::vector<int4> init_val(std::min(array_size, static_cast<uint32_t>((strs.size() + 3) / 4)), int4(0, 0, 0, 0));
						for (size_t index = 0; index < init_val.size(); ++index)
						{
							for (size_t j = 0; j < 4; ++j)
							{
								if (index * 4 + j < strs.size())
								{
									boost::algorithm::trim(strs[index * 4 + j]);
									init_val[index][j] = boost::lexical_cast<uint32_t>(strs[index * 4 + j]);
								}
							}
						}
						*var = init_val;
					}
				}
			}
			break;

		case REDT_int2:
			if (0 == array_size)
			{
				int2 tmp(0, 0);
				attr = node->getAttrib("x");
				if (attr)
				{
					tmp.x() = attr->getValueInt();
				}
				attr = node->getAttrib("y");
				if (attr)
				{
					tmp.y() = attr->getValueInt();
				}

				var = MakeUniquePtr<RenderVariableInt2>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableInt2Array>();

				XMLNodePtr value_node = node->getFirstNode("value");
				if (value_node)
				{
					value_node = value_node->getFirstNode();
					if (value_node && (XNT_CData == value_node->getType()))
					{
						std::string value_str = value_node->getValueString();
						std::vector<std::string> strs;
						boost::algorithm::split(strs, value_str, boost::is_any_of(","));
						std::vector<int2> init_val(std::min(array_size, static_cast<uint32_t>((strs.size() + 1) / 2)), int2(0, 0));
						for (size_t index = 0; index < init_val.size(); ++index)
						{
							for (size_t j = 0; j < 2; ++j)
							{
								if (index * 2 + j < strs.size())
								{
									boost::algorithm::trim(strs[index * 2 + j]);
									init_val[index][j] = boost::lexical_cast<int32_t>(strs[index * 2 + j]);
								}
							}
						}
						*var = init_val;
					}
				}
			}
			break;

		case REDT_int3:
			if (0 == array_size)
			{
				int3 tmp(0, 0, 0);
				attr = node->getAttrib("x");
				if (attr)
				{
					tmp.x() = attr->getValueInt();
				}
				attr = node->getAttrib("y");
				if (attr)
				{
					tmp.y() = attr->getValueInt();
				}
				attr = node->getAttrib("z");
				if (attr)
				{
					tmp.z() = attr->getValueInt();
				}

				var = MakeUniquePtr<RenderVariableInt3>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableInt3Array>();

				XMLNodePtr value_node = node->getFirstNode("value");
				if (value_node)
				{
					value_node = value_node->getFirstNode();
					if (value_node && (XNT_CData == value_node->getType()))
					{
						std::string value_str = value_node->getValueString();
						std::vector<std::string> strs;
						boost::algorithm::split(strs, value_str, boost::is_any_of(","));
						std::vector<int3> init_val(std::min(array_size, static_cast<uint32_t>((strs.size() + 2) / 3)), int3(0, 0, 0));
						for (size_t index = 0; index < init_val.size(); ++index)
						{
							for (size_t j = 0; j < 3; ++j)
							{
								if (index * 3 + j < strs.size())
								{
									boost::algorithm::trim(strs[index * 3 + j]);
									init_val[index][j] = boost::lexical_cast<int32_t>(strs[index * 3 + j]);
								}
							}
						}
						*var = init_val;
					}
				}
			}
			break;

		case REDT_int4:
			if (0 == array_size)
			{
				int4 tmp(0, 0, 0, 0);
				attr = node->getAttrib("x");
				if (attr)
				{
					tmp.x() = attr->getValueInt();
				}
				attr = node->getAttrib("y");
				if (attr)
				{
					tmp.y() = attr->getValueInt();
				}
				attr = node->getAttrib("z");
				if (attr)
				{
					tmp.z() = attr->getValueInt();
				}
				attr = node->getAttrib("w");
				if (attr)
				{
					tmp.w() = attr->getValueInt();
				}

				var = MakeUniquePtr<RenderVariableInt4>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableInt4Array>();

				XMLNodePtr value_node = node->getFirstNode("value");
				if (value_node)
				{
					value_node = value_node->getFirstNode();
					if (value_node && (XNT_CData == value_node->getType()))
					{
						std::string value_str = value_node->getValueString();
						std::vector<std::string> strs;
						boost::algorithm::split(strs, value_str, boost::is_any_of(","));
						std::vector<int4> init_val(std::min(array_size, static_cast<uint32_t>((strs.size() + 3) / 4)), int4(0, 0, 0, 0));
						for (size_t index = 0; index < init_val.size(); ++index)
						{
							for (size_t j = 0; j < 4; ++j)
							{
								if (index * 4 + j < strs.size())
								{
									boost::algorithm::trim(strs[index * 4 + j]);
									init_val[index][j] = boost::lexical_cast<int32_t>(strs[index * 4 + j]);
								}
							}
						}
						*var = init_val;
					}
				}
			}
			break;

		case REDT_float2:
			if (0 == array_size)
			{
				float2 tmp(0, 0);
				attr = node->getAttrib("x");
				if (attr)
				{
					tmp.x() = attr->getValueFloat();
				}
				attr = node->getAttrib("y");
				if (attr)
				{
					tmp.y() = attr->getValueFloat();
				}

				var = MakeUniquePtr<RenderVariableFloat2>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableFloat2Array>();

				XMLNodePtr value_node = node->getFirstNode("value");
				if (value_node)
				{
					value_node = value_node->getFirstNode();
					if (value_node && (XNT_CData == value_node->getType()))
					{
						std::string value_str = value_node->getValueString();
						std::vector<std::string> strs;
						boost::algorithm::split(strs, value_str, boost::is_any_of(","));
						std::vector<float2> init_val(std::min(array_size, static_cast<uint32_t>((strs.size() + 1) / 2)), float2(0, 0));
						for (size_t index = 0; index < init_val.size(); ++index)
						{
							for (size_t j = 0; j < 2; ++j)
							{
								if (index * 2 + j < strs.size())
								{
									boost::algorithm::trim(strs[index * 2 + j]);
									init_val[index][j] = boost::lexical_cast<float>(strs[index * 2 + j]);
								}
							}
						}
						*var = init_val;
					}
				}
			}
			break;

		case REDT_float3:
			if (0 == array_size)
			{
				float3 tmp(0, 0, 0);
				attr = node->getAttrib("x");
				if (attr)
				{
					tmp.x() = attr->getValueFloat();
				}
				attr = node->getAttrib("y");
				if (attr)
				{
					tmp.y() = attr->getValueFloat();
				}
				attr = node->getAttrib("z");
				if (attr)
				{
					tmp.z() = attr->getValueFloat();
				}

				var = MakeUniquePtr<RenderVariableFloat3>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableFloat3Array>();

				XMLNodePtr value_node = node->getFirstNode("value");
				if (value_node)
				{
					value_node = value_node->getFirstNode();
					if (value_node && (XNT_CData == value_node->getType()))
					{
						std::string value_str = value_node->getValueString();
						std::vector<std::string> strs;
						boost::algorithm::split(strs, value_str, boost::is_any_of(","));
						std::vector<float3> init_val(std::min(array_size, static_cast<uint32_t>((strs.size() + 2) / 3)), float3(0, 0, 0));
						for (size_t index = 0; index < init_val.size(); ++index)
						{
							for (size_t j = 0; j < 3; ++j)
							{
								if (index * 3 + j < strs.size())
								{
									boost::algorithm::trim(strs[index * 3 + j]);
									init_val[index][j] = boost::lexical_cast<float>(strs[index * 3 + j]);
								}
							}
						}
						*var = init_val;
					}
				}
			}
			break;

		case REDT_float4:
			if (0 == array_size)
			{
				float4 tmp(0, 0, 0, 0);
				attr = node->getAttrib("x");
				if (attr)
				{
					tmp.x() = attr->getValueFloat();
				}
				attr = node->getAttrib("y");
				if (attr)
				{
					tmp.y() = attr->getValueFloat();
				}
				attr = node->getAttrib("z");
				if (attr)
				{
					tmp.z() = attr->getValueFloat();
				}
				attr = node->getAttrib("w");
				if (attr)
				{
					tmp.w() = attr->getValueFloat();
				}

				var = MakeUniquePtr<RenderVariableFloat4>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableFloat4Array>();

				XMLNodePtr value_node = node->getFirstNode("value");
				if (value_node)
				{
					value_node = value_node->getFirstNode();
					if (value_node && (XNT_CData == value_node->getType()))
					{
						std::string value_str = value_node->getValueString();
						std::vector<std::string> strs;
						boost::algorithm::split(strs, value_str, boost::is_any_of(","));
						std::vector<float4> init_val(std::min(array_size, static_cast<uint32_t>((strs.size() + 3) / 4)), float4(0, 0, 0, 0));
						for (size_t index = 0; index < init_val.size(); ++index)
						{
							for (size_t j = 0; j < 4; ++j)
							{
								if (index * 4 + j < strs.size())
								{
									boost::algorithm::trim(strs[index * 4 + j]);
									init_val[index][j] = boost::lexical_cast<float>(strs[index * 4 + j]);
								}
							}
						}
						*var = init_val;
					}
				}
			}
			break;

		case REDT_float4x4:
			if (0 == array_size)
			{
				float4x4 tmp(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
				for (int y = 0; y < 4; ++y)
				{
					for (int x = 0; x < 4; ++x)
					{
						attr = node->getAttrib(std::string("_")
							+ static_cast<char>('0' + y) + static_cast<char>('0' + x));
						if (attr)
						{
							tmp[y * 4 + x] = attr->getValueFloat();
						}
					}
				}

				var = MakeUniquePtr<RenderVariableFloat4x4>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableFloat4x4Array>();

				XMLNodePtr value_node = node->getFirstNode("value");
				if (value_node)
				{
					value_node = value_node->getFirstNode();
					if (value_node && (XNT_CData == value_node->getType()))
					{
						std::string value_str = value_node->getValueString();
						std::vector<std::string> strs;
						boost::algorithm::split(strs, value_str, boost::is_any_of(","));
						std::vector<float4> init_val(std::min(array_size, static_cast<uint32_t>((strs.size() + 3) / 4)), float4(0, 0, 0, 0));
						for (size_t index = 0; index < init_val.size(); ++index)
						{
							for (size_t j = 0; j < 16; ++j)
							{
								if (index * 16 + j < strs.size())
								{
									boost::algorithm::trim(strs[index * 16 + j]);
									init_val[index][j] = boost::lexical_cast<float>(strs[index * 16 + j]);
								}
							}
						}
						*var = init_val;
					}
				}
			}
			break;

		case REDT_buffer:
		case REDT_structured_buffer:
		case REDT_rw_buffer:
		case REDT_rw_structured_buffer:
		case REDT_consume_structured_buffer:
		case REDT_append_structured_buffer:
			var = MakeUniquePtr<RenderVariableBuffer>();
			*var = GraphicsBufferPtr();
			attr = node->getAttrib("elem_type");
			if (attr)
			{
				*var = attr->getValueString();
			}
			else
			{
				*var = std::string("float4");
			}
			break;

		case REDT_byte_address_buffer:
		case REDT_rw_byte_address_buffer:
			
			var = MakeUniquePtr<RenderVariableByteAddressBuffer>();
			*var = GraphicsBufferPtr();
			break;

		default:
			AIR_UNREACHABLE("Invalid type");
			break;
		}
		return var;
	}


#endif
	std::unique_ptr<RenderVariable> stream_in_var(ResIdentifierPtr const & res, uint32_t type, uint32_t array_size)
	{
		
		std::unique_ptr<RenderVariable> var;

		switch (type)
		{
		case REDT_bool:
			if (0 == array_size)
			{
				bool tmp;
				res->read(&tmp, sizeof(tmp));

				var = MakeUniquePtr<RenderVariableBool>();
				*var = tmp;
			}
			break;

		case REDT_uint:
			if (0 == array_size)
			{
				uint32_t tmp;
				res->read(&tmp, sizeof(tmp));

				var = MakeUniquePtr<RenderVariableUInt>();
				*var = LE2Native(tmp);
			}
			else
			{
				var = MakeUniquePtr<RenderVariableUIntArray>();

				uint32_t len;
				res->read(&len, sizeof(len));
				len = LE2Native(len);
				if (len > 0)
				{
					std::vector<uint32_t> init_val(len);
					res->read(&init_val[0], len * sizeof(init_val[0]));
					for (uint32_t i = 0; i < len; ++i)
					{
						init_val[i] = LE2Native(init_val[i]);
					}
					*var = init_val;
				}
			}
			break;

		case REDT_int:
			if (0 == array_size)
			{
				int32_t tmp;
				res->read(&tmp, sizeof(tmp));

				var = MakeUniquePtr<RenderVariableInt>();
				*var = LE2Native(tmp);
			}
			else
			{
				var = MakeUniquePtr<RenderVariableIntArray>();

				uint32_t len;
				res->read(&len, sizeof(len));
				len = LE2Native(len);
				if (len > 0)
				{
					std::vector<int32_t> init_val(len);
					res->read(&init_val[0], len * sizeof(init_val[0]));
					for (uint32_t i = 0; i < len; ++i)
					{
						init_val[i] = LE2Native(init_val[i]);
					}
					*var = init_val;
				}
			}
			break;

		case REDT_string:
		{
			var = MakeUniquePtr<RenderVariableString>();
			*var = readShortString(res);
		}
		break;

		case REDT_texture1D:
		case REDT_texture2D:
		case REDT_texture3D:
		case REDT_textureCUBE:
		case REDT_texture1DArray:
		case REDT_texture2DArray:
		case REDT_texture3DArray:
		case REDT_textureCUBEArray:
		case REDT_rw_texture1D:
		case REDT_rw_texture2D:
		case REDT_rw_texture3D:
		case REDT_rw_texture1DArray:
		case REDT_rw_texture2DArray:
		{
			var = MakeUniquePtr<RenderVariableTexture>();
			*var = TexturePtr();
			*var = readShortString(res);
		}
		break;

		case REDT_sampler:
		{
			SamplerStateDesc desc;
			res->read(&desc, sizeof(desc));
			desc.mBorderColor[0] = LE2Native(desc.mBorderColor[0]);
			desc.mBorderColor[1] = LE2Native(desc.mBorderColor[1]);
			desc.mBorderColor[2] = LE2Native(desc.mBorderColor[2]);
			desc.mBorderColor[3] = LE2Native(desc.mBorderColor[3]);
			desc.mAddrModeU = LE2Native(desc.mAddrModeU);
			desc.mAddrModeV = LE2Native(desc.mAddrModeV);
			desc.mAddrModeW = LE2Native(desc.mAddrModeW);
			desc.mFilter = LE2Native(desc.mFilter);
			desc.mMinLod = LE2Native(desc.mMinLod);
			desc.mMaxLod = LE2Native(desc.mMaxLod);
			desc.mMiPMapLoadBias = LE2Native(desc.mMiPMapLoadBias);
			desc.mCmpFunc = LE2Native(desc.mCmpFunc);

			var = MakeUniquePtr<RenderVariableSampler>();
			*var = SingletonManager::getRenderFactoryInstance().MakeSamplerStateObject(desc);
		}
		break;

		case REDT_shader:
		{
			ShaderDesc desc;
			desc.mProfile = readShortString(res);
			desc.mFunctionName = readShortString(res);

			var = MakeUniquePtr<RenderVariableShader>();
			*var = desc;
		}
		break;

		case REDT_float:
			if (0 == array_size)
			{
				float tmp;
				res->read(&tmp, sizeof(tmp));

				var = MakeUniquePtr<RenderVariableFloat>();
				*var = LE2Native(tmp);
			}
			else
			{
				var = MakeUniquePtr<RenderVariableFloatArray>();

				uint32_t len;
				res->read(&len, sizeof(len));
				len = LE2Native(len);
				if (len > 0)
				{
					std::vector<float> init_val(len);
					res->read(&init_val[0], len * sizeof(init_val[0]));
					for (uint32_t i = 0; i < len; ++i)
					{
						init_val[i] = LE2Native(init_val[i]);
					}
					*var = init_val;
				}
			}
			break;

		case REDT_uint2:
			if (0 == array_size)
			{
				uint2 tmp;
				res->read(&tmp, sizeof(tmp));
				for (int i = 0; i < 2; ++i)
				{
					tmp[i] = LE2Native(tmp[i]);
				}

				var = MakeUniquePtr<RenderVariableUInt2>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableInt2Array>();

				uint32_t len;
				res->read(&len, sizeof(len));
				len = LE2Native(len);
				if (len > 0)
				{
					std::vector<int2> init_val(len);
					res->read(&init_val[0], len * sizeof(init_val[0]));
					for (uint32_t i = 0; i < len; ++i)
					{
						for (int j = 0; j < 2; ++j)
						{
							init_val[i][j] = LE2Native(init_val[i][j]);
						}
					}
					*var = init_val;
				}
			}
			break;

		case REDT_uint3:
			if (0 == array_size)
			{
				uint3 tmp;
				res->read(&tmp, sizeof(tmp));
				for (int i = 0; i < 3; ++i)
				{
					tmp[i] = LE2Native(tmp[i]);
				}

				var = MakeUniquePtr<RenderVariableUInt3>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableInt3Array>();

				uint32_t len;
				res->read(&len, sizeof(len));
				len = LE2Native(len);
				if (len > 0)
				{
					std::vector<int3> init_val(len);
					res->read(&init_val[0], len * sizeof(init_val[0]));
					for (uint32_t i = 0; i < len; ++i)
					{
						for (int j = 0; j < 3; ++j)
						{
							init_val[i][j] = LE2Native(init_val[i][j]);
						}
					}
					*var = init_val;
				}
			}
			break;

		case REDT_uint4:
			if (0 == array_size)
			{
				uint4 tmp;
				res->read(&tmp, sizeof(tmp));
				for (int i = 0; i < 4; ++i)
				{
					tmp[i] = LE2Native(tmp[i]);
				}

				var = MakeUniquePtr<RenderVariableUInt4>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableInt4Array>();

				uint32_t len;
				res->read(&len, sizeof(len));
				len = LE2Native(len);
				if (len > 0)
				{
					std::vector<int4> init_val(len);
					res->read(&init_val[0], len * sizeof(init_val[0]));
					for (uint32_t i = 0; i < len; ++i)
					{
						for (int j = 0; j < 4; ++j)
						{
							init_val[i][j] = LE2Native(init_val[i][j]);
						}
					}
					*var = init_val;
				}
			}
			break;

		case REDT_int2:
			if (0 == array_size)
			{
				int2 tmp;
				res->read(&tmp, sizeof(tmp));
				for (int i = 0; i < 2; ++i)
				{
					tmp[i] = LE2Native(tmp[i]);
				}

				var = MakeUniquePtr<RenderVariableInt2>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableInt2Array>();

				uint32_t len;
				res->read(&len, sizeof(len));
				len = LE2Native(len);
				if (len > 0)
				{
					std::vector<int2> init_val(len);
					res->read(&init_val[0], len * sizeof(init_val[0]));
					for (uint32_t i = 0; i < len; ++i)
					{
						for (int j = 0; j < 2; ++j)
						{
							init_val[i][j] = LE2Native(init_val[i][j]);
						}
					}
					*var = init_val;
				}
			}
			break;

		case REDT_int3:
			if (0 == array_size)
			{
				int3 tmp;
				res->read(&tmp, sizeof(tmp));
				for (int i = 0; i < 3; ++i)
				{
					tmp[i] = LE2Native(tmp[i]);
				}

				var = MakeUniquePtr<RenderVariableInt3>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableInt3Array>();

				uint32_t len;
				res->read(&len, sizeof(len));
				len = LE2Native(len);
				if (len > 0)
				{
					std::vector<int3> init_val(len);
					res->read(&init_val[0], len * sizeof(init_val[0]));
					for (uint32_t i = 0; i < len; ++i)
					{
						for (int j = 0; j < 3; ++j)
						{
							init_val[i][j] = LE2Native(init_val[i][j]);
						}
					}
					*var = init_val;
				}
			}
			break;

		case REDT_int4:
			if (0 == array_size)
			{
				int4 tmp;
				res->read(&tmp, sizeof(tmp));
				for (int i = 0; i < 4; ++i)
				{
					tmp[i] = LE2Native(tmp[i]);
				}

				var = MakeUniquePtr<RenderVariableInt4>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableInt4Array>();

				uint32_t len;
				res->read(&len, sizeof(len));
				len = LE2Native(len);
				if (len > 0)
				{
					std::vector<int4> init_val(len);
					res->read(&init_val[0], len * sizeof(init_val[0]));
					for (uint32_t i = 0; i < len; ++i)
					{
						for (int j = 0; j < 4; ++j)
						{
							init_val[i][j] = LE2Native(init_val[i][j]);
						}
					}
					*var = init_val;
				}
			}
			break;

		case REDT_float2:
			if (0 == array_size)
			{
				float2 tmp;
				res->read(&tmp, sizeof(tmp));
				for (int i = 0; i < 2; ++i)
				{
					tmp[i] = LE2Native(tmp[i]);
				}

				var = MakeUniquePtr<RenderVariableFloat2>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableFloat2Array>();

				uint32_t len;
				res->read(&len, sizeof(len));
				len = LE2Native(len);
				if (len > 0)
				{
					std::vector<float2> init_val(len);
					res->read(&init_val[0], len * sizeof(init_val[0]));
					for (uint32_t i = 0; i < len; ++i)
					{
						for (int j = 0; j < 2; ++j)
						{
							init_val[i][j] = LE2Native(init_val[i][j]);
						}
					}
					*var = init_val;
				}
			}
			break;

		case REDT_float3:
			if (0 == array_size)
			{
				float3 tmp;
				res->read(&tmp, sizeof(tmp));
				for (int i = 0; i < 3; ++i)
				{
					tmp[i] = LE2Native(tmp[i]);
				}

				var = MakeUniquePtr<RenderVariableFloat3>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableFloat3Array>();

				uint32_t len;
				res->read(&len, sizeof(len));
				len = LE2Native(len);
				if (len > 0)
				{
					std::vector<float3> init_val(len);
					res->read(&init_val[0], len * sizeof(init_val[0]));
					for (uint32_t i = 0; i < len; ++i)
					{
						for (int j = 0; j < 3; ++j)
						{
							init_val[i][j] = LE2Native(init_val[i][j]);
						}
					}
					*var = init_val;
				}
			}
			break;

		case REDT_float4:
			if (0 == array_size)
			{
				float4 tmp;
				res->read(&tmp, sizeof(tmp));
				for (int i = 0; i < 4; ++i)
				{
					tmp[i] = LE2Native(tmp[i]);
				}

				var = MakeUniquePtr<RenderVariableFloat4>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableFloat4Array>();

				uint32_t len;
				res->read(&len, sizeof(len));
				len = LE2Native(len);
				if (len > 0)
				{
					std::vector<float4> init_val(len);
					res->read(&init_val[0], len * sizeof(init_val[0]));
					for (uint32_t i = 0; i < len; ++i)
					{
						for (int j = 0; j < 4; ++j)
						{
							init_val[i][j] = LE2Native(init_val[i][j]);
						}
					}
					*var = init_val;
				}
			}
			break;

		case REDT_float4x4:
			if (0 == array_size)
			{
				float4x4 tmp;
				res->read(&tmp, sizeof(tmp));
				for (int i = 0; i < 16; ++i)
				{
					tmp[i] = LE2Native(tmp[i]);
				}

				var = MakeUniquePtr<RenderVariableFloat4x4>();
				*var = tmp;
			}
			else
			{
				var = MakeUniquePtr<RenderVariableFloat4x4Array>();

				uint32_t len;
				res->read(&len, sizeof(len));
				len = LE2Native(len);
				if (len > 0)
				{
					std::vector<float4x4> init_val(len);
					res->read(&init_val[0], len * sizeof(init_val[0]));
					for (uint32_t i = 0; i < len; ++i)
					{
						for (int j = 0; j < 16; ++j)
						{
							init_val[i][j] = LE2Native(init_val[i][j]);
						}
					}
					*var = init_val;
				}
			}
			break;

		case REDT_buffer:
		case REDT_structured_buffer:
		case REDT_rw_buffer:
		case REDT_rw_structured_buffer:
		case REDT_consume_structured_buffer:
		case REDT_append_structured_buffer:
		{
			var = MakeUniquePtr<RenderVariableBuffer>();
			*var = GraphicsBufferPtr();
			*var = readShortString(res);
		}
		break;

		case REDT_byte_address_buffer:
		case REDT_rw_byte_address_buffer:
			var = MakeUniquePtr<RenderVariableByteAddressBuffer>();
			*var = GraphicsBufferPtr();
			break;

		default:
			AIR_UNREACHABLE("Invalid type");
		}

		return var;
	}


#if AIR_IS_DEV_PLATFORM
	void stream_out_var(std::ostream& os, RenderVariable const & var, uint32_t type, uint32_t array_size)
	{
		switch (type)
		{
		case REDT_bool:
			if (0 == array_size)
			{
				bool tmp;
				var.getValue(tmp);

				os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
			}
			break;

		case REDT_uint:
			if (0 == array_size)
			{
				uint32_t tmp;
				var.getValue(tmp);

				tmp = Native2LE(tmp);
				os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
			}
			else
			{
				std::vector<uint32_t> init_val;
				var.getValue(init_val);

				uint32_t len = static_cast<uint32_t>(init_val.size());
				len = Native2LE(len);
				os.write(reinterpret_cast<char const *>(&len), sizeof(len));
				if (len > 0)
				{
					for (uint32_t i = 0; i < len; ++i)
					{
						init_val[i] = Native2LE(init_val[i]);
					}
					os.write(reinterpret_cast<char const *>(&init_val[0]), len * sizeof(init_val[0]));
				}
			}
			break;

		case REDT_int:
			if (0 == array_size)
			{
				int32_t tmp;
				var.getValue(tmp);

				tmp = Native2LE(tmp);
				os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
			}
			else
			{
				std::vector<int32_t> init_val;
				var.getValue(init_val);

				uint32_t len = static_cast<uint32_t>(init_val.size());
				len = Native2LE(len);
				os.write(reinterpret_cast<char const *>(&len), sizeof(len));
				if (len > 0)
				{
					for (uint32_t i = 0; i < len; ++i)
					{
						init_val[i] = Native2LE(init_val[i]);
					}
					os.write(reinterpret_cast<char const *>(&init_val[0]), len * sizeof(init_val[0]));
				}
			}
			break;

		case REDT_string:
		{
			std::string tmp;
			var.getValue(tmp);
			writeShortString(os, tmp);
		}
		break;

		case REDT_texture1D:
		case REDT_texture2D:
		case REDT_texture3D:
		case REDT_textureCUBE:
		case REDT_texture1DArray:
		case REDT_texture2DArray:
		case REDT_texture3DArray:
		case REDT_textureCUBEArray:
		case REDT_rw_texture1D:
		case REDT_rw_texture2D:
		case REDT_rw_texture3D:
		case REDT_rw_texture1DArray:
		case REDT_rw_texture2DArray:
		{
			std::string tmp;
			var.getValue(tmp);
			writeShortString(os, tmp);
		}
		break;

		case REDT_sampler:
		{
			SamplerStateObjectPtr tmp;
			var.getValue(tmp);
			SamplerStateDesc desc = tmp->getDesc();
			desc.mBorderColor[0] = Native2LE(desc.mBorderColor[0]);
			desc.mBorderColor[1] = Native2LE(desc.mBorderColor[1]);
			desc.mBorderColor[2] = Native2LE(desc.mBorderColor[2]);
			desc.mBorderColor[3] = Native2LE(desc.mBorderColor[3]);
			desc.mAddrModeU = Native2LE(desc.mAddrModeU);
			desc.mAddrModeV = Native2LE(desc.mAddrModeV);
			desc.mAddrModeW = Native2LE(desc.mAddrModeW);
			desc.mFilter= Native2LE(desc.mFilter);
			desc.mMinLod = Native2LE(desc.mMinLod);
			desc.mMaxLod = Native2LE(desc.mMaxLod);
			desc.mMiPMapLoadBias = Native2LE(desc.mMiPMapLoadBias);
			desc.mCmpFunc = Native2LE(desc.mCmpFunc);
			os.write(reinterpret_cast<char const *>(&desc), sizeof(desc));
		}
		break;

		case REDT_shader:
		{
			ShaderDesc tmp;
			var.getValue(tmp);
			writeShortString(os, tmp.mProfile);
			writeShortString(os, tmp.mFunctionName);
		}
		break;

		case REDT_float:
			if (0 == array_size)
			{
				float tmp;
				var.getValue(tmp);

				tmp = Native2LE(tmp);
				os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
			}
			else
			{
				std::vector<float> init_val;
				var.getValue(init_val);

				uint32_t len = static_cast<uint32_t>(init_val.size());
				len = Native2LE(len);
				os.write(reinterpret_cast<char const *>(&len), sizeof(len));
				if (len > 0)
				{
					for (uint32_t i = 0; i < len; ++i)
					{
						init_val[i] = Native2LE(init_val[i]);
					}
					os.write(reinterpret_cast<char const *>(&init_val[0]), len * sizeof(init_val[0]));
				}
			}
			break;

		case REDT_uint2:
			if (0 == array_size)
			{
				uint2 tmp;
				var.getValue(tmp);

				for (int i = 0; i < 2; ++i)
				{
					tmp[i] = Native2LE(tmp[i]);
				}
				os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
			}
			else
			{
				std::vector<int2> init_val;
				var.getValue(init_val);

				uint32_t len = static_cast<uint32_t>(init_val.size());
				len = Native2LE(len);
				os.write(reinterpret_cast<char const *>(&len), sizeof(len));
				if (len > 0)
				{
					for (uint32_t i = 0; i < len; ++i)
					{
						for (int j = 0; j < 2; ++j)
						{
							init_val[i][j] = Native2LE(init_val[i][j]);
						}
					}
					os.write(reinterpret_cast<char const *>(&init_val[0]), len * sizeof(init_val[0]));
				}
			}
			break;

		case REDT_uint3:
			if (0 == array_size)
			{
				uint3 tmp;
				var.getValue(tmp);

				for (int i = 0; i < 3; ++i)
				{
					tmp[i] = Native2LE(tmp[i]);
				}
				os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
			}
			else
			{
				std::vector<int3> init_val;
				var.getValue(init_val);

				uint32_t len = static_cast<uint32_t>(init_val.size());
				len = Native2LE(len);
				os.write(reinterpret_cast<char const *>(&len), sizeof(len));
				if (len > 0)
				{
					for (uint32_t i = 0; i < len; ++i)
					{
						for (int j = 0; j < 3; ++j)
						{
							init_val[i][j] = Native2LE(init_val[i][j]);
						}
					}
					os.write(reinterpret_cast<char const *>(&init_val[0]), len * sizeof(init_val[0]));
				}
			}
			break;

		case REDT_uint4:
			if (0 == array_size)
			{
				uint4 tmp;
				var.getValue(tmp);

				for (int i = 0; i < 3; ++i)
				{
					tmp[i] = Native2LE(tmp[i]);
				}
				os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
			}
			else
			{
				std::vector<int4> init_val;
				var.getValue(init_val);

				uint32_t len = static_cast<uint32_t>(init_val.size());
				len = Native2LE(len);
				os.write(reinterpret_cast<char const *>(&len), sizeof(len));
				if (len > 0)
				{
					for (uint32_t i = 0; i < len; ++i)
					{
						for (int j = 0; j < 4; ++j)
						{
							init_val[i][j] = Native2LE(init_val[i][j]);
						}
					}
					os.write(reinterpret_cast<char const *>(&init_val[0]), len * sizeof(init_val[0]));
				}
			}
			break;

		case REDT_int2:
			if (0 == array_size)
			{
				int2 tmp;
				var.getValue(tmp);

				for (int i = 0; i < 2; ++i)
				{
					tmp[i] = Native2LE(tmp[i]);
				}
				os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
			}
			else
			{
				std::vector<int2> init_val;
				var.getValue(init_val);

				uint32_t len = static_cast<uint32_t>(init_val.size());
				len = Native2LE(len);
				os.write(reinterpret_cast<char const *>(&len), sizeof(len));
				if (len > 0)
				{
					for (uint32_t i = 0; i < len; ++i)
					{
						for (int j = 0; j < 2; ++j)
						{
							init_val[i][j] = Native2LE(init_val[i][j]);
						}
					}
					os.write(reinterpret_cast<char const *>(&init_val[0]), len * sizeof(init_val[0]));
				}
			}
			break;

		case REDT_int3:
			if (0 == array_size)
			{
				int3 tmp;
				var.getValue(tmp);

				for (int i = 0; i < 3; ++i)
				{
					tmp[i] = Native2LE(tmp[i]);
				}
				os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
			}
			else
			{
				std::vector<int3> init_val;
				var.getValue(init_val);

				uint32_t len = static_cast<uint32_t>(init_val.size());
				len = Native2LE(len);
				os.write(reinterpret_cast<char const *>(&len), sizeof(len));
				if (len > 0)
				{
					for (uint32_t i = 0; i < len; ++i)
					{
						for (int j = 0; j < 3; ++j)
						{
							init_val[i][j] = Native2LE(init_val[i][j]);
						}
					}
					os.write(reinterpret_cast<char const *>(&init_val[0]), len * sizeof(init_val[0]));
				}
			}
			break;

		case REDT_int4:
			if (0 == array_size)
			{
				int4 tmp;
				var.getValue(tmp);

				for (int i = 0; i < 4; ++i)
				{
					tmp[i] = Native2LE(tmp[i]);
				}
				os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
			}
			else
			{
				std::vector<int4> init_val;
				var.getValue(init_val);

				uint32_t len = static_cast<uint32_t>(init_val.size());
				len = Native2LE(len);
				os.write(reinterpret_cast<char const *>(&len), sizeof(len));
				if (len > 0)
				{
					for (uint32_t i = 0; i < len; ++i)
					{
						for (int j = 0; j < 4; ++j)
						{
							init_val[i][j] = Native2LE(init_val[i][j]);
						}
					}
					os.write(reinterpret_cast<char const *>(&init_val[0]), len * sizeof(init_val[0]));
				}
			}
			break;

		case REDT_float2:
			if (0 == array_size)
			{
				float2 tmp;
				var.getValue(tmp);

				for (int i = 0; i < 2; ++i)
				{
					tmp[i] = Native2LE(tmp[i]);
				}
				os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
			}
			else
			{
				std::vector<float2> init_val;
				var.getValue(init_val);

				uint32_t len = static_cast<uint32_t>(init_val.size());
				len = Native2LE(len);
				os.write(reinterpret_cast<char const *>(&len), sizeof(len));
				if (len > 0)
				{
					for (uint32_t i = 0; i < len; ++i)
					{
						for (int j = 0; j < 2; ++j)
						{
							init_val[i][j] = Native2LE(init_val[i][j]);
						}
					}
					os.write(reinterpret_cast<char const *>(&init_val[0]), len * sizeof(init_val[0]));
				}
			}
			break;

		case REDT_float3:
			if (0 == array_size)
			{
				float3 tmp;
				var.getValue(tmp);

				for (int i = 0; i < 3; ++i)
				{
					tmp[i] = Native2LE(tmp[i]);
				}
				os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
			}
			else
			{
				std::vector<float3> init_val;
				var.getValue(init_val);

				uint32_t len = static_cast<uint32_t>(init_val.size());
				len = Native2LE(len);
				os.write(reinterpret_cast<char const *>(&len), sizeof(len));
				if (len > 0)
				{
					for (uint32_t i = 0; i < len; ++i)
					{
						for (int j = 0; j < 3; ++j)
						{
							init_val[i][j] = Native2LE(init_val[i][j]);
						}
					}
					os.write(reinterpret_cast<char*>(&init_val[0]), len * sizeof(init_val[0]));
				}
			}
			break;

		case REDT_float4:
			if (0 == array_size)
			{
				float4 tmp;
				var.getValue(tmp);

				for (int i = 0; i < 4; ++i)
				{
					tmp[i] = Native2LE(tmp[i]);
				}
				os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
			}
			else
			{
				std::vector<float4> init_val;
				var.getValue(init_val);

				uint32_t len = static_cast<uint32_t>(init_val.size());
				len = Native2LE(len);
				os.write(reinterpret_cast<char const *>(&len), sizeof(len));
				if (len > 0)
				{
					for (uint32_t i = 0; i < len; ++i)
					{
						for (int j = 0; j < 4; ++j)
						{
							init_val[i][j] = Native2LE(init_val[i][j]);
						}
					}
					os.write(reinterpret_cast<char const *>(&init_val[0]), len * sizeof(init_val[0]));
				}
			}
			break;

		case REDT_float4x4:
			if (0 == array_size)
			{
				float4x4 tmp;
				var.getValue(tmp);

				for (int i = 0; i < 16; ++i)
				{
					tmp[i] = Native2LE(tmp[i]);
				}
				os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
			}
			else
			{
				std::vector<float4x4> init_val;
				var.getValue(init_val);

				uint32_t len = static_cast<uint32_t>(init_val.size());
				len = Native2LE(len);
				os.write(reinterpret_cast<char const *>(&len), sizeof(len));
				if (len > 0)
				{
					for (uint32_t i = 0; i < len; ++i)
					{
						for (int j = 0; j < 16; ++j)
						{
							init_val[i][j] = Native2LE(init_val[i][j]);
						}
					}
					os.write(reinterpret_cast<char const *>(&init_val[0]), len * sizeof(init_val[0]));
				}
			}
			break;

		case REDT_buffer:
		case REDT_structured_buffer:
		case REDT_rw_buffer:
		case REDT_rw_structured_buffer:
		case REDT_consume_structured_buffer:
		case REDT_append_structured_buffer:
		{
			std::string tmp;
			var.getValue(tmp);
			writeShortString(os, tmp);
		}
		break;

		case REDT_byte_address_buffer:
		case REDT_rw_byte_address_buffer:
			break;

		default:
			AIR_UNREACHABLE("Invalid type");
		}
	}
#endif

}


namespace Air
{
	class EffectLoadingDesc : public ResLoadingDesc
	{
	private:
		struct EffectDesc
		{
			std::string mResName;
			RenderEffectPtr mEffect;
		};
	public:
		explicit EffectLoadingDesc(std::string const & name)
		{
			mEffectDesc.mResName = name;
		}

		uint64_t getType() const override
		{
			static uint64_t const type = CT_HASH("EffectLoadingDesc");
			return type;
		}
		bool getStateLess() const override
		{
			return false;
		}

		void subThreadStage() override
		{

		}
		void mainThreadStage() override
		{
			mEffectDesc.mEffect = MakeSharedPtr<RenderEffect>();
			mEffectDesc.mEffect->load(mEffectDesc.mResName);
		}

		bool hasSubThreadStage() const override
		{
			return false;
		}

		bool match(ResLoadingDesc const & rhs) const
		{
			if (this->getType() == rhs.getType())
			{
				EffectLoadingDesc const & eld = static_cast<EffectLoadingDesc const &>(rhs);
				return (mEffectDesc.mResName == eld.mEffectDesc.mResName);
			}
			return false;
		}

		void copyDataFrom(ResLoadingDesc const & rhs)
		{
			BOOST_ASSERT(this->getType() == rhs.getType());
			EffectLoadingDesc const & eld = static_cast<EffectLoadingDesc const &>(rhs);
			mEffectDesc.mResName = eld.mEffectDesc.mResName;
		}

		std::shared_ptr<void> cloneResourceFrom(std::shared_ptr<void> const& resource)
		{
			mEffectDesc.mEffect = std::static_pointer_cast<RenderEffect>(resource)->clone();
			return std::static_pointer_cast<void>(mEffectDesc.mEffect);
		}
		std::shared_ptr<void> getResource() const override
		{
			return mEffectDesc.mEffect;
		}
	private:
		EffectDesc mEffectDesc;
	};


	RenderVariable::RenderVariable()
	{

	}
	RenderVariable::~RenderVariable()
	{

	}

	RenderVariable& RenderVariable::operator =(bool const &)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(uint32_t const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(int32_t const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(float const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(uint2 const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(uint3 const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(uint4 const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(int2 const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(int3 const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(int4 const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(float2 const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(float3 const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(float4 const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(float4x4 const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(TexturePtr const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(TextureSubresource const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(SamplerStateObjectPtr const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(GraphicsBufferPtr const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::string const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(ShaderDesc const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<bool> const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<uint32_t> const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<int32_t> const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<float> const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<uint2> const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<uint3> const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<uint4> const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<int2> const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<int3> const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<int4> const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<float2> const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<float3> const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}

	RenderVariable& RenderVariable::operator=(std::vector<float4> const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}


	RenderVariable& RenderVariable::operator=(std::vector<float4x4> const & /*value*/)
	{
		AIR_UNREACHABLE("Can't be called");
	}


	void RenderVariable::getValue(bool& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(uint32_t& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(int32_t& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(float& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(uint2& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(uint3& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(uint4& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(int2& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(int3& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(int4& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(float2& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(float3& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(float4& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}


	void RenderVariable::getValue(float4x4& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(TexturePtr& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(TextureSubresource& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(SamplerStateObjectPtr& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(GraphicsBufferPtr& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(std::string& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(ShaderDesc& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(std::vector<bool>& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(std::vector<uint32_t>& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(std::vector<int32_t>& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(std::vector<float>& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(std::vector<uint2>& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(std::vector<uint3>& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(std::vector<uint4>& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(std::vector<int2>& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(std::vector<int3>& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(std::vector<int4>& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(std::vector<float2>& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(std::vector<float3>& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(std::vector<float4>& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::getValue(std::vector<float4x4>& /*value*/) const
	{
		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::bindToCBuffer(RenderEffectConstantBuffer& cbuff, uint32_t offset, uint32_t stride)
	{
		AIR_UNUSED(cbuff);
		AIR_UNUSED(offset);
		AIR_UNUSED(stride);

		AIR_UNREACHABLE("Can't be called");
	}

	void RenderVariable::rebindToCBuffer(RenderEffectConstantBuffer& cbuff)
	{
		AIR_UNUSED(cbuff);

		AIR_UNREACHABLE("Can't be called");
	}

#if AIR_IS_DEV_PLATFORM
	void RenderTechnique::load(RenderEffect& effect, XMLNodePtr const & node, uint32_t tech_index)
	{
		mName = node->getAttrib("name")->getValueString();
		mNameHash = boost::hash_range(mName.begin(), mName.end());
		RenderTechnique* parent_tech = nullptr;
		XMLAttributePtr inherit_attr = node->getAttrib("inherit");
		if (inherit_attr)
		{
			std::string inherit = inherit_attr->getValueString();
			BOOST_ASSERT(inherit != mName);
			parent_tech = effect.getTechniqueByName(inherit);
			BOOST_ASSERT(parent_tech);
		}
		{
			XMLNodePtr anno_node = node->getFirstNode("annotation");
			if (anno_node)
			{
				mAnnotations = MakeSharedPtr < std::remove_reference<decltype(*mAnnotations)>::type>();
				if (parent_tech && parent_tech->mAnnotations)
				{
					*mAnnotations = *parent_tech->mAnnotations;
				}

				for (; anno_node; anno_node = anno_node->getNextSibling("annotation"))
				{
					RenderEffectAnnotationPtr annotation = MakeSharedPtr<RenderEffectAnnotation>();
					mAnnotations->push_back(annotation);
					annotation->load(anno_node);
				}
			}
			else if (parent_tech)
			{
				mAnnotations = parent_tech->mAnnotations;
			}
		}
		{
			XMLNodePtr macro_node = node->getFirstNode("macro");
			if (macro_node)
			{
				mMacros = MakeSharedPtr < std::remove_reference<decltype(*mMacros)>::type>();
				if (parent_tech && parent_tech->mMacros)
				{
					*mMacros = *parent_tech->mMacros;
					for (; macro_node; macro_node = macro_node->getNextSibling("macro"))
					{
						std::string name = macro_node->getAttribString("name");
						std::string value = macro_node->getAttribString("value");
						bool found = false;
						for (size_t i = 0; i < mMacros->size(); ++i)
						{
							if ((*mMacros)[i].first == name)
							{
								(*mMacros)[i].second = value;
								found = true;
								break;
							}
						}
						if (!found)
						{
							mMacros->emplace_back(name, value);
						}
					}
				}
				else if (parent_tech)
				{
					mMacros = parent_tech->mMacros;
				}
			}
			if (!node->getFirstNode("pass") && parent_tech)
			{
				mIsValidate = parent_tech->mIsValidate;
				mHasDiscard = parent_tech->mHasDiscard;
				mHasTessellation = parent_tech->mHasTessellation;
				mTransparent = parent_tech->mTransparent;
				mWeight = parent_tech->mWeight;
				if (mMacros == parent_tech->mMacros)
				{
					mPasses = parent_tech->mPasses;
				}
				else
				{
					for (uint32_t index = 0; index < parent_tech->mPasses.size(); ++index)
					{
						RenderPassPtr pass = MakeSharedPtr<RenderPass>();
						mPasses.push_back(pass);
						auto inherit_pass = parent_tech->mPasses[index].get();
						pass->load(effect, tech_index, index, inherit_pass);
						mIsValidate &= pass->isValidate();
					}
				}
			}
			else
			{
				mIsValidate = true;
				mHasDiscard = false;
				mHasTessellation = false;
				mTransparent = false;
				if (parent_tech)
				{
					mWeight = parent_tech->getWeight();
				}
				else
				{
					mWeight = 1;
				}
				uint32_t index = 0;
				for (XMLNodePtr pass_node = node->getFirstNode("pass"); pass_node; pass_node = pass_node->getNextSibling("pass"), ++index)
				{
					RenderPassPtr pass = MakeSharedPtr<RenderPass>();
					mPasses.push_back(pass);
					RenderPass* inherit_pass = nullptr;
					if (parent_tech && (index < parent_tech->mPasses.size()))
					{
						inherit_pass = parent_tech->mPasses[index].get();
					}
					pass->load(effect, pass_node, tech_index, index, inherit_pass);
					mIsValidate &= pass->isValidate();
					for (XMLNodePtr state_node = pass_node->getFirstNode("state"); state_node; state_node = state_node->getNextSibling("state"))
					{
						++mWeight;
						std::string state_name = state_node->getAttribString("name");
						if ("blend_enable" == state_name)
						{
							std::string value_str = state_node->getAttribString("value");
							if (BoolFromStr(value_str))
							{
								mTransparent = true;
							}
						}
					}
					mHasDiscard |= pass->getShaderObject(effect)->hasDiscard();
					mHasTessellation |= pass->getShaderObject(effect)->hasTessellation();
				}
				if (mTransparent)
				{
					mWeight += 10000;
				}
			}
		}
	}
#endif

#if AIR_IS_DEV_PLATFORM

	void RenderTechnique::streamOut(RenderEffect const & effect, std::ostream & os, uint32_t tech_index) const
	{
		writeShortString(os, mName);
		uint8_t num_anno;
		if (mAnnotations)
		{
			num_anno = static_cast<uint8_t>(mAnnotations->size());
		}
		else
		{
			num_anno = 0;
		}
		os.write(reinterpret_cast<char const*>(&num_anno), sizeof(num_anno));
		for (uint32_t i = 0; i < num_anno; ++i)
		{
			RenderEffectAnnotationPtr annotation = MakeSharedPtr<RenderEffectAnnotation>();
			annotation = (*mAnnotations)[i];
			annotation->streamOut(os);
		}
		uint8_t num_macro;
		if (mMacros)
		{
			num_macro = static_cast<uint8_t>(mMacros->size());
		}
		else
		{
			num_macro = 0;
		}
		os.write(reinterpret_cast<char const *>(&num_macro), sizeof(num_macro));
		for (uint32_t i = 0; i < num_macro; ++i)
		{
			writeShortString(os, (*mMacros)[i].first);
			writeShortString(os, (*mMacros)[i].second);
		}
		os.write(reinterpret_cast<char const *>(&mTransparent), sizeof(mTransparent));
		float w = Native2LE(mWeight);
		os.write(reinterpret_cast<char const *>(&w), sizeof(w));
		uint8_t num_passes = static_cast<uint8_t>(mPasses.size());
		os.write(reinterpret_cast<char const *>(&num_passes), sizeof(num_passes));
		for (uint32_t pass_index = 0; pass_index < num_passes; ++pass_index)
		{
			mPasses[pass_index]->streamOut(effect, os, tech_index, pass_index);
		}
	}
#endif
	bool RenderTechnique::streamIn(RenderEffect& effect, ResIdentifierPtr const & res, uint32_t tech_index)
	{
		mName = readShortString(res);
		mNameHash = boost::hash_range(mName.begin(), mName.end());
		uint8_t num_anno;
		res->read(&num_anno, sizeof(num_anno));
		if (num_anno > 0)
		{
			mAnnotations = MakeSharedPtr < std::remove_reference<decltype(*mAnnotations)>::type>();
			mAnnotations->resize(num_anno);
			for (uint32_t i = 0; i < num_anno; ++i)
			{
				RenderEffectAnnotationPtr annotation = MakeSharedPtr<RenderEffectAnnotation>();
				(*mAnnotations)[i] = annotation;
				annotation->streamIn(res);
			}
		}
		uint8_t num_macros;
		res->read(&num_macros, sizeof(num_macros));
		if (num_macros > 0)
		{
			mMacros = MakeSharedPtr <std::remove_reference<decltype(*mMacros)>::type>();
			mMacros->resize(num_macros);
			for (uint32_t i = 0; i < num_macros; ++i)
			{
				std::string name = readShortString(res);
				std::string value = readShortString(res);
				(*mMacros)[i] = std::make_pair(name, value);
			}
		}
		mIsValidate = true;
		mHasDiscard = false;
		mHasTessellation = false;
		res->read(&mTransparent, sizeof(mTransparent));
		res->read(&mWeight, sizeof(mWeight));
		mWeight = LE2Native(mWeight);
		bool ret = true;
		uint8_t num_passes;
		res->read(&num_passes, sizeof(num_passes));
		mPasses.resize(num_passes);
		for (uint32_t pass_index = 0; pass_index < num_passes; ++pass_index)
		{
			RenderPassPtr pass = MakeSharedPtr<RenderPass>();
			mPasses[pass_index] = pass;
			ret &= pass->streamIn(effect, res, tech_index, pass_index);
			mIsValidate &= pass->isValidate();
			mHasDiscard |= pass->getShaderObject(effect)->hasDiscard();
			mHasTessellation |= pass->getShaderObject(effect)->hasTessellation();
		}
		return ret;
	}
	
	float  RenderTechnique::getWeight() const
	{
		return mWeight;
	}

	size_t RenderTechnique::getNameHash() const
	{
		return mNameHash;
	}


	RenderEffectConstantBuffer::RenderEffectConstantBuffer() : mIsDirty(true)
	{
		static int x = 0;
		x++;
		id = x;
	}
#if AIR_IS_DEV_PLATFORM
	void RenderEffectConstantBuffer::load(std::string const & name, ConstantBufferType type)
	{
		mName = MakeSharedPtr<std::remove_reference<decltype(*mName)>::type>();
		mName->first = name;
		mName->second = boost::hash_range(mName->first.begin(), mName->first.end());
		mParamIndices = MakeSharedPtr<std::remove_reference<decltype(*mParamIndices)>::type>();
		mType = type;
	}
#endif
	void RenderEffectConstantBuffer::streamIn(ResIdentifierPtr const & res)
	{
		mName = MakeSharedPtr<std::remove_reference<decltype(*mName)>::type>();
		mName->first = readShortString(res);
		mName->second = boost::hash_range(mName->first.begin(), mName->first.end());
		mParamIndices = MakeSharedPtr < std::remove_reference<decltype(*mParamIndices)>::type>();
		uint16_t len;
		res->read(&len, sizeof(len));
		len = LE2Native(len);
		mParamIndices->resize(len);
		res->read(&(*mParamIndices)[0], len * sizeof((*mParamIndices)[0]));
		for (uint32_t i = 0; i < len; i++)
		{
			(*mParamIndices)[i] = LE2Native((*mParamIndices)[i]);
		}
	}

#if AIR_IS_DEV_PLATFORM
	void RenderEffectConstantBuffer::streamOut(std::ostream& os) const
	{
		writeShortString(os, mName->first);
		uint16_t len = Native2LE(static_cast<uint16_t>(mParamIndices->size()));
		os.write(reinterpret_cast<char const *>(&len), sizeof(len));
		for (size_t i = 0; i < mParamIndices->size(); ++i)
		{
			uint32_t tmp = Native2LE((*mParamIndices)[i]);
			os.write(reinterpret_cast<char const*>(&tmp), sizeof(tmp));
		}
	}
#endif



	std::shared_ptr<RenderEffectConstantBuffer> RenderEffectConstantBuffer::clone(RenderEffect& src_effect, RenderEffect& dst_effect)
	{
		auto ret = MakeSharedPtr<RenderEffectConstantBuffer>();
		ret->mName = mName;
		ret->mParamIndices = mParamIndices;
		ret->mBuffer = mBuffer;
		ret->resize(static_cast<uint32_t>(mBuffer.size()));
		for (size_t i = 0; i < mParamIndices->size(); ++i)
		{
			RenderEffectParameter* src_param = src_effect.getParameterByIndex((*mParamIndices)[i]);
			if (src_param->inCBuffer())
			{
				RenderEffectParameter* dst_param = dst_effect.getParameterByIndex((*mParamIndices)[i]);
				dst_param->rebindToCBuffer(*ret);
			}
		}
		return ret;
	}

	void RenderEffect::load(std::string const & name)
	{
		mEffectTemplate = MakeSharedPtr<RenderEffectTemplate>();
		mEffectTemplate->load(name, *this);
	}

	RenderEffectPtr RenderEffect::clone()
	{
		auto ret = MakeSharedPtr<RenderEffect>();
		ret->isClone = true;
		ret->mEffectTemplate = mEffectTemplate;
		ret->mParams.resize(mParams.size());
		for (size_t i = 0; i < mParams.size(); ++i)
		{
			ret->mParams[i] = mParams[i]->clone();
		}
		ret->mCbuffers.resize(mCbuffers.size());
		for (size_t i = 0; i < mCbuffers.size(); ++i)
		{
			if (mCbuffers[i]->getType() == CBT_Object)
			{
				ret->mCbuffers[i] = mCbuffers[i]->clone(*this, *ret);
			}
			else
			{
				ret->mCbuffers[i] = mCbuffers[i];
			}
		}
		ret->mShaderObjs.resize(mShaderObjs.size());
		for (size_t i = 0; i < mShaderObjs.size(); ++i)
		{
			ret->mShaderObjs[i] = mShaderObjs[i]->clone(*ret);
		}
		return ret;
	}
	std::string const & RenderEffect::getResName() const
	{
		return mEffectTemplate->getResourceName();
	}
	size_t RenderEffect::getResNameHash() const
	{
		return mEffectTemplate->getResourceNameHash();
	}
	uint32_t RenderEffect::getNumParameters() const
	{
		return static_cast<uint32_t>(mParams.size());
	}
	RenderEffectParameter* RenderEffect::getParameterBySemantic(std::string_view semantic) const
	{
		size_t const semantic_hash = boost::hash_range(semantic.begin(), semantic.end());
		for (auto const & param : mParams)
		{
			if (semantic_hash == param->getSemanticHash())
			{
				return param.get();
			}
		}
		return nullptr;
	}
	RenderEffectParameter* RenderEffect::getParameterByName(std::string_view name) const
	{
		size_t const name_hash = boost::hash_range(name.begin(), name.end());
		for (auto const & param : mParams)
		{
			if (param->getNameHash() == name_hash)
			{
				return param.get();
			}
		}
		return nullptr;
	}
	RenderEffectParameter* RenderEffect::getParameterByIndex(uint32_t n) const
	{
		BOOST_ASSERT(n < this->getNumParameters());
		return mParams[n].get();
	}

	uint32_t RenderEffect::getNumCBuffers() const
	{
		return static_cast<uint32_t>(mCbuffers.size());
	}

	RenderEffectConstantBuffer* RenderEffect::getCBufferByName(std::string_view name) const
	{
		size_t name_hash = boost::hash_range(name.begin(), name.end());
		for (auto const &cb : mCbuffers)
		{
			if (cb->getNameHash() == name_hash)
			{
				return cb.get();
			}
		}
		return nullptr;
	}
	RenderEffectConstantBuffer* RenderEffect::getCBufferByIndex(uint32_t n) const
	{
		BOOST_ASSERT(n < getNumCBuffers());
		return mCbuffers[n].get();
	}
	uint32_t RenderEffect::getNumTechniques() const
	{
		return mEffectTemplate->getNumTechniques();
	}

	RenderTechnique* RenderEffect::getTechniqueByName(std::string_view name) const
	{
		return mEffectTemplate->getTechniqueByName(name);
	}
	RenderTechnique* RenderEffect::getTechniqueByIndex(uint32_t n) const
	{
		return mEffectTemplate->getTechniqueByIndex(n);
	}
	RenderShaderFragment const & RenderEffect::getShaderFragmentByIndex(uint32_t n) const
	{
		return mEffectTemplate->getShaderFragmentByIndex(n);
	}
	uint32_t RenderEffect::addShaderDesc(ShaderDesc const & sd)
	{
		return mEffectTemplate->addShaderDesc(sd);
	}
	ShaderDesc& RenderEffect::getShaderDesc(uint32_t id)
	{
		return mEffectTemplate->getShaderDesc(id);
	}
	ShaderDesc const & RenderEffect::getShaderDesc(uint32_t id) const
	{
		return mEffectTemplate->getShaderDesc(id);
	}

	uint32_t RenderEffect::addShaderObject()
	{
		uint32_t index = static_cast<uint32_t>(mShaderObjs.size());
		mShaderObjs.push_back(SingletonManager::getRenderFactoryInstance().makeShaderObject());
		return index;
	}

	uint32_t RenderEffect::getNumMacros() const
	{
		return mEffectTemplate->getNumMacros();
	}


	std::pair<std::string, std::string> const & RenderEffect::getMacroByIndex(uint32_t n) const
	{
		return mEffectTemplate->getMacroByIndex(n);
	}

	std::string const & RenderEffect::getTypeName(uint32_t code) const
	{
		return mEffectTemplate->getTypeName(code);
	}

	std::string const & RenderEffect::getHLSHShader() const
	{
		//logWarn("hlsl:\n%s",mEffectTemplate->getHLSLShaderText().c_str());
		return mEffectTemplate->getHLSLShaderText();
	}

	bool RenderEffectTemplate::streamIn(ResIdentifierPtr const & source, RenderEffect& effect)
	{
		RenderEngine& re = SingletonManager::getRenderFactoryInstance().getRenderEngineInstance();
		bool ret = false;
		if (source)
		{
			uint32_t fourcc;
			source->read(&fourcc, sizeof(fourcc));
			fourcc = LE2Native(fourcc);
			uint32_t ver;
			source->read(&ver, sizeof(ver));
			ver = LE2Native(ver);
			if ((MakeFourCC<'K', 'F', 'X', ' '>::value == fourcc) && (AIR_VERSION == ver))
			{
				uint32_t shader_fourcc;
				source->read(&shader_fourcc, sizeof(shader_fourcc));
				shader_fourcc = LE2Native(shader_fourcc);

				uint32_t shader_ver;
				source->read(&shader_ver, sizeof(shader_ver));
				shader_ver = LE2Native(shader_ver);

				uint8_t shader_platform_name_len;
				source->read(&shader_platform_name_len, sizeof(shader_platform_name_len));
				std::string shader_platform_name(shader_platform_name_len, 0);
				source->read(&shader_platform_name[0], shader_platform_name_len);

				if ((re.getNativeShaderFourCC() == shader_fourcc) && (re.getNativeShaderdVersion() == shader_ver) && (re.getNativeShaderPlatformName() == shader_platform_name))
				{
					uint64_t timestamp;
					source->read(&timestamp, sizeof(timestamp));
#if AIR_IS_DEV_PLATFORM
					timestamp = LE2Native(timestamp);
					if (mTimestamp <= timestamp)
#endif
					{
						mShaderDescs.resize(1);
						uint16_t num_macros;
						source->read(&num_macros, sizeof(num_macros));
						num_macros = LE2Native(num_macros);
						if (num_macros > 0)
						{
							mMacros = MakeSharedPtr<std::remove_reference<decltype(*mMacros)>::type>();
						}
						for (uint32_t i = 0; i < num_macros; ++i)
						{
							std::string name = readShortString(source);
							std::string value = readShortString(source);
							mMacros->emplace_back(std::make_pair(name, value), true);
						}

					}
					{
						uint16_t num_cbufs;
						source->read(&num_cbufs, sizeof(num_cbufs));
						num_cbufs = LE2Native(num_cbufs);
						effect.mCbuffers.resize(num_cbufs);
						for (uint32_t i = 0; i < num_cbufs; ++i)
						{
							effect.mCbuffers[i] = MakeSharedPtr<RenderEffectConstantBuffer>();
							effect.mCbuffers[i]->streamIn(source);
						}
					}
					{
						uint16_t num_params;
						source->read(&num_params, sizeof(num_params));
						num_params = LE2Native(num_params);
						effect.mParams.resize(num_params);
						for (uint16_t i = 0; i < num_params; ++i)
						{
							effect.mParams[i] = MakeUniquePtr<RenderEffectParameter>();
							effect.mParams[i]->streamIn(source);
						}
					}
					{
						uint16_t num_shader_frags;
						source->read(&num_shader_frags, sizeof(num_shader_frags));
						num_shader_frags = LE2Native(num_shader_frags);
						if (num_shader_frags > 0)
						{
							mShaderFrags.resize(num_shader_frags);
							for (uint16_t i = 0; i < num_shader_frags; ++i)
							{
								mShaderFrags[i].streamIn(source);
							}
						}
					}
					{
						uint16_t num_shader_desc;
						source->read(&num_shader_desc, sizeof(num_shader_desc));
						num_shader_desc = LE2Native(num_shader_desc);
						mShaderDescs.resize(num_shader_desc + 1);
						for (uint16_t i = 0; i < num_shader_desc; ++i)
						{
							mShaderDescs[i + 1].mProfile = readShortString(source);
							mShaderDescs[i + 1].mFunctionName = readShortString(source);
							source->read(&mShaderDescs[i + 1].mMacrosHash, sizeof(mShaderDescs[i + 1].mMacrosHash));

							source->read(&mShaderDescs[i + 1].mTechPassType, sizeof(mShaderDescs[i + 1].mTechPassType));

							mShaderDescs[i + 1].mTechPassType = LE2Native(mShaderDescs[i + 1].mTechPassType);
							uint8_t len;
							source->read(&len, sizeof(len));
							if (len > 0)
							{
								mShaderDescs[i + 1].mSODecl.resize(len);
								source->read(&mShaderDescs[i + 1].mSODecl[0], len * sizeof(mShaderDescs[i + 1].mSODecl[0]));
								for (uint32_t j = 0; j < len; ++j)
								{
									mShaderDescs[i + 1].mSODecl[j].mUsage = LE2Native(mShaderDescs[i + 1].mSODecl[j].mUsage);
								}
							}
						}
					}
					ret = true;
					{
						uint16_t num_techs;
						source->read(&num_techs, sizeof(num_techs));
						num_techs = LE2Native(num_techs);
						mTechniques.resize(num_techs);
						for (uint16_t i = 0; i < num_techs; ++i)
						{
							mTechniques[i] = MakeUniquePtr<RenderTechnique>();
							ret &= mTechniques[i]->streamIn(effect, source, i);
						}
					}
				}
			}
		}
		return ret;
	}

#if AIR_IS_DEV_PLATFORM
	void RenderEffectTemplate::streamOut(std::ostream& os, RenderEffect const & effect)
		const
	{
		RenderEngine& re = SingletonManager::getRenderFactoryInstance().getRenderEngineInstance();
		uint32_t fourcc = Native2LE(MakeFourCC<'K', 'F', 'X', ' '>::value);
		os.write(reinterpret_cast<char const *>(&fourcc), sizeof(fourcc));

		uint32_t ver = Native2LE(AIR_VERSION);
		os.write(reinterpret_cast<char const *>(&ver), sizeof(ver));

		uint32_t shader_fourcc = Native2LE(re.getNativeShaderFourCC());
		os.write(reinterpret_cast<char const *>(&shader_fourcc), sizeof(shader_fourcc));

		uint32_t shader_ver = Native2LE(re.getNativeShaderdVersion());
		os.write(reinterpret_cast<char const *>(&shader_ver), sizeof(shader_ver));

		uint8_t shader_platform_name_len = static_cast<uint8_t>(re.getNativeShaderPlatformName().size());
		os.write(reinterpret_cast<char const *>(&shader_platform_name_len), sizeof(shader_platform_name_len));
		os.write(&re.getNativeShaderPlatformName()[0], shader_platform_name_len);

		uint64_t timestamp = Native2LE(mTimestamp);
		os.write(reinterpret_cast<char const *>(&timestamp), sizeof(timestamp));

		{
			uint16_t num_macros = 0;
			if (mMacros)
			{
				for (uint32_t i = 0; i < mMacros->size(); ++i)
				{
					if ((*mMacros)[i].second)
					{
						++num_macros;
					}
				}
			}

			num_macros = Native2LE(num_macros);
			os.write(reinterpret_cast<char const *>(&num_macros), sizeof(num_macros));

			if (mMacros)
			{
				for (uint32_t i = 0; i < mMacros->size(); ++i)
				{
					if ((*mMacros)[i].second)
					{
						writeShortString(os, (*mMacros)[i].first.first);
						writeShortString(os, (*mMacros)[i].first.second);
					}
				}
			}
		}

		{
			uint16_t num_cbufs = Native2LE(static_cast<uint16_t>(effect.mCbuffers.size()));
			os.write(reinterpret_cast<char const *>(&num_cbufs), sizeof(num_cbufs));
			for (uint32_t i = 0; i < effect.mCbuffers.size(); ++i)
			{
				effect.mCbuffers[i]->streamOut(os);
			}
		}

		{
			uint16_t num_params = Native2LE(static_cast<uint16_t>(effect.mParams.size()));
			os.write(reinterpret_cast<char const *>(&num_params), sizeof(num_params));
			for (uint32_t i = 0; i < effect.mParams.size(); ++i)
			{
				effect.mParams[i]->streamOut(os);
			}
		}

		{
			uint16_t num_shader_frags = Native2LE(static_cast<uint16_t>(mShaderFrags.size()));
			os.write(reinterpret_cast<char const *>(&num_shader_frags), sizeof(num_shader_frags));
			for (uint32_t i = 0; i < mShaderFrags.size(); ++i)
			{
				mShaderFrags[i].streamOut(os);
			}
		}

		{
			uint16_t num_shader_descs = Native2LE(static_cast<uint16_t>(mShaderDescs.size() - 1));
			os.write(reinterpret_cast<char const *>(&num_shader_descs), sizeof(num_shader_descs));
			for (uint32_t i = 0; i < mShaderDescs.size() - 1; ++i)
			{
				writeShortString(os, mShaderDescs[i + 1].mProfile);
				writeShortString(os, mShaderDescs[i + 1].mFunctionName);

				uint64_t tmp64 = Native2LE(mShaderDescs[i + 1].mMacrosHash);
				os.write(reinterpret_cast<char const *>(&tmp64), sizeof(tmp64));

				uint32_t tmp32 = Native2LE(mShaderDescs[i + 1].mTechPassType);
				os.write(reinterpret_cast<char const *>(&tmp32), sizeof(tmp32));

				uint8_t len = static_cast<uint8_t>(mShaderDescs[i + 1].mSODecl.size());
				os.write(reinterpret_cast<char const *>(&len), sizeof(len));
				for (uint32_t j = 0; j < len; ++j)
				{
					ShaderDesc::StreamOutputDecl so_decl = mShaderDescs[i + 1].mSODecl[j];
					so_decl.mUsage = Native2LE(so_decl.mUsage);
					os.write(reinterpret_cast<char const *>(&so_decl), sizeof(so_decl));
				}
			}
		}

		{
			uint16_t num_techs = Native2LE(static_cast<uint16_t>(mTechniques.size()));
			os.write(reinterpret_cast<char const *>(&num_techs), sizeof(num_techs));
			for (uint32_t i = 0; i < mTechniques.size(); ++i)
			{
				mTechniques[i]->streamOut(effect, os, i);
			}
		}

	}
#endif

	uint32_t RenderEffectTemplate::addShaderDesc(ShaderDesc const &sd)
	{
		for (uint32_t i = 0; i < mShaderDescs.size(); ++i)
		{
			if (mShaderDescs[i] == sd)
			{
				return i;
			}
		}
		uint32_t id = static_cast<uint32_t>(mShaderDescs.size());
		mShaderDescs.push_back(sd);
		return id;
	}
	ShaderDesc& RenderEffectTemplate::getShaderDesc(uint32_t id)
	{
		BOOST_ASSERT(id < mShaderDescs.size());
		return mShaderDescs[id];
	}
	ShaderDesc const & RenderEffectTemplate::getShaderDesc(uint32_t id) const
	{
		BOOST_ASSERT(id < mShaderDescs.size());
		return mShaderDescs[id];
	}
	uint32_t RenderEffectTemplate::getNumMacros() const
	{
		return mMacros ? static_cast<uint32_t>(mMacros->size()) : 0;
	}
	std::pair<std::string, std::string> const & RenderEffectTemplate::getMacroByIndex(uint32_t n) const
	{
		BOOST_ASSERT(n < this->getNumMacros());
		return (*mMacros)[n].first;
	}
	std::string const & RenderEffectTemplate::getTypeName(uint32_t code) const
	{
		return type_define::getInstance().getTypeName(code);
	}

#if AIR_IS_DEV_PLATFORM
	void RenderEffectTemplate::recursiveIncludeNode(XMLNode const & root, std::vector<std::string>& include_names) const
	{
		for (XMLNodePtr node = root.getFirstNode("include"); node; node = node->getNextSibling("include"))
		{
			XMLAttributePtr attr = node->getAttrib("name");
			BOOST_ASSERT(attr);
			std::string include_name = attr->getValueString();
			
			bool found = false;
			for (size_t i = 0; i < include_names.size(); ++i)
			{
				if (include_name == include_names[i])
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				include_names.push_back(include_name);
				XMLDocument include_doc;
				XMLNodePtr include_root = include_doc.parse(ResLoader::getInstance().open(include_name));
				this->recursiveIncludeNode(*include_root, include_names);
			}

		}
	}
	void RenderEffectTemplate::insertIncludeNodes(XMLDocument& target_doc, XMLNode& target_root, XMLNodePtr const & target_place, XMLNode const & include_root) const
	{
		for (XMLNodePtr child_node = include_root.getFirstNode(); child_node; child_node = child_node->getNextSibling())
		{
			if ((XNT_Element == child_node->getType()) && (child_node->getName() != "include"))
			{
				target_root.insertNode(target_place, target_doc.cloneNode(child_node));
			}
		}
	}
#endif


	RenderTechnique* RenderEffectTemplate::getTechniqueByName(std::string_view name) const
	{
		size_t const name_hash = boost::hash_range(name.begin(), name.end());
		for (auto const & tech : mTechniques)
		{
			if (name_hash == tech->getNameHash())
			{
				return tech.get();
			}
		}
		return nullptr;
	}

	RenderTechnique* RenderEffectTemplate::getTechniqueByIndex(uint32_t index) const
	{
		BOOST_ASSERT(index < this->getNumTechniques());
		return mTechniques[index].get();
	}

	void RenderEffectTemplate::load(std::string const & name, RenderEffect& effect)
	{
		RenderEnvironment & env = SingletonManager::getRenderFactoryInstance().getRenderEngineInstance().getRenderEnvironment();
		std::string fxml_name = ResLoader::getInstance().locate(name);
		if (fxml_name.empty())
		{
			fxml_name = name;
		}
		std::string kfx_name = fxml_name.substr(0, fxml_name.rfind(".")) + ".assets";
#if AIR_IS_DEV_PLATFORM
		ResIdentifierPtr source = ResLoader::getInstance().open(fxml_name);
#endif
		ResIdentifierPtr kfx_source = ResLoader::getInstance().open(kfx_name);
#if AIR_IS_DEV_PLATFORM
		std::unique_ptr<XMLDocument> doc;
		XMLNodePtr root;
#endif
		mResName = fxml_name;
		mResNameHash = boost::hash_range(fxml_name.begin(), fxml_name.end());
#if AIR_IS_DEV_PLATFORM
		if (source)
		{
			mTimestamp = source->getTimestamp();
			doc = MakeUniquePtr<XMLDocument>();
			root = doc->parse(source);
			std::vector<std::string> include_names;
			this->recursiveIncludeNode(*root, include_names);
			for (auto const & include_name : include_names)
			{
				ResIdentifierPtr include_source = ResLoader::getInstance().open(include_name);
				if (include_source)
				{
					mTimestamp = std::max(mTimestamp, include_source->getTimestamp());
				}
			}
		}
		else
		{
			mTimestamp = 0;
		}
#endif
		if (!this->streamIn(kfx_source, effect))
		{
#if AIR_IS_DEV_PLATFORM
			if (source)
			{
				effect.mParams.clear();
				effect.mCbuffers.clear();
				effect.mSharedBuffers.clear();
				effect.mShaderObjs.clear();
				mMacros.reset();
				mShaderFrags.clear();
				mHLSLShader.clear();
				mTechniques.clear();

				mShaderDescs.resize(1);
				XMLAttributePtr attr;
				std::vector<std::unique_ptr<XMLDocument>> include_docs;
				std::vector<std::string> whole_include_names;
				for (XMLNodePtr node = root->getFirstNode("include");
					node;)
				{
					attr = node->getAttrib("name");
					BOOST_ASSERT(attr);
					std::string include_name = attr->getValueString();
					include_docs.push_back(MakeUniquePtr<XMLDocument>());
					XMLNodePtr include_root = include_docs.back()->parse(ResLoader::getInstance().open(include_name));
					std::vector<std::string> include_names;
					this->recursiveIncludeNode(*include_root, include_names);
					if (!include_names.empty())
					{
						for (auto iter = include_names.begin(); iter != include_names.end();)
						{
							bool found = false;
							for (auto iter_w = whole_include_names.begin(); iter_w != whole_include_names.end(); ++iter_w)
							{
								if (*iter == *iter_w)
								{
									found = true;
									break;
								}
							}
							if (found)
							{
								iter = include_names.erase(iter);
							}
							else
							{
								include_docs.push_back(MakeUniquePtr<XMLDocument>());
								XMLNodePtr recursive_include_root = include_docs.back()->parse(ResLoader::getInstance().open(*iter));
								this->insertIncludeNodes(*doc, *root, node, *recursive_include_root);
								whole_include_names.push_back(*iter);
								++iter;
							}
						}
					}
					bool found = false;
					for (auto iter_w = whole_include_names.begin(); iter_w != whole_include_names.end(); ++iter_w)
					{
						if (include_name == *iter_w)
						{
							found = true;
							break;
						}
					}
					if (!found)
					{
						this->insertIncludeNodes(*doc, *root, node, *include_root);
						whole_include_names.push_back(include_name);
					}
					XMLNodePtr node_next = node->getNextSibling("include");
					root->removeNode(node);
					node = node_next;
				}
				{
					XMLNodePtr macro_node = root->getFirstNode("macro");
					if (macro_node)
					{
						mMacros = MakeSharedPtr<std::remove_reference<decltype(*mMacros)>::type>();
					}
					for (; macro_node; macro_node = macro_node->getNextSibling("macro"))
					{
						mMacros->emplace_back(std::make_pair(macro_node->getAttrib("name")->getValueString(), macro_node->getAttrib("value")->getValueString()), true);
					}
				}
				std::vector<XMLNodePtr> parameter_nodes;
				bool global_cb_created = false;
				for (XMLNodePtr node = root->getFirstNode(); node; node = node->getNextSibling())
				{
					if ("parameter" == node->getName())
					{

						parameter_nodes.push_back(node);
						if (!global_cb_created)
						{
							effect.mCbuffers.push_back(MakeSharedPtr<RenderEffectConstantBuffer>());
							RenderEffectConstantBuffer* cbuff = effect.mCbuffers.back().get();
							cbuff->load("global_cb", CBT_Object);
							global_cb_created = true;
						}
					}
					else if ("cbuffer" == node->getName())
					{
						std::string cbuff_name = node->getAttribString("name");
						ConstantBufferType cbt = constant_buffer_type_define::getInstance().fromStr(node->getAttribString("type", "CBT_Object"));
						

						if (cbt == CBT_Object)
						{
							effect.mCbuffers.push_back(MakeSharedPtr<RenderEffectConstantBuffer>());
							RenderEffectConstantBuffer* cbuff = effect.mCbuffers.back().get();
							cbuff->load(cbuff_name, cbt);

							for (XMLNodePtr sub_node = node->getFirstNode("parameter"); sub_node; sub_node = sub_node->getNextSibling("parameter"))
							{
								parameter_nodes.push_back(sub_node);
							}
						}
						else
						{
							SharedConstantBuffer* shared_buffer = env.getConstantBuffer(cbuff_name);
							if (!shared_buffer)
							{
								shared_buffer = env.addConstantBuffer(cbuff_name, cbt);
								shared_buffer->mCBuffer->load(cbuff_name, cbt);
								for (XMLNodePtr sub_node = node->getFirstNode("parameter"); sub_node; sub_node = sub_node->getNextSibling("parameter"))
								{
									shared_buffer->mParams.push_back(MakeUniquePtr<RenderEffectParameter>());
									shared_buffer->mParams.back()->load(sub_node);
								}
							}
							effect.mCbuffers.push_back(shared_buffer->getCBuffer());
							effect.mSharedBuffers.push_back(shared_buffer);
						}
					}
				}
				for (uint32_t param_index = 0; param_index < parameter_nodes.size(); ++param_index)
				{
					XMLNodePtr const & node = parameter_nodes[param_index];
					uint32_t type = type_define::getInstance().getTypeCode(node->getAttrib("type")->getValueString());
					if ((type != REDT_sampler) && (type != REDT_texture1D) && (type != REDT_texture2D) && (type != REDT_texture3D) && (type != REDT_textureCUBE) && (type != REDT_texture1DArray) && (type != REDT_texture2DArray) && (type != REDT_texture3DArray) && (type != REDT_textureCUBEArray) && (type != REDT_structured_buffer) && (type != REDT_byte_address_buffer) && (type != REDT_rw_buffer)
						&& (type != REDT_rw_structured_buffer) && (type != REDT_rw_texture1D)
						&& (type != REDT_rw_texture2D) && (type != REDT_rw_texture3D)
						&& (type != REDT_rw_texture1DArray) && (type != REDT_rw_texture2DArray)
						&& (type != REDT_rw_byte_address_buffer) && (type != REDT_append_structured_buffer)
						&& (type != REDT_consume_structured_buffer))
					{
						RenderEffectConstantBuffer* cbuff = nullptr;
						XMLNodePtr parent_node = node->getParent();
						std::string cbuff_name = parent_node->getAttribString("name", "global_cb");
				
						size_t const cbuff_name_hash = RT_HASH(cbuff_name.c_str());
						for (size_t i = 0; i < effect.mCbuffers.size(); i++)
						{
							if (effect.mCbuffers[i]->getNameHash() == cbuff_name_hash)
							{
								cbuff = effect.mCbuffers[i].get();
								break;
							}
						}
						BOOST_ASSERT(cbuff);
						cbuff->addParameters(param_index);
					}
					effect.mParams.push_back(MakeUniquePtr<RenderEffectParameter>());
					effect.mParams.back()->load(node);
				}
				for (XMLNodePtr shader_node = root->getFirstNode("shader"); shader_node; shader_node = shader_node->getNextSibling("shader"))
				{
					mShaderFrags.push_back(RenderShaderFragment());
					mShaderFrags.back().load(shader_node);
				}
				this->GenHLSLShaderText(effect);
				uint32_t index = 0;
				for (XMLNodePtr node = root->getFirstNode("technique"); node; node = node->getNextSibling("technique"), ++index)
				{
					mTechniques.push_back(MakeUniquePtr<RenderTechnique>());
					mTechniques.back()->load(effect, node, index);
				}
			}
			/*std::ofstream ofs(kfx_name.c_str(), std::ios_base::binary | std::ios_base::out);
			this->streamOut(ofs, effect);*/
#endif
		}

	}

#if AIR_IS_DEV_PLATFORM
	void RenderEffectTemplate::GenHLSLShaderText(RenderEffect const & effect)
	{
		std::string& str = mHLSLShader;
		str += "#define SHADER_MODEL(major, minor) ((major) * 4 + (minor))\n\n";
		for (uint32_t i = 0; i < this->getNumMacros(); ++i)
		{
			std::pair<std::string, std::string> const & name_value = this->getMacroByIndex(i);
			str += "#define " + name_value.first + " " + name_value.second + "\n";
		}
		str += '\n';

		auto genCbufferTex = [&str, this](RenderEffectParameter const & param){
			switch (param.getType())
			{
			case REDT_texture1D:
			case REDT_texture2D:
			case REDT_texture3D:
			case REDT_textureCUBE:
			case REDT_texture1DArray:
			case REDT_texture2DArray:
			case REDT_texture3DArray:
			case REDT_textureCUBEArray:
			case REDT_sampler:
			case REDT_buffer:
			case REDT_structured_buffer:
			case REDT_byte_address_buffer:
			case REDT_rw_buffer:
			case REDT_rw_structured_buffer:
			case REDT_rw_texture1D:
			case REDT_rw_texture2D:
			case REDT_rw_texture3D:
			case REDT_rw_texture1DArray:
			case REDT_rw_texture2DArray:
			case REDT_rw_byte_address_buffer:
			case REDT_append_structured_buffer:
			case REDT_consume_structured_buffer:
				break;
			default:
				str += this->getTypeName(param.getType()) + " " + param.getName();
				if (param.getArraySize())
				{
					str += "[" + *param.getArraySize() + "]";
				}
				str += ";\n";
				break;
			}
		};

		for (uint32_t i = 0; i < effect.mSharedBuffers.size(); ++i)
		{
			SharedConstantBuffer* shared_buffer = effect.mSharedBuffers[i];
			str += "cbuffer " + shared_buffer->getCBuffer()->getName() + "\n";
			str += "{\n";
			for (uint32_t j = 0; j < shared_buffer->mParams.size(); ++j)
			{
				RenderEffectParameter const & param = *shared_buffer->mParams[j];
				genCbufferTex(param);
			}
			str += "};\n";
		}

		for (uint32_t i = 0; i < effect.getNumCBuffers(); ++i)
		{
			RenderEffectConstantBuffer const & cbuff = *effect.getCBufferByIndex(i);
			if (cbuff.getType() != CBT_Object)
			{
				continue;
			}
			str += "cbuffer " + cbuff.getName() + "\n";
			str += "{\n";
			for (uint32_t j = 0; j < cbuff.getNumParameters(); ++j)
			{
				RenderEffectParameter const & param = *effect.getParameterByIndex(cbuff.getParametersIndex(j));
				genCbufferTex(param);
			}
			str += "};\n";
		}
		for (uint32_t i = 0; i < effect.getNumParameters(); ++i)
		{
			RenderEffectParameter& param = *effect.getParameterByIndex(i);
			std::string elem_type;
			switch (param.getType())
			{
			case REDT_texture1D:
			case REDT_texture2D:
			case REDT_texture3D:
			case REDT_textureCUBE:
			case REDT_texture1DArray:
			case REDT_texture2DArray:
			case REDT_textureCUBEArray:
			case REDT_buffer:
			case REDT_structured_buffer:
			case REDT_rw_buffer:
			case REDT_rw_structured_buffer:
			case REDT_rw_texture1D:
			case REDT_rw_texture2D:
			case REDT_rw_texture3D:
			case REDT_rw_texture1DArray:
			case REDT_rw_texture2DArray:
			case REDT_append_structured_buffer:
			case REDT_consume_structured_buffer:
				param.getVar().getValue(elem_type);
				break;
			default:
				break;
			}
			std::string const &param_name = param.getName();
			switch (param.getType())
			{
			case REDT_texture1D:
				str += "Texture1D<" + elem_type + "> " + param_name + ";\n";
				break;
			case REDT_texture2D:
				str += "Texture2D<" + elem_type + "> " + param_name + ";\n";
				break;
			case REDT_texture3D:
				str += "#if AIR_MAX_TEX_DEPTH <= 1\n";
				str += "Texture2D<" + elem_type + ">" + param_name + ";\n";
				str += "#else\n";
				str += "Textrue3D<" + elem_type + ">" + param_name + ";\n";
				str += "#endif\n";
				break;
			case REDT_textureCUBE:
				str += "TextureCube<" + elem_type + "> " + param_name + ";\n";
				break;
			case REDT_texture1DArray:
				str += "#if AIR_MAX_TEX_ARRAY_LEN >  1\n";
				str += "Texture1DArray<" + elem_type + "> " + param_name + ";\n";
				str += "endif\n";
				break;
			case REDT_texture2DArray:
				str += "#if AIR_MAX_TEX_ARRAY_LEN > 1\n";
				str += "Texture2DArray<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;
			case REDT_textureCUBEArray:
				str += "#if (AIR_MAX_TEX_ARRAY_LEN > 1) && (AIR_SHADER_MODEL >= SHADER_MODE(4, 1))\n";
				str += "TextureCubeArray<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;
			case REDT_buffer:
				str += "Buffer<" + elem_type + "> " + param_name + ";\n";
				break;
			case REDT_sampler:
				str += "sampler " + param_name + ";\n";
				break;
			case REDT_structured_buffer:
				str += "StructuredBuffer<" + elem_type + "> " + param_name + ";\n";
				break;
			case REDT_byte_address_buffer:
				str += "ByteAddressBuffer " + param_name + ";\n";
				break;
			case REDT_rw_buffer:
				str += "#if AIR_SHADER_MODEL >= SHADER_MODEL(5, 0)\n";
				str += "RWBuffer<" + elem_type + "> " + param_name + ";\n";
				str += "#endif";
				break;
			case REDT_rw_structured_buffer:
				str += "RWStructuredBuffer<" + elem_type + "> " + param_name + ";\n";
				break;
			case REDT_rw_texture1D:
				str += "#if AIR_SHADER_MODEL >= SHADER_MODEL(5, 0)\n";
				str += "RWTexture1D<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;
			case REDT_rw_texture2D:
				str += "#if AIR_SHADER_MODEL >= SHADER_MODEL(5, 0)\n";
				str += "RWTexture2D<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;
			case REDT_rw_texture3D:
				str += "#if AIR_SHADER_MODEL >= SHADER_MODEL(5, 0)\n";
				str += "RWTexture3D<" + elem_type + "> " + param_name + ";\n";
				str += "endif\n";
				break;
			case REDT_rw_texture1DArray:
				str += "#if AIR_SHADER_MODEL >= SHADER_MODEL(5, 0)\n";
				str += "RWTexture1DArray<" + elem_type + "> " + param_name + ";\n";
				str += "endif\n";
				break;
			case REDT_rw_texture2DArray:
				str += "#if AIR_SHADER_MODEL >= SHADER_MODEL(5, 0)\n";
				str += "RWTexture2DAarray<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;
			case REDT_rw_byte_address_buffer:
				str += "RWByteAddressBuffer " + param_name + ";\n";
				break;
			case REDT_append_structured_buffer:
				str += "#if AIR_SHADER_MODEL >= SHADER_MODEL(5, 0)\n";
				str += "AppendStructruedBuffer<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;
			case REDT_consume_structured_buffer:
				str += "#if AIR_SHADER_MODEL >= SHADER_MODEL(5, 0)\n";
				str += "ConsumeStructuredBuffer<" + elem_type + "> " + param_name + ";\n";
				str += "#endif\n";
				break;
			default:
				break;
			}
		}

		for (uint32_t i = 0; i < this->getNumShaderFragments(); ++i)
		{
			RenderShaderFragment const & effect_shader_frag = this->getShaderFragmentByIndex(i);
			ShaderObject::ShaderType const shader_type = effect_shader_frag.getType();
			switch (shader_type)
			{
			case ShaderObject::ST_VertexShader:
				str += "#if AIR_VERTEX_SHADER\n";
				break;
			case ShaderObject::ST_PixelShader:
				str += "#if AIR_PIXEL_SHADER\n";
				break;
			case ShaderObject::ST_GeometryShader:
				str += "#if AIR_COMPUTE_SHADER\n";
				break;
			case ShaderObject::ST_ComputeShader:
				str += "#if AIR_COMPUTE_SHADER\n";
				break;
			case ShaderObject::ST_HullShader:
				str += "#if AIR_HULL_SHADER\n";
				break;
			case ShaderObject::ST_DomainShader:
				str += "#if AIR_DOMAIN_SHADER\n";
				break;
			case ShaderObject::ST_NumShaderTypes:
				break;
			default:
				AIR_UNREACHABLE("Invalid shader type");
			}
			ShaderModel const & ver = effect_shader_frag.getVersion();
			if ((ver.major_ver != 0) || (ver.minor_ver != 0))
			{
				str += "#if AIR_SHADER_MODEL >= SHADER_MODEL("
					+ boost::lexical_cast<std::string>(static_cast<int>(ver.major_ver)) + ", " + boost::lexical_cast<std::string>(static_cast<int>(ver.minor_ver)) + ")\n";
			}
			str += effect_shader_frag.getString() + "\n";
			if ((ver.major_ver != 0) || (ver.minor_ver != 0))
			{
				str += "#endif\n";
			}
			if (shader_type != ShaderObject::ST_NumShaderTypes)
			{
				str += "#endif\n";
			}

		}
	}
	std::string const & RenderEffectTemplate::getHLSLShaderText() const
	{
		//logWarn("hlsl:\n%s", str.c_str());
		return mHLSLShader;
	}
#endif

	RenderEffectPtr syncLoadRenderEffect(std::string const & effect_name)
	{
		return ResLoader::getInstance().syncQueryT<RenderEffect>(MakeUniquePtr<EffectLoadingDesc>(effect_name));
	}

#if AIR_IS_DEV_PLATFORM
	void RenderEffectAnnotation::load(XMLNodePtr const & node)
	{
		mType = type_define::getInstance().getTypeCode(node->getAttribString("type"));
		mName = node->getAttribString("name");
		mVal = read_var(node, mType, 0);
	}
#endif

	void RenderEffectAnnotation::streamIn(ResIdentifierPtr const & res)
	{
		res->read(&mType, sizeof(mType));
		mType = LE2Native(mType);
		mName = readShortString(res);
		mVal = stream_in_var(res, mType, 0);
	}

#if AIR_IS_DEV_PLATFORM
	void RenderEffectAnnotation::streamOut(std::ostream& os) const
	{
		uint32_t t = Native2LE(mType);
		os.write(reinterpret_cast<char const *>(&t), sizeof(t));
		stream_out_var(os, *mVal, mType, 0);
	}
#endif

	uint32_t RenderEffectAnnotation::getType() const
	{
		return mType;
	}
	std::string const & RenderEffectAnnotation::getName() const
	{
		return mName;
	}

	std::unique_ptr<RenderVariable> RenderVariableFloat4x4::clone()
	{
		auto ret = MakeUniquePtr<RenderVariableFloat4x4>();
		ret->mInCBuffer = mInCBuffer;
		if (mInCBuffer)
		{
			ret->mData = mData;
		}
		float4x4 val;
		this->getValue(val);
		*ret = val;
		return std::move(ret);
	}

	RenderVariable& RenderVariableFloat4x4::operator =(float4x4 const & value)
	{
		return RenderVariableConcrete<float4x4>::operator= (MathLib::transpose(value));

	}
	void RenderVariableFloat4x4::getValue(float4x4& val) const
	{
		RenderVariableConcrete<float4x4>::getValue(val);
		val = MathLib::transpose(val);
	}

	std::unique_ptr<RenderVariable> RenderVariableFloat4x4Array::clone()
	{
		auto ret = MakeUniquePtr<RenderVariableFloat4x4Array>();
		if (mInCBuffer)
		{
			if (!ret->mInCBuffer)
			{
				ret->retriveT().~vector();
			}
			ret->mData = mData;
		}
		ret->mInCBuffer = mInCBuffer;
		std::vector<float4x4> val;
		this->getValue(val);
		*ret = val;
		return std::move(ret);
	}

	RenderVariable& RenderVariableFloat4x4Array::operator=(std::vector<float4x4> const & value)
	{
		if (mInCBuffer)
		{
			float4x4* target = mData.mCbufferDesc.mCBuffer->getVariableInBuff<float4x4>(mData.mCbufferDesc.offset);
			mSize = static_cast<uint32_t>(value.size());
			for (size_t i = 0; i < value.size(); ++i)
			{
				target[i] = MathLib::transpose(value[i]);
			}
			mData.mCbufferDesc.mCBuffer->setDirty(true);
		}
		else
		{
			this->retriveT() == value;
		}
		return *this;
	}

	void RenderVariableFloat4x4Array::getValue(std::vector<float4x4>& val) const
	{
		if (mInCBuffer)
		{
			float4x4 const * src = mData.mCbufferDesc.mCBuffer->getVariableInBuff<float4x4>(mData.mCbufferDesc.offset);
			val.resize(mSize);
			for (size_t i = 0; i < mSize; ++i)
			{
				val[i] = MathLib::transpose(src[i]);
			}
		}
		else
		{
			val = this->retriveT();
		}
	}

	std::unique_ptr<RenderVariable> RenderVariableTexture::clone()
	{
		auto ret = MakeUniquePtr<RenderVariableTexture>();
		TexturePtr val;
		this->getValue(val);
		*ret = val;
		std::string elem_type;
		this->getValue(elem_type);
		*ret = elem_type;
		return std::move(ret);
	}


	RenderVariable& RenderVariableTexture::operator= (TexturePtr const & value)
	{
		uint32_t array_size = 1;
		uint32_t mipmap = 1;
		if (value)
		{
			array_size = value->getArraySize();
			mipmap = value->getNumMipMaps();
		}
		mVal = TextureSubresource(value, 0, array_size, 0, mipmap);
		return *this;
	}
	RenderVariable& RenderVariableTexture::operator= (TextureSubresource const & value)
	{
		mVal = value;
		return *this;
	}
	RenderVariable& RenderVariableTexture::operator= (std::string const & value)
	{
		mElemType = value;
		return *this;

	}

	void RenderVariableTexture::getValue(TexturePtr& val) const
	{
		if (mVal.mTex)
		{
			mVal.mNumItems = mVal.mTex->getArraySize();
			mVal.mNumLevels = mVal.mTex->getNumMipMaps();
		}
		val = mVal.mTex;
	}

	void RenderVariableTexture::getValue(TextureSubresource& val) const
	{
		if (mVal.mTex)
		{
			mVal.mNumItems = mVal.mTex->getArraySize();
			mVal.mNumLevels = mVal.mTex->getNumMipMaps();
		}
		val = mVal;
	}
	void RenderVariableTexture::getValue(std::string& val) const
	{
		val = mElemType;
	}

	std::unique_ptr<RenderVariable> RenderVariableBuffer::clone()
	{
		auto ret = MakeUniquePtr<RenderVariableBuffer>();
		GraphicsBufferPtr val;
		this->getValue(val);
		*ret = val;
		std::string elem_type;
		this->getValue(elem_type);
		*ret = elem_type;
		return std::move(ret);
	}
	RenderVariable& RenderVariableBuffer::operator= (GraphicsBufferPtr const value) 
	{
		mVal = value;
		return *this;
	}
	RenderVariable& RenderVariableBuffer::operator = (std::string const value)
	{
		mElemType = value;
		return *this;
	}
	void RenderVariableBuffer::getValue(GraphicsBufferPtr& val) const
	{
		val = mVal;
	}
	void RenderVariableBuffer::getValue(std::string & val) const
	{
		val = mElemType;
	}


	std::unique_ptr<RenderVariable> RenderVariableByteAddressBuffer::clone()
	{
		auto ret = MakeUniquePtr<RenderVariableByteAddressBuffer>();
		ret->mVal = mVal;
		ret->mElemType = mElemType;
		return std::move(ret);
	}
	RenderVariable& RenderVariableByteAddressBuffer::operator= (GraphicsBufferPtr const & value)
	{
		mVal = value;
		return *this;
	}
	RenderVariable& RenderVariableByteAddressBuffer::operator=(std::string const & value)
	{
		mElemType = value;
		return *this;
	}
	void RenderVariableByteAddressBuffer::getValue(GraphicsBufferPtr& val) const
	{
		val = mVal;
	}
	void RenderVariableByteAddressBuffer::getValue(std::string & val) const
	{
		val = mElemType;
	}

	size_t RenderEffectConstantBuffer::getNameHash() const
	{
		return mName->second;
	}

	void RenderEffectConstantBuffer::addParameters(uint32_t index) const
	{
		mParamIndices->push_back(index);
	}

	uint32_t RenderEffectConstantBuffer::getParametersIndex(uint32_t index) const
	{
		return (*mParamIndices)[index];
	}

	void RenderEffectConstantBuffer::resize(uint32_t size)
	{
		mBuffer.resize(size);
		if (size > 0)
		{
			if (!mHWBuffer || (size > mHWBuffer->getSize()))
			{
				RenderFactory& rf = SingletonManager::getRenderFactoryInstance();
				mHWBuffer = rf.makeConstantBuffer(BU_Dynamic, 0, size, nullptr);
			}
		}
		mIsDirty = true;
	}

	void RenderEffectConstantBuffer::setDirty(bool dirty)
	{
		mIsDirty = dirty;
	}

	bool RenderEffectConstantBuffer::isDirty() const
	{
		return mIsDirty;
	}

	void RenderEffectConstantBuffer::update()
	{
		//if (mIsDirty)
		{
			mHWBuffer->updateSubResource(0, static_cast<uint32_t>(mBuffer.size()), &mBuffer[0]);
			mIsDirty = false;
		}
	}
	GraphicsBufferPtr const & RenderEffectConstantBuffer::getHWbuffer() const
	{
		return mHWBuffer;
	}

	void RenderEffectConstantBuffer::bindHWBuffer(GraphicsBufferPtr const & buff)
	{
		mHWBuffer = buff;
		mBuffer.resize(buff->getSize());
	}



	#if AIR_IS_DEV_PLATFORM
	void RenderEffectParameter::load(XMLNodePtr const & node)
	{
		mType = type_define::getInstance().getTypeCode(node->getAttrib("type")->getValueString());
		mName = MakeSharedPtr<std::remove_reference<decltype(*mName)>::type>();
		mName->first = node->getAttribString("name");
		mName->second = boost::hash_range(mName->first.begin(), mName->first.end());
		XMLAttributePtr attr = node->getAttrib("semantic");
		if (attr)
		{
			mSemantic = MakeSharedPtr < std::remove_reference<decltype(*mSemantic)>::type>();
			mSemantic->first = attr->getValueString();
			mSemantic->second = boost::hash_range(mSemantic->first.begin(), mSemantic->first.end());
		}
		uint32_t as;
		attr = node->getAttrib("array_size");
		if (attr)
		{
			mArraySize = MakeSharedPtr<std::string>(attr->getValueString());
			if (!attr->tryConvert(as))
			{
				as = 1;
			}
		}
		else
		{
			as = 0;
		}
		mVar = read_var(node, mType, as);
		{
			XMLNodePtr anno_node = node->getFirstNode("annotation");
			if (anno_node)
			{
				mAnnotations = MakeSharedPtr < std::remove_reference<decltype(*mAnnotations)>::type>();
				for (; anno_node; anno_node = anno_node->getNextSibling("annotation"))
				{
					mAnnotations->push_back(MakeUniquePtr<RenderEffectAnnotation>());
					mAnnotations->back()->load(anno_node);
				}
			}
		}
		if (mAnnotations && ((REDT_texture1D == mType) || (REDT_texture2D == mType) || (REDT_texture3D == mType) || (REDT_textureCUBE == mType) || (REDT_texture1DArray == mType) || (REDT_texture2DArray == mType) || (REDT_texture3DArray == mType) || (REDT_textureCUBEArray == mType)))
			{
			for (size_t i = 0; i < mAnnotations->size(); ++i)
			{
				if (REDT_string == (*mAnnotations)[i]->getType())
				{
					if ("SasResourceAddress" == (*mAnnotations)[i]->getName())
					{
						std::string val;
						(*mAnnotations)[i]->getValue(val);
						if (ResLoader::getInstance().locate(val).empty())
						{
							logError("%s Not found", val.c_str());
						}
						else
						{
							*mVar = syncLoadTexture(val, EAH_GPU_Read | EAH_Immutable);
						}
					}
				}
			}
		}
	}
	#endif




	void RenderEffectParameter::streamIn(ResIdentifierPtr const & res)
	{
		res->read(&mType, sizeof(mType));
		mType = LE2Native(mType);
		mName = MakeSharedPtr<std::remove_reference<decltype(*mName)>::type>();
		mName->first = readShortString(res);
		mName->second = boost::hash_range(mName->first.begin(), mName->first.end());

		std::string sem = readShortString(res);
		if (!sem.empty())
		{
			mSemantic = MakeSharedPtr<std::remove_reference<decltype(*mSemantic)>::type>();
			mSemantic->first = sem;
			mSemantic->second = boost::hash_range(sem.begin(), sem.end());
		}

		uint32_t as;
		std::string as_str = readShortString(res);
		if (as_str.empty())
		{
			as = 0;
		}
		else
		{
			mArraySize = MakeSharedPtr<std::string>(as_str);

			if (!boost::conversion::try_lexical_convert(as_str, as))
			{
				as = 1;  // dummy array size
			}
		}
		mVar = stream_in_var(res, mType, as);

		uint8_t num_anno;
		res->read(&num_anno, sizeof(num_anno));
		if (num_anno > 0)
		{
			mAnnotations = MakeSharedPtr<std::remove_reference<decltype(*mAnnotations)>::type>();
			mAnnotations->resize(num_anno);
			for (uint32_t i = 0; i < num_anno; ++i)
			{
				(*mAnnotations)[i] = MakeUniquePtr<RenderEffectAnnotation>();
				(*mAnnotations)[i]->streamIn(res);
			}
		}

		if (mAnnotations && ((REDT_texture1D == mType) || (REDT_texture2D == mType) || (REDT_texture3D == mType) || (REDT_textureCUBE == mType)
			|| (REDT_texture1DArray == mType) || (REDT_texture2DArray == mType) || (REDT_texture3DArray == mType) || (REDT_textureCUBEArray == mType)))
		{
			for (size_t i = 0; i < mAnnotations->size(); ++i)
			{
				if (REDT_string == (*mAnnotations)[i]->getType())
				{
					if ("SasResourceAddress" == (*mAnnotations)[i]->getName())
					{
						std::string val;
						(*mAnnotations)[i]->getValue(val);

						if (ResLoader::getInstance().locate(val).empty())
						{
							logError("%s NOT found", val.c_str());
						}
						else
						{
							*mVar = syncLoadTexture(val, EAH_GPU_Read | EAH_Immutable);
						}
					}
				}
			}
		}
	}

#if AIR_IS_DEV_PLATFORM
	void RenderEffectParameter::streamOut(std::ostream& os) const
	{
		uint32_t t = Native2LE(mType);
		os.write(reinterpret_cast<char const *>(&t), sizeof(t));
		writeShortString(os, mName->first);
		if (mSemantic)
		{
			writeShortString(os, mSemantic->first);
		}
		else
		{
			uint8_t len = 0;
			os.write(reinterpret_cast<char const *>(&len), sizeof(len));
		}

		if (mArraySize)
		{
			writeShortString(os, *mArraySize);
		}
		else
		{
			uint8_t len = 0;
			os.write(reinterpret_cast<char const *>(&len), sizeof(len));
		}
		uint32_t as;
		if (mArraySize)
		{
			if (!boost::conversion::try_lexical_convert(*mArraySize, as))
			{
				as = 1;  // dummy array size
			}
		}
		else
		{
			as = 0;
		}
		stream_out_var(os, *mVar, mType, as);

		uint8_t num_anno;
		if (mAnnotations)
		{
			num_anno = static_cast<uint8_t>(mAnnotations->size());
		}
		else
		{
			num_anno = 0;
		}
		os.write(reinterpret_cast<char const *>(&num_anno), sizeof(num_anno));
		for (uint32_t i = 0; i < num_anno; ++i)
		{
			(*mAnnotations)[i]->streamOut(os);
		}
	}
#endif

	void RenderEffectParameter::bindToCBuffer(RenderEffectConstantBuffer& cbuff, uint32_t offset, uint32_t stride)
	{
		mCBuffer = &cbuff;
		mVar->bindToCBuffer(cbuff, offset, stride);
	}
	
	void RenderEffectParameter::rebindToCBuffer(RenderEffectConstantBuffer& cbuff)
	{
		mCBuffer = &cbuff;
		mVar->rebindToCBuffer(cbuff);

	}

	RenderEffectConstantBuffer& RenderEffectParameter::getCBuffer() const
	{
		return *mCBuffer;
	}

	bool RenderEffectParameter::inCBuffer() const
	{
		return mVar->isInCBuffer();
	}

	uint32_t RenderEffectParameter::getCBufferOffset() const
	{
		return mVar->getCBufferOffset();
	}

	uint32_t RenderEffectParameter::getStride() const
	{
		return mVar->getStride();
	}

	std::unique_ptr<RenderEffectParameter> RenderEffectParameter::clone()
	{
		std::unique_ptr<RenderEffectParameter> ret = MakeUniquePtr<RenderEffectParameter>();
		ret->mName = mName;
		ret->mSemantic = mSemantic;
		ret->mType = mType;
		ret->mVar = mVar->clone();
		ret->mArraySize = mArraySize;
		ret->mAnnotations = mAnnotations;
		return ret;
	}

#if AIR_IS_DEV_PLATFORM
	void RenderPass::load(RenderEffect& effect, XMLNodePtr const & node, uint32_t tech_index, uint32_t pass_index, RenderPass const * inherit_pass)
	{
		RenderFactory& rf = SingletonManager::getRenderFactoryInstance();
		mName = node->getAttrib("name")->getValueString();
		mNameHash = boost::hash_range(mName.begin(), mName.end());

		DomainType dt = DT_Surface;
		BlendModeType bmt = BMT_Opaque;
		ShadingModelType smt = SMT_DefaultLit;
		bool hasVertexShader = true;

		XMLAttributePtr passAttr = node->getAttrib("domain");
		if (passAttr)
		{
			dt = domain_type_define::getInstance().fromStr(passAttr->getValueString());
		}
		passAttr = node->getAttrib("blendMode");
		if (passAttr)
		{
			bmt = blend_mode_type_define::getInstance().fromStr(passAttr->getValueString());
		}
		passAttr = node->getAttrib("shadingModel");
		if (passAttr)
		{
			smt = shading_model_type_define::getInstance().fromStr(passAttr->getValueString());
		}

		passAttr = node->getAttrib("hasVertexShader");
		if (passAttr)
		{
			hasVertexShader = BoolFromStr(passAttr->getValueString());
		}


		{
			XMLNodePtr anno_node = node->getFirstNode("annotation");
			if (anno_node)
			{
				mAnnotations = MakeSharedPtr<std::remove_reference<decltype(*mAnnotations)>::type>();
				if (inherit_pass && inherit_pass->mAnnotations)
				{
					*mAnnotations = *inherit_pass->mAnnotations;
				}
				for (; anno_node; anno_node = anno_node->getNextSibling("annotation"))
				{
					RenderEffectAnnotationPtr annatation = MakeSharedPtr<RenderEffectAnnotation>();
					mAnnotations->push_back(annatation);
					annatation->load(anno_node);
				}
			}
			else if (inherit_pass)
			{
				mAnnotations = inherit_pass->mAnnotations;
			}
		}
		{
			mMacros = MakeSharedPtr<std::remove_reference<decltype(*mMacros)>::type>();
			if (inherit_pass && inherit_pass->mMacros)
			{
				*mMacros = *inherit_pass->mMacros;
			}



			XMLNodePtr macro_node = node->getFirstNode("macro");
			if (macro_node)
			{
				for (; macro_node; macro_node = macro_node->getNextSibling("macro"))
				{
					std::string name = macro_node->getAttribString("name");
					std::string value = macro_node->getAttribString("value");
					bool found = false;
					for (size_t i = 0; i < mMacros->size(); ++i)
					{
						if ((*mMacros)[i].first == name)
						{
							(*mMacros)[i].second = value;
							found = true;
							break;
						}
					}
					if (!found)
					{
						mMacros->emplace_back(name, value);
					}
				}
			}
		}



		RasterizerStateDesc rs_desc;
		DepthStencilStateDesc dss_desc;
		BlendStateDesc bs_desc;
		mShaderObjectIndex = effect.addShaderObject();
		mShaderDescIds.fill(0);
		if (inherit_pass)
		{
			rs_desc = inherit_pass->mRenderStateObject->getRasterizerStateDesc();
			dss_desc = inherit_pass->mRenderStateObject->getDepthStencilStateDesc();
			bs_desc = inherit_pass->mRenderStateObject->getBlendStateDesc();
			mShaderDescIds = inherit_pass->mShaderDescIds;
		}

		uint64_t macros_hash = setDefaultShadingModelState(dt, bmt, smt, effect, tech_index, hasVertexShader);

		for (XMLNodePtr state_node = node->getFirstNode("state"); state_node; state_node = state_node->getNextSibling("state"))
		{
			std::string state_name = state_node->getAttribString("name");
			size_t const state_name_hash = RT_HASH(state_name.c_str());
			if (CT_HASH("polygon_mode") == state_name_hash)
			{
				std::string value_str = state_node->getAttribString("value");
				rs_desc.mPolygonMode = polygon_mode_define::getInstance().fromStr(value_str);

			}
			else if (CT_HASH("shade_mode") == state_name_hash)
			{
				std::string value_str = state_node->getAttrib("value")->getValueString();
				rs_desc.mShadeMode = shade_mode_define::getInstance().fromStr(value_str);
			}
			else if (CT_HASH("cull_mode") == state_name_hash)
			{
				std::string value_str = state_node->getAttrib("value")->getValueString();
				rs_desc.mCullMode = cull_mode_define::getInstance().fromStr(value_str);
			}
			else if (CT_HASH("front_face_ccw") == state_name_hash)
			{
				std::string value_str = state_node->getAttrib("value")->getValueString();
				rs_desc.mFrontFaceCCW = BoolFromStr(value_str);
			}
			else if (CT_HASH("polygon_offset_factor") == state_name_hash)
			{
				rs_desc.mPolygonOffsetFactor = state_node->getAttrib("value")->getValueFloat();
			}
			else if (CT_HASH("polygon_offset_units") == state_name_hash)
			{
				rs_desc.mPolygonOffsetUnits = state_node->getAttrib("value")->getValueFloat();
			}
			else if (CT_HASH("depth_clip_enable") == state_name_hash)
			{
				std::string value_str = state_node->getAttrib("value")->getValueString();
				rs_desc.mDepthClipEnable = BoolFromStr(value_str);
			}
			else if (CT_HASH("scissor_enable") == state_name_hash)
			{
				std::string value_str = state_node->getAttrib("value")->getValueString();
				rs_desc.mScissorEnable = BoolFromStr(value_str);
			}
			else if (CT_HASH("multisample_enable") == state_name_hash)
			{
				std::string value_str = state_node->getAttrib("value")->getValueString();
				rs_desc.mMultiSampleEnabel = BoolFromStr(value_str);
			}
			else if (CT_HASH("alpha_to_coverage_enable") == state_name_hash)
			{
				std::string value_str = state_node->getAttrib("value")->getValueString();
				bs_desc.mAlphaToCoverageEnable = BoolFromStr(value_str);
			}
			else if (CT_HASH("independent_blend_enable") == state_name_hash)
			{
				std::string value_str = state_node->getAttrib("value")->getValueString();
				bs_desc.mIndependentBlendEnable = BoolFromStr(value_str);
			}
			else if (CT_HASH("blend_enable") == state_name_hash)
			{
				int index = get_index(state_node);
				std::string value_str = state_node->getAttrib("value")->getValueString();
				bs_desc.mBlendEnable[index] = BoolFromStr(value_str);
			}
			else if (CT_HASH("logic_op_enable") == state_name_hash)
			{
				int index = get_index(state_node);
				std::string value_str = state_node->getAttrib("value")->getValueString();
				bs_desc.mLogicOpEnable[index] = BoolFromStr(value_str);
			}
			else if (CT_HASH("blend_op") == state_name_hash)
			{
				int index = get_index(state_node);
				std::string value_str = state_node->getAttrib("value")->getValueString();
				bs_desc.mBlendOp[index] = blend_operation_define::instance().fromStr(value_str);
			}
			else if (CT_HASH("src_blend") == state_name_hash)
			{
				int index = get_index(state_node);
				std::string value_str = state_node->getAttrib("value")->getValueString();
				bs_desc.mSrcBlend[index] = alpha_blend_factor_define::getInstance().fromStr(value_str);
			}
			else if (CT_HASH("dest_blend") == state_name_hash)
			{
				int index = get_index(state_node);
				std::string value_str = state_node->getAttrib("value")->getValueString();
				bs_desc.mDstBlend[index] = alpha_blend_factor_define::getInstance().fromStr(value_str);
			}
			else if (CT_HASH("blend_op_alpha") == state_name_hash)
			{
				int index = get_index(state_node);
				std::string value_str = state_node->getAttrib("value")->getValueString();
				bs_desc.mBlendOpAlpha[index] = blend_operation_define::instance().fromStr(value_str);
			}
			else if (CT_HASH("src_blend_alpha") == state_name_hash)
			{
				int index = get_index(state_node);
				std::string value_str = state_node->getAttrib("value")->getValueString();
				bs_desc.mSrcBlendAlpha[index] = alpha_blend_factor_define::getInstance().fromStr(value_str);
			}
			else if (CT_HASH("dest_blend_alpha") == state_name_hash)
			{
				int index = get_index(state_node);
				std::string value_str = state_node->getAttrib("value")->getValueString();
				bs_desc.mDstBlendAlpha[index] = alpha_blend_factor_define::getInstance().fromStr(value_str);
			}
			else if (CT_HASH("logic_op") == state_name_hash)
			{
				int index = get_index(state_node);
				std::string value_str = state_node->getAttrib("value")->getValueString();
				bs_desc.mLogicOp[index] = logic_operation_define::getInstance().fromStr(value_str);
			}
			else if (CT_HASH("color_write_mask") == state_name_hash)
			{
				int index = get_index(state_node);
				bs_desc.mColorWriteMask[index] = static_cast<uint8_t>(state_node->getAttrib("value")->getValueInt());
			}
			else if (CT_HASH("blend_factor") == state_name_hash)
			{
				XMLAttributePtr attr = state_node->getAttrib("r");
				if (attr)
				{
					bs_desc.mBlendFactor.r() = attr->getValueFloat();
				}
				attr = state_node->getAttrib("g");
				if (attr)
				{
					bs_desc.mBlendFactor.g() = attr->getValueFloat();
				}
				attr = state_node->getAttrib("b");
				if (attr)
				{
					bs_desc.mBlendFactor.b() = attr->getValueFloat();
				}
				attr = state_node->getAttrib("a");
				if (attr)
				{
					bs_desc.mBlendFactor.a() = attr->getValueFloat();
				}
			}
			else if (CT_HASH("sample_mask") == state_name_hash)
			{
				bs_desc.mSampleMask = state_node->getAttrib("value")->getValueUInt();
			}
			else if (CT_HASH("depth_enable") == state_name_hash)
			{
				dss_desc.mDepthEnable = BoolFromStr(state_node->getAttrib("value")->getValueString());
			}
			else if (CT_HASH("depth_write_enable") == state_name_hash)
			{
				dss_desc.mDepthWriteEnable = BoolFromStr(state_node->getAttrib("value")->getValueString());
			}
			else if (CT_HASH("depth_func") == state_name_hash)
			{
				std::string value_str = state_node->getAttrib("value")->getValueString();
				dss_desc.mDepthFunc = compare_function_define::getInstance().fromStr(value_str);
			}
			else if (CT_HASH("front_stencil_enable") == state_name_hash)
			{
				dss_desc.mFrontStencilEnable = BoolFromStr(state_node->getAttrib("value")->getValueString());
			}
			else if (CT_HASH("front_stencil_func") == state_name_hash)
			{
				std::string value_str = state_node->getAttrib("value")->getValueString();
				dss_desc.mFrontStencilFunc = compare_function_define::getInstance().fromStr(value_str);
			}
			else if (CT_HASH("front_stencil_ref") == state_name_hash)
			{
				dss_desc.mFrontStencilRef = static_cast<uint16_t>(state_node->getAttrib("value")->getValueUInt());
			}
			else if (CT_HASH("front_stencil_read_mask") == state_name_hash)
			{
				dss_desc.mFrontstenCilReadMask = static_cast<uint16_t>(state_node->getAttrib("value")->getValueUInt());
			}
			else if (CT_HASH("front_stencil_write_mask") == state_name_hash)
			{
				dss_desc.mFrontStencilWriteMask = static_cast<uint16_t>(state_node->getAttrib("value")->getValueUInt());
			}
			else if (CT_HASH("front_stencil_fail") == state_name_hash)
			{
				std::string value_str = state_node->getAttrib("value")->getValueString();
				dss_desc.mFrontStencilFail = stencil_operation_define::getInstance().fromStr(value_str);
			}
			else if (CT_HASH("front_stencil_depth_fail") == state_name_hash)
			{
				std::string value_str = state_node->getAttrib("value")->getValueString();
				dss_desc.mFrontStencilDepthFail = stencil_operation_define::getInstance().fromStr(value_str);
			}
			else if (CT_HASH("front_stencil_pass") == state_name_hash)
			{
				std::string value_str = state_node->getAttrib("value")->getValueString();
				dss_desc.mFrontStencilPass= stencil_operation_define::getInstance().fromStr(value_str);
			}
			else if (CT_HASH("back_stencil_enable") == state_name_hash)
			{
				dss_desc.mBackStencilEnable = BoolFromStr(state_node->getAttrib("value")->getValueString());
			}
			else if (CT_HASH("back_stencil_func") == state_name_hash)
			{
				std::string value_str = state_node->getAttrib("value")->getValueString();
				dss_desc.mBackStencilFunc = compare_function_define::getInstance().fromStr(value_str);
			}
			else if (CT_HASH("back_stencil_ref") == state_name_hash)
			{
				dss_desc.mBackStencilRef = static_cast<uint16_t>(state_node->getAttrib("value")->getValueUInt());
			}
			else if (CT_HASH("back_stencil_read_mask") == state_name_hash)
			{
				dss_desc.mBackStencilReadMask = static_cast<uint16_t>(state_node->getAttrib("value")->getValueUInt());
			}
			else if (CT_HASH("back_stencil_write_mask") == state_name_hash)
			{
				dss_desc.mBackStencilWriteMask = static_cast<uint16_t>(state_node->getAttrib("value")->getValueUInt());
			}
			else if (CT_HASH("back_stencil_fail") == state_name_hash)
			{
				std::string value_str = state_node->getAttrib("value")->getValueString();
				dss_desc.mBackStencilFail = stencil_operation_define::getInstance().fromStr(value_str);
			}
			else if (CT_HASH("back_stencil_depth_fail") == state_name_hash)
			{
				std::string value_str = state_node->getAttrib("value")->getValueString();
				dss_desc.mBackStencilDepthFail = stencil_operation_define::getInstance().fromStr(value_str);
			}
			else if (CT_HASH("back_stencil_pass") == state_name_hash)
			{
				std::string value_str = state_node->getAttrib("value")->getValueString();
				dss_desc.mBackStencilPass = stencil_operation_define::getInstance().fromStr(value_str);
			}
			else if ((CT_HASH("vertex_shader") == state_name_hash) || (CT_HASH("pixel_shader") == state_name_hash)
				|| (CT_HASH("geometry_shader") == state_name_hash) || (CT_HASH("compute_shader") == state_name_hash)
				|| (CT_HASH("hull_shader") == state_name_hash) || (CT_HASH("domain_shader") == state_name_hash))
			{
				ShaderObject::ShaderType type;
				if (CT_HASH("vertex_shader") == state_name_hash)
				{
					type = ShaderObject::ST_VertexShader;
				}
				else if (CT_HASH("pixel_shader") == state_name_hash)
				{
					type = ShaderObject::ST_PixelShader;
				}
				else if (CT_HASH("geometry_shader") == state_name_hash)
				{
					type = ShaderObject::ST_GeometryShader;
				}
				else if (CT_HASH("compute_shader") == state_name_hash)
				{
					type = ShaderObject::ST_ComputeShader;
				}
				else if (CT_HASH("hull_shader") == state_name_hash)
				{
					type = ShaderObject::ST_HullShader;
				}
				else
				{
					BOOST_ASSERT("domain_shader" == state_name);
					type = ShaderObject::ST_DomainShader;
				}

				ShaderDesc sd;
				sd.mProfile = get_profile(state_node);
				sd.mFunctionName = get_func_name(state_node);
				sd.mMacrosHash = macros_hash;

				if ((ShaderObject::ST_VertexShader == type) || (ShaderObject::ST_GeometryShader == type))
				{
					XMLNodePtr so_node = state_node->getFirstNode("stream_output");
					if (so_node)
					{
						for (XMLNodePtr entry_node = so_node->getFirstNode("entry"); entry_node; entry_node = entry_node->getNextSibling("entry"))
						{
							ShaderDesc::StreamOutputDecl decl;

							std::string usage_str = entry_node->getAttrib("usage")->getValueString();
							size_t const usage_str_hash = RT_HASH(usage_str.c_str());
							XMLAttributePtr attr = entry_node->getAttrib("usage_index");
							if (attr)
							{
								decl.mUsageIndex = static_cast<uint8_t>(attr->getValueInt());
							}
							else
							{
								decl.mUsageIndex = 0;
							}

							if ((CT_HASH("POSITION") == usage_str_hash) || (CT_HASH("SV_Position") == usage_str_hash))
							{
								decl.mUsage = VEU_Position;
							}
							else if (CT_HASH("NORMAL") == usage_str_hash)
							{
								decl.mUsage = VEU_Normal;
							}
							else if (CT_HASH("COLOR") == usage_str_hash)
							{
								if (0 == decl.mUsageIndex)
								{
									decl.mUsage = VEU_Diffuse;
								}
								else
								{
									decl.mUsage = VEU_Specular;
								}
							}
							else if (CT_HASH("BLENDWEIGHT") == usage_str_hash)
							{
								decl.mUsage = VEU_BlendWeight;
							}
							else if (CT_HASH("BLENDINDICES") == usage_str_hash)
							{
								decl.mUsage = VEU_BlendIndex;
							}
							else if (CT_HASH("TEXCOORD") == usage_str_hash)
							{
								decl.mUsage = VEU_TextureCoord;
							}
							else if (CT_HASH("TANGENT") == usage_str_hash)
							{
								decl.mUsage = VEU_Tangent;
							}
							else if (CT_HASH("BINORMAL") == usage_str_hash)
							{
								decl.mUsage = VEU_Binormal;
							}

							attr = entry_node->getAttrib("component");
							std::string component_str;
							if (attr)
							{
								component_str = entry_node->getAttrib("component")->getValueString();
							}
							else
							{
								component_str = "xyzw";
							}
							decl.mStartComponent = static_cast<uint8_t>(component_str[0] - 'x');
							decl.mStartComponent = static_cast<uint8_t>(std::min(static_cast<size_t>(4), component_str.size()));

							attr = entry_node->getAttrib("slot");
							if (attr)
							{
								decl.mSlot = static_cast<uint8_t>(entry_node->getAttrib("slot")->getValueInt());
							}
							else
							{
								decl.mSlot= 0;
							}

							sd.mSODecl.push_back(decl);
						}
					}
				}

				if (ShaderObject::ST_ComputeShader == type)
				{
					XMLAttributePtr disAtt = state_node->getAttrib("dispatch");
					if (disAtt)
					{
						mUseDispach = false;
						mDispatchParams = Vector3FromStr<int, 3>(disAtt->getValueString());
					}
					else
					{
						mUseDispach = true;
					}
				}

				mShaderDescIds[type] = effect.addShaderDesc(sd);
			}
			else
			{
				AIR_UNREACHABLE("Invalid state name");
			}

		}



		


		mRenderStateObject = rf.makeRenderStateObject(rs_desc, dss_desc, bs_desc);
		auto const & shader_obj = this->getShaderObject(effect);

		for (int type = 0; type < ShaderObject::ST_NumShaderTypes; ++ type)
		{
			ShaderDesc& sd = effect.getShaderDesc(mShaderDescIds[type]);
			if (!sd.mFunctionName.empty())
			{
				if (sd.mTechPassType != 0xffffffff)
				{
					auto const & tech = *effect.getTechniqueByIndex(sd.mTechPassType >> 16);
					auto const & pass = tech.getPass((sd.mTechPassType >> 8) & 0xff);
					shader_obj->attachShader(static_cast<ShaderObject::ShaderType>(type), effect, tech, pass, pass.getShaderObject(effect));
				}
				else
				{
					auto const & tech = *effect.getTechniqueByIndex(tech_index);
					shader_obj->attachShader(static_cast<ShaderObject::ShaderType>(type), effect, tech, *this, mShaderDescIds);
					sd.mTechPassType = (tech_index << 16) + (pass_index << 8) + type;
				}
			}
		}
		shader_obj->linkShaders(effect);
		mIsValidate = shader_obj->isvalidate();
	}
	void RenderPass::load(RenderEffect& effect, uint32_t tech_index, uint32_t pass_index, RenderPass const * inhert_pass)
	{

	}
#endif

	uint64_t RenderPass::setDefaultShadingModelState(DomainType dt, BlendModeType bmt, ShadingModelType smt, RenderEffect& effect, uint32_t tech_index, bool hasVertexShader)
	{
		bool defaultVS = false;
		bool defaultPS = false;

		ShaderDesc vsdesc;
		ShaderDesc fsdesc;


		switch ((dt << (SMT_Num + BMT_Num)) | (bmt << SMT_Num) | smt)
		{
		case ((DT_Surface << (SMT_Num + BMT_Num)) | (BMT_Opaque << SMT_Num) | SMT_DefaultLit):

			mMacros->emplace_back("STANDARDSURFACE", "1");
			vsdesc.mFunctionName = "defaultModelVertexShader";
			vsdesc.mProfile = "auto";
			defaultVS = true;
			break;
		default:
			if (DT_PostProcess == dt)
			{
				mMacros->emplace_back("POST_PROCESS", "1");
				vsdesc.mFunctionName = "postProcessVertexShader";
				vsdesc.mProfile = "auto";
				defaultVS = true;
			}
			else
			{
				AIR_UNREACHABLE("invalid mateirals");
				break;
			}
		}
		uint64_t macros_hash;
		{
			RenderTechnique* tech = effect.getTechniqueByIndex(tech_index);
			size_t hash_val = 0;
			for (uint32_t i = 0; i < tech->getNumMacros(); ++i)
			{
				std::pair<std::string, std::string> const & name_value = tech->getMacroByIndex(i);
				boost::hash_range(hash_val, name_value.first.begin(), name_value.first.end());
				boost::hash_range(hash_val, name_value.second.begin(), name_value.second.end());
			}
			for (uint32_t i = 0; i < this->getNumMacros(); ++i)
			{
				std::pair<std::string, std::string> const & name_value = this->getMacroByIndex(i);
				boost::hash_range(hash_val, name_value.first.begin(), name_value.first.end());
				boost::hash_range(hash_val, name_value.second.begin(), name_value.second.end());
			}
			macros_hash = static_cast<uint64_t>(hash_val);
		}
		if (hasVertexShader)
		{
			if (defaultVS)
			{
				vsdesc.mMacrosHash = macros_hash;
				mShaderDescIds[ShaderObject::ST_VertexShader] = effect.addShaderDesc(vsdesc);
			}
			if (defaultPS)
			{
				fsdesc.mMacrosHash = macros_hash;
				mShaderDescIds[ShaderObject::ST_PixelShader] = effect.addShaderDesc(fsdesc);
			}
		}
		return macros_hash;
	}

	void RenderPass::bind(RenderEffect const & effect) const
	{
		RenderEngine& re = SingletonManager::getRenderFactoryInstance().getRenderEngineInstance();
		re.setStateObject(mRenderStateObject);
		this->getShaderObject(effect)->bind();
	}

	void RenderPass::unbind(RenderEffect const & effect) const
	{
		this->getShaderObject(effect)->unbind();
	}

	bool RenderPass::streamIn(RenderEffect& effect, ResIdentifierPtr const & res, uint32_t tech_index, uint32_t pass_index)
	{
		RenderFactory& rf = SingletonManager::getRenderFactoryInstance();

		mName =readShortString(res);
		mNameHash = boost::hash_range(mName.begin(), mName.end());

		uint8_t num_anno;
		res->read(&num_anno, sizeof(num_anno));
		if (num_anno > 0)
		{
			mAnnotations = MakeSharedPtr<std::remove_reference<decltype(*mAnnotations)>::type>();
			mAnnotations->resize(num_anno);
			for (uint32_t i = 0; i < num_anno; ++i)
			{
				RenderEffectAnnotationPtr annotation = MakeSharedPtr<RenderEffectAnnotation>();
				(*mAnnotations)[i] = annotation;
				annotation->streamIn(res);
			}
		}

		uint8_t num_macro;
		res->read(&num_macro, sizeof(num_macro));
		if (num_macro > 0)
		{
			mMacros = MakeSharedPtr<std::remove_reference<decltype(*mMacros)>::type>();
			mMacros->resize(num_macro);
			for (uint32_t i = 0; i < num_macro; ++i)
			{
				std::string name = readShortString(res);
				std::string value = readShortString(res);
				(*mMacros)[i] = std::make_pair(name, value);
			}
		}

		RasterizerStateDesc rs_desc;
		DepthStencilStateDesc dss_desc;
		BlendStateDesc bs_desc;

		res->read(&rs_desc, sizeof(rs_desc));
		rs_desc.mPolygonMode = LE2Native(rs_desc.mPolygonMode);
		rs_desc.mShadeMode = LE2Native(rs_desc.mShadeMode);
		rs_desc.mCullMode = LE2Native(rs_desc.mCullMode);
		rs_desc.mPolygonOffsetFactor = LE2Native(rs_desc.mPolygonOffsetFactor);
		rs_desc.mPolygonOffsetUnits = LE2Native(rs_desc.mPolygonOffsetUnits);

		res->read(&dss_desc, sizeof(dss_desc));
		dss_desc.mDepthFunc = LE2Native(dss_desc.mDepthFunc);
		dss_desc.mFrontStencilFunc = LE2Native(dss_desc.mFrontStencilFunc);
		dss_desc.mFrontStencilRef = LE2Native(dss_desc.mFrontStencilRef);
		dss_desc.mFrontstenCilReadMask = LE2Native(dss_desc.mFrontstenCilReadMask);
		dss_desc.mFrontStencilWriteMask = LE2Native(dss_desc.mFrontStencilWriteMask);
		dss_desc.mFrontStencilFail = LE2Native(dss_desc.mFrontStencilFail);
		dss_desc.mFrontStencilDepthFail = LE2Native(dss_desc.mFrontStencilDepthFail);
		dss_desc.mFrontStencilPass = LE2Native(dss_desc.mFrontStencilPass);
		dss_desc.mBackStencilFunc = LE2Native(dss_desc.mBackStencilFunc);
		dss_desc.mBackStencilRef = LE2Native(dss_desc.mBackStencilRef);
		dss_desc.mBackStencilReadMask = LE2Native(dss_desc.mBackStencilReadMask);
		dss_desc.mBackStencilWriteMask = LE2Native(dss_desc.mBackStencilWriteMask);
		dss_desc.mBackStencilFail = LE2Native(dss_desc.mBackStencilFail);
		dss_desc.mBackStencilDepthFail = LE2Native(dss_desc.mBackStencilDepthFail);
		dss_desc.mBackStencilPass = LE2Native(dss_desc.mBackStencilPass);

		res->read(&bs_desc, sizeof(bs_desc));
		for (size_t i = 0; i < 4; ++i)
		{
			bs_desc.mBlendFactor[i] = LE2Native(bs_desc.mBlendFactor[i]);
		}
		bs_desc.mSampleMask = LE2Native(bs_desc.mSampleMask);
		for (size_t i = 0; i < bs_desc.mBlendOp.size(); ++i)
		{
			bs_desc.mBlendOp[i] = LE2Native(bs_desc.mBlendOp[i]);
			bs_desc.mSrcBlend[i] = LE2Native(bs_desc.mSrcBlend[i]);
			bs_desc.mDstBlend[i] = LE2Native(bs_desc.mDstBlend[i]);
			bs_desc.mBlendOpAlpha[i] = LE2Native(bs_desc.mBlendOpAlpha[i]);
			bs_desc.mSrcBlendAlpha[i] = LE2Native(bs_desc.mSrcBlendAlpha[i]);
			bs_desc.mDstBlendAlpha[i] = LE2Native(bs_desc.mDstBlendAlpha[i]);
		}

		mRenderStateObject = rf.makeRenderStateObject(rs_desc, dss_desc, bs_desc);

		res->read(&mShaderDescIds[0], mShaderDescIds.size() * sizeof(mShaderDescIds[0]));
		for (int i = 0; i < ShaderObject::ST_NumShaderTypes; ++i)
		{
			mShaderDescIds[i] = LE2Native(mShaderDescIds[i]);
		}


		mShaderObjectIndex = effect.addShaderObject();
		auto const & shader_obj = this->getShaderObject(effect);

		bool native_accepted = true;

		for (int type = 0; type < ShaderObject::ST_NumShaderTypes; ++type)
		{
			ShaderDesc const & sd = effect.getShaderDesc(mShaderDescIds[type]);
			if (!sd.mFunctionName.empty())
			{
				ShaderObject::ShaderType st = static_cast<ShaderObject::ShaderType>(type);

				bool this_native_accepted;
				if (sd.mTechPassType != (tech_index << 16) + (pass_index << 8) + type)
				{
					auto const & tech = *effect.getTechniqueByIndex(sd.mTechPassType >> 16);
					auto const & pass = tech.getPass((sd.mTechPassType >> 8) & 0xFF);
					shader_obj->attachShader(st, effect, tech, pass, pass.getShaderObject(effect));
					this_native_accepted = true;
				}
				else
				{
					this_native_accepted = shader_obj->streamIn(res, static_cast<ShaderObject::ShaderType>(type),
						effect, mShaderDescIds);
				}

				native_accepted &= this_native_accepted;
			}
		}

		shader_obj->linkShaders(effect);

		mIsValidate = shader_obj->isvalidate();

		return native_accepted;
	}

#if AIR_IS_DEV_PLATFORM
	void RenderPass::streamOut(RenderEffect const & effect, std::ostream& os, uint32_t tech_index, uint32_t pass_index) const
	{
		writeShortString(os, mName);

		uint8_t num_anno;
		if (mAnnotations)
		{
			num_anno = static_cast<uint8_t>(mAnnotations->size());
		}
		else
		{
			num_anno = 0;
		}
		os.write(reinterpret_cast<char const *>(&num_anno), sizeof(num_anno));
		for (uint32_t i = 0; i < num_anno; ++i)
		{
			RenderEffectAnnotationPtr annotation = MakeSharedPtr<RenderEffectAnnotation>();
			(*mAnnotations)[i] = annotation;

			annotation->streamOut(os);
		}

		uint8_t num_macro;
		if (mMacros)
		{
			num_macro = static_cast<uint8_t>(mMacros->size());
		}
		else
		{
			num_macro = 0;
		}
		os.write(reinterpret_cast<char const *>(&num_macro), sizeof(num_macro));
		for (uint32_t i = 0; i < num_macro; ++i)
		{
			writeShortString(os, (*mMacros)[i].first);
			writeShortString(os, (*mMacros)[i].second);
		}

		RasterizerStateDesc rs_desc = mRenderStateObject->getRasterizerStateDesc();
		DepthStencilStateDesc dss_desc = mRenderStateObject->getDepthStencilStateDesc();
		BlendStateDesc bs_desc = mRenderStateObject->getBlendStateDesc();

		rs_desc.mPolygonMode = Native2LE(rs_desc.mPolygonMode);
		rs_desc.mShadeMode = Native2LE(rs_desc.mShadeMode);
		rs_desc.mCullMode = Native2LE(rs_desc.mCullMode);
		rs_desc.mPolygonOffsetFactor = Native2LE(rs_desc.mPolygonOffsetFactor);
		rs_desc.mPolygonOffsetUnits = Native2LE(rs_desc.mPolygonOffsetUnits);
		os.write(reinterpret_cast<char const *>(&rs_desc), sizeof(rs_desc));

		dss_desc.mDepthFunc = Native2LE(dss_desc.mDepthFunc);
		dss_desc.mFrontStencilFunc = Native2LE(dss_desc.mFrontStencilFunc);
		dss_desc.mFrontStencilRef = Native2LE(dss_desc.mFrontStencilRef);
		dss_desc.mFrontstenCilReadMask = Native2LE(dss_desc.mFrontstenCilReadMask);
		dss_desc.mFrontStencilWriteMask = Native2LE(dss_desc.mFrontStencilWriteMask);
		dss_desc.mFrontStencilFail = Native2LE(dss_desc.mFrontStencilFail);
		dss_desc.mFrontStencilDepthFail = Native2LE(dss_desc.mFrontStencilDepthFail);
		dss_desc.mFrontStencilPass = Native2LE(dss_desc.mFrontStencilPass);
		dss_desc.mBackStencilFunc = Native2LE(dss_desc.mBackStencilFunc);
		dss_desc.mBackStencilRef = Native2LE(dss_desc.mBackStencilRef);
		dss_desc.mBackStencilReadMask = Native2LE(dss_desc.mBackStencilReadMask);
		dss_desc.mBackStencilWriteMask = Native2LE(dss_desc.mBackStencilWriteMask);
		dss_desc.mBackStencilFail = Native2LE(dss_desc.mBackStencilFail);
		dss_desc.mBackStencilDepthFail = Native2LE(dss_desc.mBackStencilDepthFail);
		dss_desc.mBackStencilPass = Native2LE(dss_desc.mBackStencilPass);
		os.write(reinterpret_cast<char const *>(&dss_desc), sizeof(dss_desc));

		for (size_t i = 0; i < 4; ++i)
		{
			bs_desc.mBlendFactor[i] = Native2LE(bs_desc.mBlendFactor[i]);
		}
		bs_desc.mSampleMask = Native2LE(bs_desc.mSampleMask);
		for (size_t i = 0; i < bs_desc.mBlendOp.size(); ++i)
		{
			bs_desc.mBlendOp[i] = Native2LE(bs_desc.mBlendOp[i]);
			bs_desc.mSrcBlend[i] = Native2LE(bs_desc.mSrcBlend[i]);
			bs_desc.mDstBlend[i] = Native2LE(bs_desc.mDstBlend[i]);
			bs_desc.mBlendOpAlpha[i] = Native2LE(bs_desc.mBlendOpAlpha[i]);
			bs_desc.mSrcBlendAlpha[i] = Native2LE(bs_desc.mSrcBlendAlpha[i]);
			bs_desc.mDstBlendAlpha[i] = Native2LE(bs_desc.mDstBlendAlpha[i]);
		}
		os.write(reinterpret_cast<char const *>(&bs_desc), sizeof(bs_desc));

		for (uint32_t i = 0; i < mShaderDescIds.size(); ++i)
		{
			uint32_t tmp = Native2LE(mShaderDescIds[i]);
			os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
		}

		for (int type = 0; type < ShaderObject::ST_NumShaderTypes; ++type)
		{
			ShaderDesc const & sd = effect.getShaderDesc(mShaderDescIds[type]);
			if (!sd.mFunctionName.empty())
			{
				if (sd.mTechPassType == (tech_index << 16) + (pass_index << 8) + type)
				{
					this->getShaderObject(effect)->streamOut(os, static_cast<ShaderObject::ShaderType>(type));
				}
			}
		}
	}
#endif

#if AIR_IS_DEV_PLATFORM
	void RenderShaderFragment::load(XMLNodePtr const & node)
	{
		mType = ShaderObject::ST_NumShaderTypes;
		XMLAttributePtr attr = node->getAttrib("type");
		if (attr)
		{
			std::string type_str = attr->getValueString();
			size_t const type_str_hash = RT_HASH(type_str.c_str());
			if (CT_HASH("vertex_shader") == type_str_hash)
			{
				mType = ShaderObject::ST_VertexShader;
			}
			else if (CT_HASH("pixel_shader") == type_str_hash)
			{
				mType = ShaderObject::ST_PixelShader;
			}
			else if (CT_HASH("geometry_shader") == type_str_hash)
			{
				mType = ShaderObject::ST_GeometryShader;
			}
			else if (CT_HASH("compute_shader") == type_str_hash)
			{
				mType = ShaderObject::ST_ComputeShader;
			}
			else if (CT_HASH("hull_shader") == type_str_hash)
			{
				mType = ShaderObject::ST_HullShader;
			}
			else
			{
				BOOST_ASSERT("domain_shader" == type_str);
				mType = ShaderObject::ST_DomainShader;
			}
		}

		mVer = ShaderModel(0, 0);
		attr = node->getAttrib("major_version");
		if (attr)
		{
			uint8_t minor_ver = 0;
			XMLAttributePtr minor_attr = node->getAttrib("minor_version");
			if (minor_attr)
			{
				minor_ver = static_cast<uint8_t>(minor_attr->getValueInt());
			}
			mVer = ShaderModel(static_cast<uint8_t>(attr->getValueInt()), minor_ver);
		}
		else
		{
			attr = node->getAttrib("version");
			if (attr)
			{
				mVer = ShaderModel(static_cast<uint8_t>(attr->getValueInt()), 0);
			}
		}

		for (XMLNodePtr shader_text_node = node->getFirstNode(); shader_text_node; shader_text_node = shader_text_node->getNextSibling())
		{
			if ((XNT_Comment == shader_text_node->getType()) || (XNT_CData == shader_text_node->getType()))
			{
				mStr += shader_text_node->getValueString();
			}
		}
	}
#endif
	void RenderShaderFragment::streamIn(ResIdentifierPtr const & res)
	{
		res->read(&mType, sizeof(mType));
		mType = LE2Native(mType);
		res->read(&mVer, sizeof(mVer));

		uint32_t len;
		res->read(&len, sizeof(len));
		len = LE2Native(len);
		mStr.resize(len);
		res->read(&mStr[0], len * sizeof(mStr[0]));
	}
#if AIR_IS_DEV_PLATFORM
	void RenderShaderFragment::streamOut(std::ostream& os) const
	{
		uint32_t tmp;
		tmp = Native2LE(mType);
		os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
		os.write(reinterpret_cast<char const *>(&mVer), sizeof(mVer));

		uint32_t len = static_cast<uint32_t>(mStr.size());
		tmp = Native2LE(len);
		os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
		os.write(&mStr[0], len * sizeof(mStr[0]));
	}
#endif
	SharedConstantBuffer::SharedConstantBuffer()
	{
		mCBuffer = MakeSharedPtr<RenderEffectConstantBuffer>();
	}

	RenderEffectConstantBufferPtr SharedConstantBuffer::getCBuffer()
	{
		return mCBuffer;
	}

	RenderEffectParameter* SharedConstantBuffer::getParameterByIndex(uint32_t index) const
	{
		return mParams[index].get();
	}

	RenderEffectParameter* SharedConstantBuffer::getParameterByName(std::string_view name) const
	{
		size_t name_hash = boost::hash_range(name.begin(), name.end());
		for (auto const & it : mParams)
		{
			if (it->getNameHash() == name_hash)
			{
				return it.get();
			}
		}
		return nullptr;
	}
}