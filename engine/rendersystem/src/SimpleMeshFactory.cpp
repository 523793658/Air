#include "Engine.h"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/SimpleMeshFactory.hpp"
#include "basic/include/Quaternion.hpp"

#include "rendersystem/include/RenderLayout.hpp"
#include "rendersystem/include/Mesh.hpp"
#include "basic/include/Math.hpp"
namespace Air
{
	

	StaticMeshPtr SimpleMeshFactory::createStaticShpere(float radius)
	{
		RenderFactory& rf = Engine::getInstance().getRenderFactoryInstance();
		StaticMeshPtr mesh = MakeSharedPtr<StaticMesh>(nullptr, L"sphere");
		int n = 10;
		int rows = n * 2 + 1;
		int cels = n * 4 + 1;
		int i, j;
		float tcdx = 1.0f / (rows - 1);
		float tcdy = 1.0f / (cels - 1);
		float dgy = PI / (rows - 1);
		float dgx = 2 * PI / (cels - 1);
		uint16_t* index = static_cast<uint16_t*>(malloc(sizeof(uint16_t) * (rows - 1) * (cels - 1) * 2 * 3));

		VertexData* data = new VertexData[rows * cels];

		AABBox aabb;

		for (i = 0; i < rows; i++)
		{
			for (j = 0; j < cels; j++)
			{
				VertexData& v = data[i * cels + j];
				v.texCoord.x() = tcdx;
				v.texCoord.y() = j * tcdy;
				v.pos.x() = sin((n - i)* dgy) * radius;
				v.pos.y() = cos((n - i) * dgy) * sin(j * dgx) * radius;
				v.pos.z() = cos((n - i) * dgy) * cos(j * dgx) * radius;
				aabb.join(v.pos);
				if (i < rows - 1 && j < cels - 1)
				{
					index[(i * (cels - 1) + j) * 6] = (i * cels + j);
					index[(i * (cels - 1) + j) * 6 + 1] = (i * cels + j + 1);
					index[(i * (cels - 1) + j) * 6 + 2] = ((i + 1) * cels + j + 1);

					index[(i * (cels - 1) + j) * 6 + 3] = (i * cels + j);
					index[(i * (cels - 1) + j) * 6 + 4] = ((i + 1) * cels + j + 1);
					index[(i * (cels - 1) + j) * 6 + 5] = ((i + 1) * cels + j);
				}
			}
		}
		GraphicsBufferPtr vb = rf.makeVertexBuffer(BU_Static, EAH_GPU_Read | EAH_Immutable, sizeof(VertexData) * rows * cels, data);
		mesh->addVertexStream(vb, std::make_tuple(VertexElement(VEU_Position, 0, EF_BGR32F), VertexElement(VEU_Normal, 0, EF_BGR32F), VertexElement(VEU_TextureCoord, 0, EF_GR32F)));
		GraphicsBufferPtr ib = rf.makeIndexBuffer(BU_Static, EAH_GPU_Read | EAH_Immutable, sizeof(uint16_t) * (rows - 1) * (cels - 1) * 2 * 3, index);
		mesh->addIndexStream(ib, EF_R16UI);
		mesh->setPosAABB(aabb);
		return mesh;
	}
}
