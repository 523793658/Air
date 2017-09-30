#include "Engine.h"
#include "basic/include/ErrorHanding.hpp"
#include "D3D11RenderEngine.hpp"
#include "D3D11Mapping.hpp"

namespace Air
{
	DXGI_FORMAT D3D11Mapping::MappingFormat(ElementFormat format)
	{
		switch (format)
		{
		case Air::EF_Unknown:
			break;
		case Air::EF_A8:
			return DXGI_FORMAT_A8_UNORM;
		case Air::EF_R5G6B5:
			return DXGI_FORMAT_B5G6R5_UNORM;
		case Air::EF_A1RGB5:
			return DXGI_FORMAT_B5G5R5A1_UNORM;
		case Air::EF_ARGB4:
			return DXGI_FORMAT_B4G4R4A4_UNORM;
		case Air::EF_R8:
			return DXGI_FORMAT_R8_UNORM;
		case Air::EF_SIGNED_R8:
			return DXGI_FORMAT_R8_SNORM;
		case Air::EF_GR8:
			return DXGI_FORMAT_R8G8_UNORM;
		case Air::EF_SIGNED_GR8:
			return DXGI_FORMAT_R8G8_SNORM;
		case Air::EF_ARGB8:
			return DXGI_FORMAT_B8G8R8A8_UNORM;
		case Air::EF_ABGR8:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		case Air::EF_SIGNED_ABGR8:
			return DXGI_FORMAT_R8G8B8A8_SNORM;
		case Air::EF_A2BGR10:
			return DXGI_FORMAT_R10G10B10A2_UNORM;
		case Air::EF_SIGNED_A2BGR10:
			return DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
		case Air::EF_R8UI:
			return DXGI_FORMAT_R8_UINT;
		case Air::EF_R8I:
			return DXGI_FORMAT_R8_SINT;
		case Air::EF_GR8UI:
			return DXGI_FORMAT_R8G8_UINT;
		case Air::EF_GR8I:
			return DXGI_FORMAT_R8G8_SINT;
		case Air::EF_BGR8UI:
			return DXGI_FORMAT_R8G8B8A8_UINT;
		case Air::EF_BGR8I:
			return DXGI_FORMAT_R8G8B8A8_SINT;
		case Air::EF_ABGR8UI:
			return DXGI_FORMAT_R8G8B8A8_UINT;
		case Air::EF_ABGR8I:
			return DXGI_FORMAT_R8G8B8A8_SINT;
		case Air::EF_A2BGR10UI:
			return DXGI_FORMAT_R10G10B10A2_UINT;
		case Air::EF_R16:
			return DXGI_FORMAT_R16_UNORM;
		case Air::EF_SIGNED_R16:
			return DXGI_FORMAT_R16_SNORM;
		case Air::EF_GR16:
			return DXGI_FORMAT_R16G16_UNORM;
		case Air::EF_SIGNED_GR16:
			return DXGI_FORMAT_R16G16_SNORM;
		case Air::EF_ABGR16:
			return DXGI_FORMAT_R16G16B16A16_UNORM;
		case Air::EF_SIGNED_ABGR16:
			return DXGI_FORMAT_R16G16B16A16_SNORM;
		case Air::EF_R16UI:
			return DXGI_FORMAT_R16_UINT;
		case Air::EF_R16I:
			return DXGI_FORMAT_R16_SINT;
		case Air::EF_GR16UI:
			return DXGI_FORMAT_R16G16_UINT;
		case Air::EF_GR16I:
			return DXGI_FORMAT_R16G16_SINT;
		case Air::EF_ABGR16UI:
			return DXGI_FORMAT_R16G16B16A16_UINT;
		case Air::EF_ABGR16I:
			return DXGI_FORMAT_R16G16B16A16_SINT;
		case Air::EF_R32UI:
			return DXGI_FORMAT_R32_UINT;
		case Air::EF_R32I:
			return DXGI_FORMAT_R32_SINT;
		case Air::EF_GR32UI:
			return DXGI_FORMAT_R32G32_UINT;
		case Air::EF_GR32I:
			return DXGI_FORMAT_R32G32_SINT;
		case Air::EF_BGR32UI:
			return DXGI_FORMAT_R32G32B32_UINT;
		case Air::EF_BGR32I:
			return DXGI_FORMAT_R32G32B32_SINT;
		case Air::EF_ABGR32UI:
			return DXGI_FORMAT_R32G32B32A32_UINT;
		case Air::EF_ABGR32I:
			return DXGI_FORMAT_R32G32B32A32_SINT;
		case Air::EF_R16F:
			return DXGI_FORMAT_R16_FLOAT;
		case Air::EF_GR16F:
			return DXGI_FORMAT_R16G16_FLOAT;
		case Air::EF_B10G11R11F:
			return DXGI_FORMAT_R11G11B10_FLOAT;
		case Air::EF_ABGR16F:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case Air::EF_R32F:
			return DXGI_FORMAT_R32_FLOAT;
		case Air::EF_GR32F:
			return DXGI_FORMAT_R32G32_FLOAT;
		case Air::EF_BGR32F:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		case Air::EF_ABGR32F:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case Air::EF_BC1:
			return DXGI_FORMAT_BC1_UNORM;
		case Air::EF_BC2:
			return DXGI_FORMAT_BC2_UNORM;
		case Air::EF_BC3:
			return DXGI_FORMAT_BC3_UNORM;
		case Air::EF_BC4:
			return DXGI_FORMAT_BC4_UNORM;
		case Air::EF_SIGNED_BC4:
			return DXGI_FORMAT_BC4_SNORM;
		case Air::EF_BC5:
			return DXGI_FORMAT_BC5_UNORM;
		case Air::EF_SIGNED_BC5:
			return DXGI_FORMAT_BC5_SNORM;
		case Air::EF_BC6:
			return DXGI_FORMAT_BC6H_UF16;
		case Air::EF_SIGNED_BC6:
			return DXGI_FORMAT_BC6H_SF16;
		case Air::EF_BC7:
			return DXGI_FORMAT_BC7_UNORM;
		case Air::EF_D16:
			return DXGI_FORMAT_D16_UNORM;
		case Air::EF_D24S8:
			return DXGI_FORMAT_D24_UNORM_S8_UINT;
		case Air::EF_D32F:
			return DXGI_FORMAT_D32_FLOAT;
		case Air::EF_ARGB8_SRGB:
			return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		case Air::EF_ABGR8_SRGB:
			return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		case Air::EF_BC1_SRGB:
			return DXGI_FORMAT_BC1_UNORM_SRGB;
		case Air::EF_BC2_SRGB:
			return DXGI_FORMAT_BC2_UNORM_SRGB;
		case Air::EF_BC3_SRGB:
			return DXGI_FORMAT_BC3_UNORM_SRGB;
		case Air::EF_BC7_SRGB:
			return DXGI_FORMAT_BC7_UNORM_SRGB;
		default:
			TERRC(std::errc::function_not_supported);
		}
		return DXGI_FORMAT_UNKNOWN;
	}

