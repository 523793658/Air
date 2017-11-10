#ifndef _RenderableHelper_H_
#define _RenderableHelper_H_
#pragma once

#include "rendersystem/include/Renderable.hpp"

namespace Air
{
	class AIR_CORE_API RenderableHelper : public Renderable
	{
	public:
		explicit RenderableHelper(std::wstring const & name);
		virtual ~RenderableHelper();

		virtual RenderLayout& getRenderLayout() const;
		virtual AABBox const & getPosAABB() const;
		virtual AABBox const & getTexcoordAABB() const;
		virtual std::wstring const & getName() const;
		virtual bool isSkinned() const
		{
			return false;
		}
	protected:
		std::wstring mName;
		AABBox mAABB;
		AABBox mTCAABB;
		RenderLayoutPtr mRenderlayout;
		RenderEffectParameter* mColor;
	};

	class AIR_CORE_API WireFrame : public RenderableHelper
	{
	public:
		explicit WireFrame();
		~WireFrame();
		void setGeometryData(std::vector<float3> mPosition, std::vector<uint16_t> indices);

	};
}

#endif