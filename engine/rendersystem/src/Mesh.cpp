#include "Engine.h"
#include <sstream>
#include "ResLoader.h"
#include "basic/include/Util.h"
#include "basic/include/Quaternion.hpp"
#include "basic/include/Log.hpp"
#include "rendersystem/include/GraphicsBuffer.hpp"
#include "rendersystem/include/RenderMaterial.hpp"
#include "rendersystem/include/RenderLayout.hpp"
#include "rendersystem/include/Mesh.hpp"
#include "packing_system/include/LZMACodec.h"
#include "rendersystem/include/RenderDeviceCaps.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/RenderEngine.hpp"

namespace
{
	using namespace Air;
	uint32_t const MODEL_BIN_VERSION = 1;

	class RenderModelLoadingDesc : public ResLoadingDesc
	{
	private:
		struct ModelDesc
		{
			std::string mResName;
			uint32_t mAccessHint;

			std::function<RenderModelPtr(std::wstring const &)> mCreateModelFactoryFunc;
			std::function<StaticMeshPtr(RenderModelPtr const &, std::wstring const &)> mCreateMeshFactoryFunc;

			struct ModelData {
				std::vector<VertexElement> mMergedVes;
				char mAllIsIndex16Bit;
				std::vector<std::vector<uint8_t>> mMergedBuffer;
				std::vector<uint8_t> mMergedIndex;
				std::vector<GraphicsBufferPtr> mMergedVBs;
				GraphicsBufferPtr mMergedIB;
				std::vector<std::wstring> mMeshNames;
				std::vector<int32_t> mMaterialIds;
				std::vector<AABBox> mPosBox;
				std::vector<AABBox> mTexcoordBox;
				std::vector<uint32_t> mMeshNumVertices;
				std::vector<uint32_t> mMeshBaseVertices;
				std::vector<uint32_t> mMeshNumIndices;
				std::vector<uint32_t> mMeshStartIndices;
				std::vector<Joint> mJoints;
				std::shared_ptr<AnimationActionsType> mActions;
				std::shared_ptr<KeyFramesType> mKeyFrames;
				uint32_t mNumFrames;
				uint32_t mFrameRate;
				std::vector<std::shared_ptr<AABBKeyFrames>> mFramePosAABB;
			};
			std::shared_ptr<ModelData> mModelData;
			std::shared_ptr<RenderModelPtr> mModel;
		};
	public:
		RenderModelLoadingDesc(std::wstring const &resName, uint32_t accessHint, std::function<RenderModelPtr(std::wstring const &)> createModelFactoryFunc, std::function<StaticMeshPtr(RenderModelPtr const &, std::wstring const &)> createMeshFactoryFunc)
		{

		}
		uint64_t getType() const
		{
			static uint64_t const type = CT_HASH("RenderModelLoadingDesc");
			return type;
		}
		bool getStateLess() const
		{
			return false;
		}



		void subThreadStage()
		{
			loadModel(mModelDesc.mResName, mModelDesc.mModelData->mMergedVes, mModelDesc.mModelData->mAllIsIndex16Bit, 
				mModelDesc.mModelData->mMergedBuffer, mModelDesc.mModelData->mMergedIndex, mModelDesc.mModelData->mMeshNames, 
				mModelDesc.mModelData->mMaterialIds, mModelDesc.mModelData->mPosBox, mModelDesc.mModelData->mTexcoordBox, 
				mModelDesc.mModelData->mMeshNumVertices, mModelDesc.mModelData->mMeshBaseVertices, 
				mModelDesc.mModelData->mMeshNumIndices, mModelDesc.mModelData->mMeshBaseVertices);
			RenderFactory&rf = Engine::getInstance().getRenderFactoryInstance();
			RenderDeviceCaps const & caps = rf.getRenderEngineInstance().getDeviceCaps();
			if (caps.mMultithreadResCreatingSupport)
			{
				this->mainThreadStage();
			}
		}
		std::shared_ptr<void> mainThreadStage()
		{
			std::lock_guard<std::mutex> lock(mMainThreadStateMutex);
			RenderModelPtr const & model = *mModelDesc.mModel;
			if (!model || !model->isHWResourceReady())
			{
				this->fillModel();
				for (size_t i = 0; i < mModelDesc.mModelData->mMergedBuffer.size(); ++i)
				{
					mModelDesc.mModelData->mMergedVBs[i]->createHWResouce(&mModelDesc.mModelData->mMergedBuffer[i][0]);
				}
				mModelDesc.mModelData->mMergedIB->createHWResouce(&mModelDesc.mModelData->mMergedIndex[0]);
				model->buildModelInfo();
				for (uint32_t i = 0; i < model->getNumSubRenderables(); ++i)
				{
					checked_pointer_cast<StaticMesh>(model->getSubRenderable(i))->buildMeshInfo();
				}
				mModelDesc.mModelData.reset();
			}
			return std::static_pointer_cast<void>(model);

		}