	ElementFormat D3D11Mapping::MappingFormat(DXGI_FORMAT format)
	{
		switch (format)
		{
		case DXGI_FORMAT_A8_UNORM:
			return EF_A8;

		case DXGI_FORMAT_B5G6R5_UNORM:
			return EF_R5G6B5;

		case DXGI_FORMAT_B5G5R5A1_UNORM:
			return EF_A1RGB5;

		case DXGI_FORMAT_B4G4R4A4_UNORM:
			return EF_ARGB4;

		case DXGI_FORMAT_R8_UNORM:
			return EF_R8;

		case DXGI_FORMAT_R8_SNORM:
			return EF_SIGNED_R8;

		case DXGI_FORMAT_R8G8_UNORM:
			return EF_GR8;

		case DXGI_FORMAT_R8G8_SNORM:
			return EF_SIGNED_GR8;

		case DXGI_FORMAT_B8G8R8A8_UNORM:
			return EF_ARGB8;

		case DXGI_FORMAT_R8G8B8A8_UNORM:
			return EF_ABGR8;

		case DXGI_FORMAT_R8G8B8A8_SNORM:
			return EF_SIGNED_ABGR8;

		case DXGI_FORMAT_R10G10B10A2_UNORM:
			return EF_A2BGR10;

		case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
			return EF_SIGNED_A2BGR10;

		case DXGI_FORMAT_R8_UINT:
			return EF_R8UI;

		case DXGI_FORMAT_R8_SINT:
			return EF_R8I;

		case DXGI_FORMAT_R8G8_UINT:
			return EF_GR8UI;

		case DXGI_FORMAT_R8G8_SINT:
			return EF_GR8I;

		case DXGI_FORMAT_R8G8B8A8_UINT:
			return EF_ABGR8UI;

		case DXGI_FORMAT_R8G8B8A8_SINT:
			return EF_ABGR8I;

		case DXGI_FORMAT_R10G10B10A2_UINT:
			return EF_A2BGR10UI;

		case DXGI_FORMAT_R16_UNORM:
			return EF_R16;

		case DXGI_FORMAT_R16_SNORM:
			return EF_SIGNED_R16;

		case DXGI_FORMAT_R16G16_UNORM:
			return EF_GR16;

		case DXGI_FORMAT_R16G16_SNORM:
			return EF_SIGNED_GR16;

		case DXGI_FORMAT_R16G16B16A16_UNORM:
			return EF_ABGR16;

		case DXGI_FORMAT_R16G16B16A16_SNORM:
			return EF_SIGNED_ABGR16;

		case DXGI_FORMAT_R16_UINT:
			return EF_R16UI;

		case DXGI_FORMAT_R16_SINT:
			return EF_R16I;

		case DXGI_FORMAT_R16G16_UINT:
			return EF_GR16UI;

		case DXGI_FORMAT_R16G16_SINT:
			return EF_GR16I;

		case DXGI_FORMAT_R16G16B16A16_UINT:
			return EF_ABGR16UI;

		case DXGI_FORMAT_R16G16B16A16_SINT:
			return EF_ABGR16I;

		case DXGI_FORMAT_R32_UINT:
			return EF_R32UI;

		case DXGI_FORMAT_R32_SINT:
			return EF_R32I;

		case DXGI_FORMAT_R32G32_UINT:
			return EF_GR32UI;

		case DXGI_FORMAT_R32G32_SINT:
			return EF_GR32I;

		case DXGI_FORMAT_R32G32B32_UINT:
			return EF_BGR32UI;

		case DXGI_FORMAT_R32G32B32_SINT:
			return EF_BGR32I;

		case DXGI_FORMAT_R32G32B32A32_UINT:
			return EF_ABGR32UI;

		case DXGI_FORMAT_R32G32B32A32_SINT:
			return EF_ABGR32I;

		case DXGI_FORMAT_R16_FLOAT:
			return EF_R16F;

		case DXGI_FORMAT_R16G16_FLOAT:
			return EF_GR16F;

		case DXGI_FORMAT_R11G11B10_FLOAT:
			return EF_B10G11R11F;

		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			return EF_ABGR16F;

		case DXGI_FORMAT_R32_FLOAT:
			return EF_R32F;

		case DXGI_FORMAT_R32G32_FLOAT:
			return EF_GR32F;

		case DXGI_FORMAT_R32G32B32_FLOAT:
			return EF_BGR32F;

		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			return EF_ABGR32F;

		case DXGI_FORMAT_BC1_UNORM:
			return EF_BC1;

		case DXGI_FORMAT_BC2_UNORM:
			return EF_BC2;

		case DXGI_FORMAT_BC3_UNORM:
			return EF_BC3;

		case DXGI_FORMAT_BC4_UNORM:
			return EF_BC4;

		case DXGI_FORMAT_BC4_SNORM:
			return EF_SIGNED_BC4;

		case DXGI_FORMAT_BC5_UNORM:
			return EF_BC5;

		case DXGI_FORMAT_BC5_SNORM:
			return EF_SIGNED_BC5;

		case DXGI_FORMAT_BC6H_UF16:
			return EF_BC6;

		case DXGI_FORMAT_BC6H_SF16:
			return EF_SIGNED_BC6;

		case DXGI_FORMAT_BC7_UNORM:
			return EF_BC7;

		case DXGI_FORMAT_D16_UNORM:
			return EF_D16;

		case DXGI_FORMAT_D24_UNORM_S8_UINT:
			return EF_D24S8;

		case DXGI_FORMAT_D32_FLOAT:
			return EF_D32F;

		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
			return EF_ARGB8_SRGB;

		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
			return EF_ABGR8_SRGB;

		case DXGI_FORMAT_BC1_UNORM_SRGB:
			return EF_BC1_SRGB;

		case DXGI_FORMAT_BC2_UNORM_SRGB:
			return EF_BC2_SRGB;

		case DXGI_FORMAT_BC3_UNORM_SRGB:
			return EF_BC3_SRGB;

		case DXGI_FORMAT_BC7_UNORM_SRGB:
			return EF_BC7_SRGB;

		default:
			TERRC(std::errc::function_not_supported);
		}
	}

