#ifndef _Mesh_H_
#define _Mesh_H_
#pragma once
#include "PreDeclare.h"
#include "rendersystem/include/Renderable.hpp"
#include "rendersystem\include\RenderLayout.hpp"
namespace Air
{
	class AIR_CORE_API Mesh
	{

	};

	class AIR_CORE_API StaticMesh : public Renderable
	{
	public:
		StaticMesh(RenderModelPtr const & model, std::wstring const & name);
		virtual ~StaticMesh();

		void buildMeshInfo()
		{
			mHWResReady = true;
		}

		virtual void setTechnique(RenderEffectPtr const & effect, RenderTechnique* tech)
		{
			mEffect = effect;
			mTechnique = tech;
		}

		void setMaterialID(int32_t mid)
		{
			mMaterialId = mid;
		}
		int32_t getMaterialID() const
		{
			return mMaterialId;
		}

		void setPosAABB(AABBox const & aabb)
		{
			mAABB = aabb;
		}
		AABBox const & getPosAABB() const
		{
			return mAABB;
		}

		void setTexcoordAABB(AABBox const & aabb)
		{
			mTCAABB = aabb;
		}

		AABBox const & getTexcoordAABB() const
		{
			return mTCAABB;
		}

		virtual bool isSkinned() const
		{
			return false;
		}


		virtual RenderLayout& getRenderLayout() const;

		void addVertexStream(void const * buf, uint32_t size, VertexElement const & ve, uint32_t access_hint);
		void addVertexStream(GraphicsBufferPtr const & buffer, VertexElement const & ve);

		void addVertexStream(GraphicsBufferPtr const & buffer, ArrayRef<VertexElement> ve);

		virtual std::wstring const & getName() const
		{
			return mName;
		}


		void addIndexStream(void const * buf, uint32_t size, ElementFormat format, uint32_t access_hint);
		void addIndexStream(GraphicsBufferPtr const & index_stream, ElementFormat format);

		void setNumVertices(uint32_t n);
		uint32_t getNumVertices() const
		{
			return mRenderLayout->getNumVertices();
		}

		void setNumIndices(uint32_t n)
		{
			return mRenderLayout->setNumIndices(n);
		}

		uint32_t getNumIndices() const
		{
			return mRenderLayout->getNumIndices();
		}

		void setStartVertexLocation(uint32_t location)
		{
			mRenderLayout->setStartVertexLocation(location);
		}
		uint32_t getStartVertexLocation() const
		{
			return mRenderLayout->getStartVertexLocation();
		}

		void setStartIndexLocation(uint32_t location)
		{
			mRenderLayout->setStartIndexLocation(location);
		}
		uint32_t getStartIndexLocation() const
		{
			return mRenderLayout->getStartIndexLocation();
		}

		void setTopologyType(RenderLayout::TopologyType type);
	protected:
		std::wstring mName;
		RenderLayoutPtr mRenderLayout;
		bool mHWResReady;
		AABBox mAABB;
		AABBox mTCAABB;
		int32_t mMaterialId;
		std::weak_ptr<RenderModel> mModel;
	};

	class AIR_CORE_API RenderModel : public Renderable
	{
	public:
		explicit RenderModel(std::wstring const & name);
		virtual ~RenderModel()
		{

		}


		size_t getNumMaterials() const 
		{
			return mMaterials.size();
		}

		void setNumMaterials(size_t i)
		{
			mMaterials.resize(i);
		}

		RenderMaterialPtr & getMaterial(size_t index)
		{
			return mMaterials[index];
		}

		RenderMaterialPtr const & getMaterial(size_t index) const
		{
			return mMaterials[index];
		}

		virtual bool isSkinned() const override
		{
			return false;
		}

		void buildModelInfo()
		{
			this->doBuildModelInfo();
		}
	protected:
		virtual void updateAABB() override;
		virtual void doBuildModelInfo()
		{

		}

	protected:
		std::wstring mName;
		RenderLayoutPtr mRenderLayout;
		AABBox mPosAABB;
		AABBox mTexcoordAABB;
		std::vector<RenderMaterialPtr> mMaterials;
		bool mHWResourceReady;
	};

	struct AIR_CORE_API Joint
	{
		std::wstring mName;
		Quaternion mBindReal;
		Quaternion mBindDual;
		float mBindScale;

		Quaternion mInverseOriginReal;
		Quaternion mInverseOriginDual;

		float mInverseBindScale;

		int16_t parent;
	};

	struct AIR_CORE_API AnimationAction
	{
		std::wstring mName;
		uint32_t mStartFrame;
		uint32_t mEndFrame;
	};
	typedef std::vector<AnimationAction> AnimationActionsType;

	template<typename T>
	struct CreateMeshFactory
	{
		StaticMeshPtr operator()(RenderModelPtr const & model, std::wstring const & name)
		{
			return MakeSharedPtr<T>(model, name);
		}
	};

	template<typename T>
	struct CreateModelFactory
	{
		RenderModelPtr operator()(std::wstring const &name)
		{
			return MakeSharedPtr<T>(name);
		}
	};

	struct AIR_CORE_API KeyFrames 
	{
		std::vector<uint32_t> mFrameId;
		std::vector<Quaternion> mBindReal;
		std::vector<Quaternion> mBindDual;
		std::vector<float> mBindScale;

		std::pair<std::pair<Quaternion, Quaternion>, float> getFrame(float frame) const;
	};
	typedef std::vector<KeyFrames> KeyFramesType;

	struct AIR_CORE_API AABBKeyFrames
	{
		std::vector<uint32_t> mFrameId;
		std::vector<AABBox> mBB;
		AABBox getFrame(float frame) const;
	};
	typedef std::vector<AABBKeyFrames> AABBKeyFramesType;

	AIR_CORE_API 	void loadModel(std::string const & path,
		std::vector<VertexElement> & merged_ves, char& all_is_index_16_bit,
		std::vector<std::vector<uint8_t>>& merged_buff, std::vector<uint8_t>& merged_indices,
		std::vector<std::wstring>& mesh_names, std::vector<int32_t>& mtl_ids,
		std::vector<AABBox> & pos_bbs, std::vector<AABBox>& tc_bbs,
		std::vector<uint32_t> & mesh_num_vertices, std::vector<uint32_t>& mesh_base_vertices,
		std::vector<uint32_t> & mesh_num_indices, std::vector<uint32_t>& mesh_base_indices);



	AIR_CORE_API RenderModelPtr syncLoadModel(std::wstring const & meshml_name, uint32_t access_hint, std::function<RenderModelPtr(std::wstring const &)> createModelFactoryFunc = CreateModelFactory<RenderModel>(), std::function<StaticMeshPtr(RenderModelPtr const &, std::wstring const &)> createMeshFactoryFunc = CreateMeshFactory<StaticMesh>());




}
#endif