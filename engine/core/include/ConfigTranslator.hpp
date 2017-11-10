#ifndef _ConfigTranslator_H_
#define _ConfigTranslator_H_
#pragma once
#include "PreDeclare.h"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/RenderLayerPass.hpp"
namespace Air
{
	FrameBuffer::Attachment getAttachmentFromStr(std::string_view str);
	RenderLayerPassType getRLPTFromStr(std::string_view str);
	

}
#endif
