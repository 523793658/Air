#include "Engine.h"
#include "basic/include/Util.h"
#include "D3D11RenderEngine.hpp"


#include "render_engine_d3d11/include/D3D11RenderFactory.hpp"
#include "render_engine_d3d11/include/D3D11RenderFactoryInternal.hpp"
namespace Air
{

	std::unique_ptr<RenderEngine> D3D11RenderFactory::doMakeRenderEngine()
	{
		return MakeUniquePtr<D3D11RenderEngine>();
	}
}

void makeRenderFactory(std::unique_ptr<Air::RenderFactory>& ptr)
{
	ptr = Air::MakeUniquePtr<Air::D3D11RenderFactory>();
}
