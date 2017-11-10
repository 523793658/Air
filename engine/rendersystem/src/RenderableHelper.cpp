#include "Context.h"
#include "SingletonManager.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/RenderEffect.hpp"
#include "rendersystem/include/RenderableHelper.hpp"

namespace Air
{
	RenderableHelper::RenderableHelper(std::wstring const & name)
		:mName(name)
	{

	}
	RenderLayout& RenderableHelper::getRenderLayout() const
	{
		return *mRenderlayout;
	}

	AABBox const & RenderableHelper::getPosAABB() const
	{
		return mAABB;
	}

	AABBox const & RenderableHelper::getTexcoordAABB() const
	{
		return mTCAABB;
	}
	std::wstring const & RenderableHelper::getName() const
	{
		return mName;
	}

	RenderableHelper:: ~RenderableHelper()
	{

	}

	WireFrame::WireFrame()
		:RenderableHelper(L"frame")
	{

	}
	WireFrame::~WireFrame()
	{

	}
	void WireFrame::setGeometryData(std::vector<float3> position, std::vector<uint16_t> indices)
	{
		RenderFactory& rf = SingletonManager::getRenderFactoryInstance();
		RenderLayoutPtr layout = rf.MakeRenderLayout();
		char* data = (char*)malloc(position.size() * sizeof(float3));
		memcpy(data, &position[0], position.size() * sizeof(float3));
		GraphicsBufferPtr vb = rf.makeVertexBuffer(BU_Static, EAH_GPU_Read | EAH_Immutable, position.size() * sizeof(float3), data);

		data = (char*)malloc(indices.size() * sizeof(uint16_t));
		memcpy(data, &indices[0], indices.size() * sizeof(uint16_t));
		GraphicsBufferPtr ib = rf.makeIndexBuffer(BU_Static, EAH_GPU_Read | EAH_Immutable, indices.size() * sizeof(uint16_t), data);

		layout->bindVertexStream(vb, VertexElement(VEU_Position, 0, EF_BGR32F), RenderLayout::ST_Geometry);
		layout->bindIndexStream(ib, EF_R16UI);
		layout->setTopologyType(RenderLayout::TT_LineList);
		mRenderlayout = layout;
		mEffect = syncLoadRenderEffect("assets/shader/simplest.asd");
		mTechnique = mEffect->getTechniqueByName("simplest");
		this->bindEffect(mEffect);
		for (auto i : position)
		{
			mAABB.join(i);
		}
	}

}