	D3D11_PRIMITIVE_TOPOLOGY D3D11Mapping::mapping(RenderLayout::TopologyType type)
	{
		switch (type)
		{
		case Air::RenderLayout::TT_PointList:
			return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

		case Air::RenderLayout::TT_LineList:
			return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

		case Air::RenderLayout::TT_LineStrip:
			return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;

		case Air::RenderLayout::TT_TriangleList:
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		case Air::RenderLayout::TT_TriangleStrip:
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

		case Air::RenderLayout::TT_LineList_Adj:
			return D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;

		case Air::RenderLayout::TT_LineStrip_Adj:
			return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;

		case Air::RenderLayout::TT_TriangleList_Adj:
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;

		case Air::RenderLayout::TT_TriangleStrip_Adj:
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;

		case Air::RenderLayout::TT_1_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_2_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_3_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_4_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_5_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_6_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_7_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_8_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_9_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_10_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_11_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_12_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_13_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_14_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_15_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_16_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_17_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_18_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_19_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_20_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_21_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_22_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_23_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_24_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_25_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_26_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_27_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_28_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_29_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_30_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_31_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST;

		case Air::RenderLayout::TT_32_Ctrl_Pt_PatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST;
		default:
			AIR_UNREACHABLE("Invalid topotogy type");
		}
	}


