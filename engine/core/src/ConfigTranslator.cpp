#include "Context.h"
#include "rendersystem/include/FrameBuffer.hpp"
#include "boost/functional/hash.hpp"
#include "ConfigTranslator.hpp"
namespace Air
{
	FrameBuffer::Attachment getAttachmentFromStr(std::string_view str)
	{
		size_t name_hash = boost::hash_range(str.begin(), str.end());
		switch (name_hash)
		{
		case CT_HASH("ATT_Color0"):
			return FrameBuffer::ATT_Color0;

		case CT_HASH("ATT_Color1"):
			return FrameBuffer::ATT_Color1;
		
		case CT_HASH("ATT_Color2"):
			return FrameBuffer::ATT_Color2;
		
		case CT_HASH("ATT_Color3"):
			return FrameBuffer::ATT_Color3;

		case CT_HASH("ATT_Color4"):
			return FrameBuffer::ATT_Color4;

		case CT_HASH("ATT_Color5"):
			return FrameBuffer::ATT_Color5;

		case CT_HASH("ATT_Color6"):
			return FrameBuffer::ATT_Color6;

		case CT_HASH("ATT_COLOR7"):
			return FrameBuffer::ATT_Color7;

		case CT_HASH("ATT_DepthStencil"):
			return FrameBuffer::ATT_DepthStencil;

		default:
			AIR_UNREACHABLE("Invalid Attachment name.");
			break;
		}
	}

	RenderLayerPassType getRLPTFromStr(std::string_view str)
	{
		size_t name_hash = boost::hash_range(str.begin(), str.end());
		switch (name_hash)
		{
		case CT_HASH("RLPT_ShadowMap"):
			return RLPT_ShadowMap;

		case CT_HASH("RLPT_ForwardRendering"):
			return RLPT_ForwardRendering;

		case CT_HASH("RLPT_SkyBox"):
			return RLPT_SkyBox;

		case CT_HASH("RLPT_Helper"):
			return RLPT_Helper;

		default:
			AIR_UNREACHABLE("invalid Pass type.");
			break;
		}
	}
}