		bool hasSubThreadStage() const
		{
			return true;
		}

		bool match(ResLoadingDesc const & rhs) const
		{
			if (this->getType() == rhs.getType())
			{
				RenderModelLoadingDesc const & rmld = static_cast<RenderModelLoadingDesc const &>(rhs);
				return (mModelDesc.mResName == rmld.mModelDesc.mResName) && (mModelDesc.mAccessHint == rmld.mModelDesc.mAccessHint);
			}
			return false;
		}
		void copyDataFrom(ResLoadingDesc const & rhs)
		{
			BOOST_ASSERT(this->getType() == rhs.getType());
			RenderModelLoadingDesc const & rmld = static_cast<RenderModelLoadingDesc const &>(rhs);
			mModelDesc.mResName = rmld.mModelDesc.mResName;
			mModelDesc.mAccessHint = rmld.mModelDesc.mAccessHint;
			mModelDesc.mModelData = rmld.mModelDesc.mModelData;
			mModelDesc.mModel = rmld.mModelDesc.mModel;
		}
		std::shared_ptr<void> cloneResourceFrom(std::shared_ptr<void> const& resource)
		{
			RenderModelPtr rhs_model = std::static_pointer_cast<RenderModel>(resource);
			RenderModelPtr model = mModelDesc.mCreateModelFactoryFunc(rhs_model->getName());
			model->setNumMaterials(rhs_model->getNumMaterials());
			for (uint32_t mtl_index = 0; mtl_index < model->getNumMaterials(); ++mtl_index)
			{
				RenderMaterialPtr mtl = MakeSharedPtr<RenderMaterial>();
				*mtl = *rhs_model->getMaterial(mtl_index);
				model->getMaterial(mtl_index) = mtl;
			}
			if (rhs_model->getNumSubRenderables() > 0)
			{
				RenderLayout const & rhs_rl = rhs_model->getSubRenderable(0)->getRenderLayout();
				std::vector<StaticMeshPtr> meshes(rhs_model->getNumSubRenderables());
				for (uint32_t mesh_index = 0; mesh_index < rhs_model->getNumSubRenderables(); ++mesh_index)
				{
					StaticMeshPtr rhs_mesh = checked_pointer_cast<StaticMesh>(rhs_model->getSubRenderable(mesh_index));
					meshes[mesh_index] = mModelDesc.mCreateMeshFactoryFunc(model, rhs_model->getName());
					StaticMeshPtr& mesh = meshes[mesh_index];
					mesh->setMaterialID(rhs_mesh->getMaterialID());
					mesh->setPosAABB(rhs_mesh->getPosAABB());
					mesh->setTexcoordAABB(rhs_mesh->getTexcoordAABB());
					for (uint32_t ve_index = 0; ve_index < rhs_rl.getNumVertexStreams(); ++ve_index)
					{
						mesh->addVertexStream(rhs_rl.getVertexStream(ve_index), rhs_rl.getVertexStreamFormat(ve_index)[0]);
					}
					mesh->addIndexStream(rhs_rl.getIndexStream(), rhs_rl.getIndexStreamFormat());
					mesh->setNumVertices(rhs_mesh->getNumVertices());
					mesh->setNumIndices(rhs_mesh->getNumIndices());
					mesh->setStartVertexLocation(rhs_mesh->getStartVertexLocation());
					mesh->setStartIndexLocation(rhs_mesh->getStartIndexLocation());
				}
				BOOST_ASSERT(model->isSkinned() == rhs_model->isSkinned());

				if (rhs_model->isSkinned())
				{
				}
				model->assignSubrenderable(meshes.begin(), meshes.end());
			}
			model->buildModelInfo();
			for (uint32_t i = 0; i < model->getNumSubRenderables(); ++i)
			{
				checked_pointer_cast<StaticMesh>(model->getSubRenderable(i))->buildMeshInfo();
			}
			return std::static_pointer_cast<void>(model);
		}