	uint32_t D3D11Mapping::MappingColorMask(uint32_t mask)
	{
		uint32_t ret = 0;
		if (mask & CMASK_Red)
		{
			ret |= D3D11_COLOR_WRITE_ENABLE_RED;
		}
		if (mask & CMASK_Green)
		{
			ret |= D3D11_COLOR_WRITE_ENABLE_GREEN;
		}
		if (mask & CMASK_Blue )
		{
			ret |= D3D11_COLOR_WRITE_ENABLE_BLUE;
		}
		if (mask & CMASK_Alpha)
		{
			ret |= D3D11_COLOR_WRITE_ENABLE_ALPHA;
		}
		return ret;
	}

	D3D11_FILL_MODE D3D11Mapping::mapping(PolygonMode mode)
	{
		switch (mode)
		{
		case Air::PM_Point:
		case Air::PM_Line:
			return D3D11_FILL_WIREFRAME;
		case Air::PM_Fill:
			return D3D11_FILL_SOLID;
		default:
			AIR_UNREACHABLE("Invalid polygon mode");
		}
	}
	D3D11_CULL_MODE D3D11Mapping::mapping(CullMode mode)
	{
		switch (mode)
		{
		case Air::CM_None:
			return D3D11_CULL_NONE;
		case Air::CM_Front:
			return D3D11_CULL_FRONT;
		case Air::CM_Back:
			return D3D11_CULL_BACK;
		default:
			AIR_UNREACHABLE("Invalid cullmode");
		}
	}
	D3D11_STENCIL_OP D3D11Mapping::mapping(StencilOperation op)
	{
		switch (op)
		{
		case Air::SOP_Keep:
			return D3D11_STENCIL_OP_KEEP;
		case Air::SOP_Zero:
			return D3D11_STENCIL_OP_ZERO;
		case Air::SOP_Replace:
			return D3D11_STENCIL_OP_REPLACE;
		case Air::SOP_Incr:
			return D3D11_STENCIL_OP_INCR;
		case Air::SOP_Decr:
			return D3D11_STENCIL_OP_DECR;
		case Air::SOP_Invert:
			return D3D11_STENCIL_OP_INVERT;
		case Air::SOP_Incr_Wrap:
			return D3D11_STENCIL_OP_INCR;
		case Air::SOP_Decr_Wrap:
			return D3D11_STENCIL_OP_DECR;
		default:
			AIR_UNREACHABLE("Invalid stencil operation");
		}
	}
	D3D11_COMPARISON_FUNC D3D11Mapping::mapping(CompareFunction cmp)
	{
		switch (cmp)
		{
		case Air::CF_AlwaysFail:
			return D3D11_COMPARISON_NEVER;
		case Air::CF_AlwaysPass:
			return D3D11_COMPARISON_ALWAYS;
		case Air::CF_Less:
			return D3D11_COMPARISON_LESS;
		case Air::CF_LessEqual:
			return D3D11_COMPARISON_LESS_EQUAL;
		case Air::CF_Equal:
			return D3D11_COMPARISON_EQUAL;
		case Air::CF_NotEqual:
			return D3D11_COMPARISON_NOT_EQUAL;
		case Air::CF_GreaterEqual:
			return D3D11_COMPARISON_GREATER_EQUAL;
		case Air::CF_Greater:
			return D3D11_COMPARISON_GREATER;
		default:
			AIR_UNREACHABLE("Invalid comparison func");

		}
	}
	D3D11_DEPTH_WRITE_MASK D3D11Mapping::mapping(bool mask)
	{
		return mask ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	}
	D3D11_BLEND_OP D3D11Mapping::mapping(BlendOperation op)
	{
		switch (op)
		{
		case Air::BOP_Add:
			return D3D11_BLEND_OP_ADD;
		case Air::BOP_Sub:
			return D3D11_BLEND_OP_SUBTRACT;
		case Air::BOP_Rev_Sub:
			return D3D11_BLEND_OP_REV_SUBTRACT;
		case Air::BOP_Min:
			return D3D11_BLEND_OP_MIN;
		case Air::BOP_Max:
			return D3D11_BLEND_OP_MAX;
		default:
			AIR_UNREACHABLE("Invalid Blend Operation");
		}
	}
	D3D11_BLEND D3D11Mapping::mapping(AlphaBlendFactor fa)
	{
		switch (fa)
		{
		case Air::ABF_Zero:
			return D3D11_BLEND_ZERO;
		case Air::ABF_One:
			return D3D11_BLEND_ONE;
		case Air::ABF_Src_Alpha:
			return D3D11_BLEND_SRC_ALPHA;
		case Air::ABF_Dst_Alpha:
			return D3D11_BLEND_DEST_ALPHA;
		case Air::ABF_Inv_Src_Alpha:
			return D3D11_BLEND_INV_SRC_ALPHA;
		case Air::ABF_Inv_Dst_Alpha:
			return D3D11_BLEND_INV_DEST_ALPHA;
		case Air::ABF_Src_Color:
			return D3D11_BLEND_SRC_COLOR;
		case Air::ABF_Dst_Color:
			return D3D11_BLEND_DEST_ALPHA;
		case Air::ABF_Inv_Src_Color:
			return D3D11_BLEND_INV_SRC_COLOR;
		case Air::ABF_Inv_Dst_Color:
			return D3D11_BLEND_INV_DEST_COLOR;
		case Air::ABF_Src_Alpha_Sat:
			return D3D11_BLEND_SRC_ALPHA_SAT;
		case Air::ABF_Blend_Factor:
			return D3D11_BLEND_BLEND_FACTOR;
		case Air::ABF_Inv_Blend_Factor:
			return D3D11_BLEND_INV_BLEND_FACTOR;
		case Air::ABF_Src1_Alpha:
			return D3D11_BLEND_SRC1_ALPHA;
		case Air::ABF_Inv_Src1_Alpha:
			return D3D11_BLEND_INV_SRC1_ALPHA;
		case Air::ABF_Src1_Color:
			return D3D11_BLEND_SRC1_COLOR;
		case Air::ABF_Inv_Src1_Color:
			return D3D11_BLEND_INV_SRC1_COLOR;
		default:
			AIR_UNREACHABLE("Invalid blend factor");
		}
	}
	D3D11_LOGIC_OP D3D11Mapping::mapping(LogicOperation op)
	{
		switch (op)
		{
		case Air::LOP_Clear:
			return D3D11_LOGIC_OP_CLEAR;
		case Air::LOP_Set:
			return D3D11_LOGIC_OP_SET;
		case Air::LOP_Copy:
			return D3D11_LOGIC_OP_COPY;
		case Air::LOP_CopyInverted:
			return D3D11_LOGIC_OP_COPY_INVERTED;
		case Air::LOP_Noop:
			return D3D11_LOGIC_OP_NOOP;
		case Air::LOP_Invert:
			return D3D11_LOGIC_OP_INVERT;
		case Air::LOP_And:
			return D3D11_LOGIC_OP_AND;
		case Air::LOP_NAnd:
			return D3D11_LOGIC_OP_NAND;
		case Air::LOP_Or:
			return D3D11_LOGIC_OP_OR;
		case Air::LOP_NOR:
			return D3D11_LOGIC_OP_NOR;
		case Air::LOP_XOR:
			return D3D11_LOGIC_OP_XOR;
		case Air::LOP_Equiv:
			return D3D11_LOGIC_OP_EQUIV;
		case Air::LOP_AndReverse:
			return D3D11_LOGIC_OP_AND_REVERSE;
		case Air::LOP_AndInverted:
			return D3D11_LOGIC_OP_AND_INVERTED;
		case Air::LOP_OrReverse:
			return D3D11_LOGIC_OP_OR_REVERSE;
		case Air::LOP_OrInverted:
			return D3D11_LOGIC_OP_OR_INVERTED;
		default:
			AIR_UNREACHABLE("Invalid logic operation");
		}
	}
	D3D11_TEXTURE_ADDRESS_MODE D3D11Mapping::mapping(TexAddressingMode mode)
	{
		switch (mode)
		{
		case Air::TAM_Wrap:
			return D3D11_TEXTURE_ADDRESS_WRAP;
		case Air::TAM_Mirror:
			return D3D11_TEXTURE_ADDRESS_MIRROR;
		case Air::TAM_Clamp:
			return D3D11_TEXTURE_ADDRESS_CLAMP;
		case Air::TAM_Border:
			return D3D11_TEXTURE_ADDRESS_BORDER;
		default:
			AIR_UNREACHABLE("Invalid texture address mode");
		}
	}
	D3D11_FILTER D3D11Mapping::mapping(TexFilterOp filter)
	{
		switch (filter)
		{
		case Air::TFO_Min_Mag_Mip_Point:
			return D3D11_FILTER_MIN_MAG_MIP_POINT;

		case Air::TFO_Min_Mag_Point_Mip_Linear:
			return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;

		case Air::TFO_Min_Point_Mag_Linear_Mip_Point:
			return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;

		case Air::TFO_Min_Linear_Mag_Mip_Point:
			return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;

		case Air::TFO_Min_Mag_Linear_Mip_Point:
			return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;

		case Air::TFO_Min_Linear_Mag_Point_Mip_Linear:
			return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;

		case Air::TFO_Min_Point_Mag_Mip_Linear:
			return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;

		case Air::TFO_Min_Mag_Mip_Linear:
			return D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		case Air::TFO_Anisotropic:
			return D3D11_FILTER_ANISOTROPIC;

		case Air::TFO_Cmp_Min_Mag_Mip_Point:
			return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;

		case Air::TFO_Cmp_Min_Mag_Point_Mip_Linear:
			return D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;

		case Air::TFO_Cmp_Min_Point_Mag_Linear_Mip_Point:
			return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;

		case Air::TFO_Cmp_Min_Point_Mag_Mip_Linear:
			return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;

		case Air::TFO_Cmp_Min_Linear_Mag_Mip_Point:
			return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;

		case Air::TFO_Cmp_Min_Linear_Mag_Point_Mip_Linear:
			return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;

		case Air::TFO_Cmp_Min_Mag_Linear_Mip_Point:
			return D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;

		case Air::TFO_Cmp_Min_Mag_Mip_Linear:
			return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;

		case Air::TFO_Cmp_Anisotropic:
			return D3D11_FILTER_COMPARISON_ANISOTROPIC;
		default:
			AIR_UNREACHABLE("Invalid filter mode");
		}
	}

