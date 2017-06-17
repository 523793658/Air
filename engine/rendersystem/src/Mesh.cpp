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
				std::vector<RenderMaterialPtr> mMaterials;
				std::vector<VertexElement> mMergedVes;
				char mAllIsIndex16Bit;
				std::vector<std::vector<uint8_t>> mMergedBuffer;
				std::vector<uint8_t> mMergedIndex;
				std::vector<GraphicsBufferPtr> mMergedVBs;
				GraphicsBufferPtr mMergedIB;
				std::vector<std::string> mMeshNames;
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
		RenderModelLoadingDesc(std::string const &resName, uint32_t accessHint, std::function<RenderModelPtr(std::wstring const &)> createModelFactoryFunc, std::function<StaticMeshPtr(RenderModelPtr const &, std::wstring const &)> createMeshFactoryFunc)
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
				this->addsSubPath();
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
			this->addsSubPath();
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
		}
		void addsSubPath()
		{
		}
	private:
		ModelDesc mModelDesc;
		std::mutex mMainThreadStateMutex;
	};
}
namespace Air
{
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
		GraphicsBufferPtr vb = rf.makeVertex
	}

	RenderModelPtr syncLoadModel(std::string const & meshml_name, uint32_t access_hint, std::function<RenderModelPtr(std::wstring const &)> createModelFactoryFunc, std::function<StaticMeshPtr(RenderModelPtr const &, std::wstring const &)> createMeshFactoryFunc)
	{
		BOOST_ASSERT(createModelFactoryFunc);
		BOOST_ASSERT(createMeshFactoryFunc);
		return ResLoader::getInstance().syncQueryT<RenderModel>(MakeSharedPtr<RenderModelLoadingDesc>(meshml_name, access_hint, createModelFactoryFunc, createMeshFactoryFunc));
	}

	std::string const jit_ext_name = ".model_bin";

	void loadModel(std::string const & path, std::vector<RenderMaterialPtr>& mtls,
		std::vector<VertexElement> & merged_ves, char& all_is_index_16_bit,
		std::vector<std::vector<uint8_t>>& merged_buff, std::vector<uint8_t>& merged_indices,
		std::vector<std::string>& mesh_names, std::vector<int32_t>& mtl_ids,
		std::vector<AABBox> & pos_bbs, std::vector<AABBox>& tc_bbs,
		std::vector<uint32_t> & mesh_num_vertices, std::vector<uint32_t>& mesh_base_vertices,
		std::vector<uint32_t> & mesh_num_indices, std::vector<uint32_t>& mesh_base_indices,

		std::vector<Joint> & joints, std::shared_ptr<AnimationActionsType> & actions,
		std::shared_ptr<KeyFramesType> & kfs, uint32_t & num_frame, uint32_t & frame_rate,
		std::vector<std::shared_ptr<AABBKeyFrames>>& frame_pos_bbs)
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


	}
}