		std::shared_ptr<void> getResource() const
		{
			return *mModelDesc.mModel;
		}
	private:
		void fillModel()
		{
			RenderModelPtr const & model = *mModelDesc.mModel;
			RenderFactory& rf = Engine::getInstance().getRenderFactoryInstance();
			mModelDesc.mModelData->mMergedVBs.resize(mModelDesc.mModelData->mMergedBuffer.size());
			for (size_t i = 0; i < mModelDesc.mModelData->mMergedVBs.size(); ++i)
			{
				mModelDesc.mModelData->mMergedVBs[i] = rf.makeDelayCreationVertexBuffer(BU_Static, mModelDesc.mAccessHint, static_cast<uint32_t>(mModelDesc.mModelData->mMergedBuffer[i].size()));
			}
			mModelDesc.mModelData->mMergedIB = rf.makeDelayCreationIndexBuffer(BU_Static, mModelDesc.mAccessHint, static_cast<uint32_t>(mModelDesc.mModelData->mMergedIndex.size()));

			std::vector<StaticMeshPtr> meshes(mModelDesc.mModelData->mMeshNames.size());
			for (uint32_t mesh_index = 0; mesh_index < mModelDesc.mModelData->mMeshNames.size(); ++mesh_index)
			{
				meshes[mesh_index] = mModelDesc.mCreateMeshFactoryFunc(model, mModelDesc.mModelData->mMeshNames[mesh_index]);
				StaticMeshPtr& mesh = meshes[mesh_index];

				mesh->setMaterialID(mModelDesc.mModelData->mMaterialIds[mesh_index]);
				mesh->setPosAABB(mModelDesc.mModelData->mPosBox[mesh_index]);
				mesh->setTexcoordAABB(mModelDesc.mModelData->mTexcoordBox[mesh_index]);
				for (uint32_t ve_index = 0; ve_index < mModelDesc.mModelData->mMergedBuffer.size(); ++ve_index)
				{
					mesh->addVertexStream(mModelDesc.mModelData->mMergedVBs[ve_index], mModelDesc.mModelData->mMergedVes[ve_index]);
				}
				mesh->addIndexStream(mModelDesc.mModelData->mMergedIB, mModelDesc.mModelData->mAllIsIndex16Bit ? EF_R16UI : EF_R32UI);
				mesh->setNumVertices(mModelDesc.mModelData->mMeshNumVertices[mesh_index]);
				mesh->setNumIndices(mModelDesc.mModelData->mMeshNumIndices[mesh_index]);
				mesh->setStartVertexLocation(mModelDesc.mModelData->mMeshBaseVertices[mesh_index]);
				mesh->setStartIndexLocation(mModelDesc.mModelData->mMeshStartIndices[mesh_index]);
			}
			model->assignSubrenderable(meshes.begin(), meshes.end());
		}
	private:
		ModelDesc mModelDesc;
		std::mutex mMainThreadStateMutex;
	};
}
namespace Air
{

	RenderModel::RenderModel(std::wstring const & name) : mName(name), mHWResourceReady(false)
	{

	}

	void RenderModel::updateAABB()
	{
		mPosAABB = AABBox(float3(0, 0, 0), float3(0, 0, 0));
		mTexcoordAABB = AABBox(float3(0, 0, 0), float3(0, 0, 0));
		for (auto const & mesh : mSubRenderables)
		{
			mPosAABB |= mesh->getPosAABB();
			mTexcoordAABB |= mesh->getTexcoordAABB();
		}
	}

	StaticMesh::StaticMesh(RenderModelPtr const & model, std::wstring const & name)
		:mName(name), mModel(model), mHWResReady(false)
	{
		mRenderLayout = Engine::getInstance().getRenderFactoryInstance().MakeRenderLayout();
		mRenderLayout->setTopologyType(RenderLayout::TT_TriangleList);
	}