	void D3D11Mapping::mapping(std::vector<D3D11_INPUT_ELEMENT_DESC>& elements, size_t stream, ArrayRef<VertexElement> vet, RenderLayout::StreamType type, uint32_t freq)
	{
		elements.resize(vet.size());
		uint16_t elem_offset = 0;
		for (uint32_t i = 0; i < elements.size(); ++i)
		{
			VertexElement const & vs_elem = vet[i];
			D3D11_INPUT_ELEMENT_DESC& element = elements[i];
			element.SemanticIndex = vs_elem.mUsageIndex;
			element.Format = D3D11Mapping::MappingFormat(vs_elem.mFormat);
			element.InputSlot = static_cast<WORD>(stream);
			element.AlignedByteOffset = elem_offset;
			if (RenderLayout::ST_Geometry == type)
			{
				element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				element.InstanceDataStepRate = 0;
			}
			else
			{
				BOOST_ASSERT(RenderLayout::ST_Instance == type);
				element.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
					element.InstanceDataStepRate = freq;
			}
			switch (vs_elem.mUsage)
			{
			case VEU_Position:
				element.SemanticName = "POSITION";
				break;
			case VEU_Normal:
				element.SemanticName = "NORMAL";
				break;
			case VEU_Diffuse:
				element.SemanticName = "COLOR";
				break;
			case VEU_Specular: 
				element.SemanticName = "COLOR";
				break;
			case VEU_BlendWeight:
				element.SemanticName = "BLENDWEIGHT";
				break;
			case VEU_BlendIndex:
				element.SemanticName = "BLENDINDICES";
				if (EF_ABGR8 == vs_elem.mFormat)
				{
					element.Format = DXGI_FORMAT_R8G8B8A8_UINT;
				}
				break;
			case VEU_TextureCoord:
				element.SemanticName = "TEXCOORD";
				break;
			case VEU_Tangent:
				element.SemanticName = "TANGENT";
				break;
			case VEU_Binormal:
				element.SemanticName = "BINORMAL";
				break;
			}
			elem_offset = static_cast<uint16_t>(elem_offset + vs_elem.getElementSize());
		}
	}


