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
				v.pos.x() = sin((i - n)* dgy) * radius;
				v.pos.y() = cos((i - n) * dgy) * sin(j * dgx) * radius;
				v.pos.z() = cos((i - n) * dgy) * cos(j * dgx) * radius;
				v.normal = v.pos;
				aabb.join(v.pos);
				if (i < rows - 1 && j < cels - 1)
				{
					index[(i * (cels - 1) + j) * 6] = (i * cels + j);
					index[(i * (cels - 1) + j) * 6 + 2] = (i * cels + j + 1);
					index[(i * (cels - 1) + j) * 6 + 1] = ((i + 1) * cels + j + 1);

					index[(i * (cels - 1) + j) * 6 + 3] = (i * cels + j);
					index[(i * (cels - 1) + j) * 6 + 5] = ((i + 1) * cels + j + 1);
					index[(i * (cels - 1) + j) * 6 + 4] = ((i + 1) * cels + j);
				}
			}
		}
		GraphicsBufferPtr vb = rf.makeVertexBuffer(BU_Static, EAH_GPU_Read | EAH_Immutable, sizeof(VertexData) * rows * cels, data);
		mesh->addVertexStream(vb, { VertexElement(VEU_Position, 0, EF_BGR32F), VertexElement(VEU_Normal, 0, EF_BGR32F), VertexElement(VEU_TextureCoord, 0, EF_GR32F) });
		GraphicsBufferPtr ib = rf.makeIndexBuffer(BU_Static, EAH_GPU_Read | EAH_Immutable, sizeof(uint16_t) * (rows - 1) * (cels - 1) * 2 * 3, index);
		mesh->addIndexStream(ib, EF_R16UI);
		mesh->setPosAABB(aabb);
		return mesh;
	}

	StaticMeshPtr SimpleMeshFactory::createStaticCube()
	{
		RenderFactory& rf = Engine::getInstance().getRenderFactoryInstance();
		StaticMeshPtr mesh = MakeSharedPtr<StaticMesh>(nullptr, L"cube");
		VertexData* data = new VertexData[24]{
			//前面
			{ {0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f} } ,
			{ { 0.5f, -0.5f, -0.5f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 1.0f } },
			{ { -0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
			{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } },
			//后面
			{ { -0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
			{ { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
			{ { 0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
			{ { 0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
			//左面
			{ { -0.5f, 0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
			{ { -0.5f, -0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
			{ { -0.5f, 0.5f, 0.5f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
			{ { -0.5f, -0.5f, 0.5f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
			//右面
			{ { 0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
			{ { 0.5f, -0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
			{ { 0.5f, 0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
			{ { 0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
			//上面
			{ { 0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
			{ { 0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
			{ { -0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
			{ { -0.5f, 0.5f, -0.5f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f } },
			
			{ { 0.5f, -0.5f, -0.5f },{ 0.0f, -1.0f, 0.0f },{ 1.0f, 0.0f } },
			{ { 0.5f, -0.5f, 0.5f },{ 0.0f, -1.0f, 0.0f },{ 1.0f, 1.0f } },
			{ { -0.5f, -0.5f, -0.5f },{ 0.0f, -1.0f, 0.0f },{ 0.0f, 0.0f } },
			{ { -0.5f, -0.5f, 0.5f },{ 0.0f, -1.0f, 0.0f },{ 0.0f, 1.0f } }
		};
		uint16_t indexData[36] = {
			2, 0, 1, 3, 2, 1,
			6, 4, 5, 7, 6, 5,
			10, 8, 9, 11, 10, 9,
			14, 12, 13, 15, 14, 13,
			18, 16, 17, 19, 18, 17,
			22, 20, 21, 23, 22, 21,
		};

		uint16_t* index = static_cast<uint16_t*>(malloc(sizeof(uint16_t) * 36));
		memcpy(index, indexData, sizeof(uint16_t) * 36);

		

		GraphicsBufferPtr vb = rf.makeVertexBuffer(BU_Static, EAH_GPU_Read | EAH_Immutable, sizeof(VertexData) * 24, data);
		mesh->addVertexStream(vb, { VertexElement(VEU_Position, 0, EF_BGR32F), VertexElement(VEU_Normal, 0, EF_BGR32F), VertexElement(VEU_TextureCoord, 0, EF_GR32F) });
		GraphicsBufferPtr ib = rf.makeIndexBuffer(BU_Static, EAH_GPU_Read | EAH_Immutable, sizeof(uint16_t) * 36, index);
		mesh->addIndexStream(ib, EF_R16UI);
		mesh->setPosAABB(AABBox(float3(-0.5, -0.5, -0.5), float3(0.5, 0.5, 0.5)));
		return mesh;
	}

	StaticMeshPtr SimpleMeshFactory::createStaticQuat(float halfWidth /* = 0.5f */)
	{
		RenderFactory& rf = Engine::getInstance().getRenderFactoryInstance();
		StaticMeshPtr mesh = MakeSharedPtr<StaticMesh>(nullptr, L"quat");
		float2 pos[] =
		{
			float2(-halfWidth, +halfWidth),
			float2(+halfWidth, +halfWidth),
			float2(-halfWidth, -halfWidth),
			float2(+halfWidth, -halfWidth)
		};

		GraphicsBufferPtr vb = rf.makeVertexBuffer(BU_Static, EAH_GPU_Read | EAH_Immutable, sizeof(float2) * 4, pos);
		mesh->addVertexStream(vb, VertexElement(VEU_Position, 0, EF_GR32F));
		mesh->setPosAABB(AABBox(float3(-halfWidth, -halfWidth, 0.0), float3(halfWidth, halfWidth, 0.0)));
		mesh->setTopologyType(RenderLayout::TT_TriangleStrip);
		return mesh;
	}
}