	StaticMesh::~StaticMesh()
	{
	}
	RenderLayout& StaticMesh::getRenderLayout() const
	{
		return *mRenderLayout;
	}

	void StaticMesh::addVertexStream(void const * buf, uint32_t size, VertexElement const & ve, uint32_t access_hint)
	{
		RenderFactory& rf = Engine::getInstance().getRenderFactoryInstance();
		GraphicsBufferPtr vb = rf.makeVertexBuffer(BU_Static, access_hint, size, buf);
		this->addVertexStream(vb, ve);
	}

	void StaticMesh::addVertexStream(GraphicsBufferPtr const & buffer, VertexElement const & ve)
	{
		mRenderLayout->bindVertexStream(buffer, std::make_tuple(ve));
	}
	void StaticMesh::addIndexStream(void const * buf, uint32_t size, ElementFormat format, uint32_t access_hint)
	{
		RenderFactory& rf = Engine::getInstance().getRenderFactoryInstance();
		GraphicsBufferPtr ib = rf.makeIndexBuffer(BU_Static, access_hint, size, buf);
		this->addIndexStream(ib, format);
	}

	void StaticMesh::addIndexStream(GraphicsBufferPtr const & index_stream, ElementFormat format)
	{
		mRenderLayout->bindIndexStream(index_stream, format);
	}

	



	RenderModelPtr syncLoadModel(std::wstring const & meshml_name, uint32_t access_hint, std::function<RenderModelPtr(std::wstring const &)> createModelFactoryFunc, std::function<StaticMeshPtr(RenderModelPtr const &, std::wstring const &)> createMeshFactoryFunc)
	{
		BOOST_ASSERT(createModelFactoryFunc);
		BOOST_ASSERT(createMeshFactoryFunc);
		return ResLoader::getInstance().syncQueryT<RenderModel>(MakeSharedPtr<RenderModelLoadingDesc>(meshml_name, access_hint, createModelFactoryFunc, createMeshFactoryFunc));
	}

	std::string const jit_ext_name = ".model_bin";