	D3D11_SO_DECLARATION_ENTRY D3D11Mapping::mapping(ShaderDesc::StreamOutputDecl const &decl)
	{
		D3D11_SO_DECLARATION_ENTRY ret;
		ret.Stream = 0;
		ret.SemanticIndex = decl.mUsageIndex;
		ret.StartComponent = decl.mStartComponent;
		ret.ComponentCount = decl.mComponentCount;
		ret.OutputSlot = decl.mSlot;
		switch (decl.mUsage)
		{
		case VEU_Position:
			ret.SemanticName = "SV_Position";
			break;
		case VEU_Normal:
			ret.SemanticName = "NORMAL";
			break;
		case VEU_Diffuse:
			ret.SemanticName = "COLOR";
			break;
		case VEU_Specular:
			ret.SemanticName = "COLOR";
			break;
		case VEU_BlendWeight:
			ret.SemanticName = "BLENDWEIGHT";
			break;
		case VEU_BlendIndex:
			ret.SemanticName = "BLENDINDEICES";
			break;
		case VEU_TextureCoord:
			ret.SemanticName = "TEXCOORD";
			break;
		case VEU_Tangent:
			ret.SemanticName = "TANGENT";
			break;
		case VEU_Binormal:
			ret.SemanticName = "BINORMAL";
			break;
		}
		return ret;
	}

}