#ifndef _D3D11_Mapping_H_
#define _D3D11_Mapping_H_
#pragma once
#include "PreDeclare.h"
#include "basic/include/ArrayRef.hpp"
#include "ElementFormat.h"
#include "D3D11Typedefs.hpp"
#include "rendersystem/include/RenderLayout.hpp"
#include "rendersystem/include/RenderStateObject.hpp"
#include "rendersystem/include/ShaderObject.hpp"
namespace Air
{
	class D3D11Mapping
	{
	public:
		static DXGI_FORMAT MappingFormat(ElementFormat format);
		static ElementFormat MappingFormat(DXGI_FORMAT format);
		static uint32_t MappingColorMask(uint32_t mask);

		static D3D11_PRIMITIVE_TOPOLOGY mapping(RenderLayout::TopologyType type);

		static D3D11_FILL_MODE mapping(PolygonMode mode);
		static D3D11_CULL_MODE mapping(CullMode mode);
		static D3D11_STENCIL_OP mapping(StencilOperation op);
		static D3D11_COMPARISON_FUNC mapping(CompareFunction cmp);
		static D3D11_DEPTH_WRITE_MASK mapping(bool mask);
		static D3D11_BLEND_OP mapping(BlendOperation op);
		static D3D11_BLEND mapping(AlphaBlendFactor fa);
		static D3D11_LOGIC_OP mapping(LogicOperation op);
		static D3D11_TEXTURE_ADDRESS_MODE mapping(TexAddressingMode mode);
		static D3D11_FILTER mapping(TexFilterOp filter);
		static void mapping(std::vector<D3D11_INPUT_ELEMENT_DESC>& elements, size_t stream, ArrayRef<VertexElement> vet, RenderLayout::StreamType type, uint32_t freq);

		static D3D11_SO_DECLARATION_ENTRY mapping(ShaderDesc::StreamOutputDecl const &decl);
		
	};
}




#endif