	void loadModel(std::string const & path,
		std::vector<VertexElement> & merged_ves, char& all_is_index_16_bit,
		std::vector<std::vector<uint8_t>>& merged_buff, std::vector<uint8_t>& merged_indices,
		std::vector<std::wstring>& mesh_names, std::vector<int32_t>& mtl_ids,
		std::vector<AABBox> & pos_bbs, std::vector<AABBox>& tc_bbs,
		std::vector<uint32_t> & mesh_num_vertices, std::vector<uint32_t>& mesh_base_vertices,
		std::vector<uint32_t> & mesh_num_indices, std::vector<uint32_t>& mesh_base_indices)
	{
		ResIdentifierPtr lzma_file;
		std::string absPath = ResLoader::getInstance().getAbsPath(path + ".asset");
		std::replace(absPath.begin(), absPath.end(), '\\', '/');

		lzma_file = ResLoader::getInstance().open(absPath);
		uint32_t fourcc;
		lzma_file->read(&fourcc, size_t(fourcc));
		fourcc = LE2Native(fourcc);
		BOOST_ASSERT((fourcc == MakeFourCC<'M', 'E', 'S', 'H'>::value));


		uint32_t ver;
		lzma_file->read(&ver, sizeof(ver));
		ver = LE2Native(ver);
		BOOST_ASSERT(MODEL_BIN_VERSION == ver);

		std::shared_ptr<std::stringstream> ss = MakeSharedPtr<std::stringstream>();

		uint64_t original_len, len;
		lzma_file->read(&original_len, sizeof(original_len));
		original_len = LE2Native(original_len);
		lzma_file->read(&len, sizeof(len));
		len = LE2Native(len);

		//解码
		//LZMACodec
		LZMACodec lzma;
		lzma.decode(*ss, lzma_file, len, original_len);
		ResIdentifierPtr decoded = MakeSharedPtr<ResIdentifier>(lzma_file->getResName(), lzma_file->getTimestamp(), ss);

		//模型数量
		uint32_t num_meshes;
		decoded->read(&num_meshes, sizeof(num_meshes));
		num_meshes = LE2Native(num_meshes);


		//读一个短字符串 readShortString();
		uint32_t num_merged_ves;
		decoded->read(&num_merged_ves, sizeof(num_merged_ves));
		merged_ves.resize(num_merged_ves);
		for (size_t i = 0; i < num_merged_ves; ++i)
		{
			decoded->read(&merged_ves[i], sizeof(merged_ves[i]));
			merged_ves[i].mUsage = LE2Native(merged_ves[i].mUsage);
			merged_ves[i].mFormat = LE2Native(merged_ves[i].mFormat);
		}

		uint32_t all_num_vertices;
		uint32_t all_num_indices;

		decoded->read(&all_num_vertices, sizeof(all_num_vertices));
		all_num_vertices = LE2Native(all_num_vertices);

		decoded->read(&all_num_indices, sizeof(all_num_indices));
		all_num_indices = LE2Native(all_num_indices);

		decoded->read(&all_is_index_16_bit, sizeof(all_is_index_16_bit));

		int const index_elem_size = all_is_index_16_bit ? 2 : 4;

		merged_buff.resize(merged_ves.size());
		for (size_t i = 0; i < merged_buff.size(); ++i)
		{
			merged_buff[i].resize(all_num_vertices * merged_ves[i].getElementSize());
			decoded->read(&merged_buff[i][0], merged_buff[i].size() * sizeof(merged_buff[i][0]));
		}

		merged_indices.resize(all_num_indices * index_elem_size);
		decoded->read(&merged_indices[0], merged_indices.size() * sizeof(merged_indices[0]));

		mesh_names.resize(num_meshes);
		mtl_ids.resize(num_meshes);
		pos_bbs.resize(num_meshes);
		tc_bbs.resize(num_meshes);
		mesh_num_vertices.resize(num_meshes);
		mesh_base_vertices.resize(num_meshes);
		mesh_num_indices.resize(num_meshes);
		mesh_base_indices.resize(num_meshes);
		for (uint32_t mesh_index = 0; mesh_index < num_meshes; ++mesh_index)
		{
			mesh_names[mesh_index] = readShortWString(decoded);
			decoded->read(&mtl_ids[mesh_index], sizeof(mtl_ids[mesh_index]));

			float3 min_bb, max_bb;
			decoded->read(&min_bb, sizeof(min_bb));
			min_bb.x() = LE2Native(min_bb.x());
			min_bb.y() = LE2Native(min_bb.y());
			min_bb.z() = LE2Native(min_bb.z());

			decoded->read(&max_bb, sizeof(max_bb));
			max_bb.x() = LE2Native(max_bb.x());
			max_bb.y() = LE2Native(max_bb.y());
			max_bb.z() = LE2Native(max_bb.z());

			pos_bbs[mesh_index] = AABBox(min_bb, max_bb);

			decoded->read(&min_bb[0], sizeof(min_bb[0]));
			decoded->read(&min_bb[1], sizeof(min_bb[1]));

			min_bb.x() = LE2Native(min_bb.x());
			min_bb.y() = LE2Native(min_bb.y());
			min_bb.z() = 0;

			decoded->read(&max_bb[0], sizeof(max_bb[0]));
			decoded->read(&max_bb[1], sizeof(max_bb[1]));
			max_bb.x() = LE2Native(max_bb.x());
			max_bb.y() = LE2Native(max_bb.y());
			max_bb.z() = 0;
			tc_bbs[mesh_index] = AABBox(min_bb, max_bb);

			decoded->read(&mesh_num_vertices[mesh_index], sizeof(mesh_num_vertices[mesh_index]));
			mesh_num_vertices[mesh_index] = LE2Native(mesh_num_vertices[mesh_index]);
			decoded->read(&mesh_base_vertices[mesh_index], sizeof(mesh_base_vertices[mesh_index]));
			mesh_base_vertices[mesh_index] = LE2Native(mesh_base_vertices[mesh_index]);


			decoded->read(&mesh_num_indices[mesh_index], sizeof(mesh_num_indices[mesh_index]));
			mesh_num_indices[mesh_index] = LE2Native(mesh_num_indices[mesh_index]);
			decoded->read(&mesh_base_indices[mesh_index], sizeof(mesh_base_indices[mesh_index]));
			mesh_base_indices[mesh_index] = LE2Native(mesh_base_indices[mesh_index]);
		}
	}
}