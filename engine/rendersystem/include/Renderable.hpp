#ifndef _Renderable_H_
#define _Renderable_H_
#pragma once
#include "core/include/PreDeclare.h"
namespace Air
{


	enum PassCategory
	{
		PC_Depth = 0,
		PC_GBuffer,
		PC_ShadowMap,
		PC_Shadowing,
		PC_IndirectLighting,
		PC_Shading,
		PC_Reflection,
		PC_SpecialShading,
		PC_SimpleForward,
		PC_ForwardRendering,
		PC_None
	};

	enum PassTargetBuffer
	{
		PTB_Opaque = 0,
		PTB_TransparencyBack,
		PTB_TransparencyFront,
		PTB_None
	};

	enum PassRT
	{
		PRT_RT0 = 0,
		PRT_RT1,
		PRT_MRT,
		PRT_ShadowMap,
		PRT_ShadowMapWODepth,
		PRT_CascadedShadowMap,
		PRT_ReflectiveShadowMap,
		PRT_None
	};

	// pass type is a 32-bit value:
	// 0000000000000000000000 C3 C2 C1 C0 TB1 TB0 RT3 RT2 RT1 RT0

	template <PassRT rt, PassTargetBuffer tb, PassCategory cat>
	struct MakePassType
	{
		static uint32_t const value = (rt << 0) | (tb << 4) | (cat << 6);
	};

	enum PassType
	{
		PT_GenShadowMap = MakePassType<PRT_ShadowMap, PTB_None, PC_ShadowMap>::value,
		PT_ForwardRendering = MakePassType<PRT_RT0, PTB_Opaque, PC_ForwardRendering>::value,

		PT_Custom = MakePassType<PRT_None, PTB_None, PC_None>::value,
	};

	inline PassRT GetPassRT(PassType pt)
	{
		return static_cast<PassRT>(pt & 0xF);
	}
	inline PassTargetBuffer GetPassTargetBuffer(PassType pt)
	{
		return static_cast<PassTargetBuffer>((pt >> 4) & 0x3);
	}
	inline PassCategory GetPassCategory(PassType pt)
	{
		return static_cast<PassCategory>((pt >> 6) & 0xF);
	}
	inline PassType ComposePassType(PassRT rt, PassTargetBuffer tb, PassCategory cat)
	{
		return static_cast<PassType>((rt << 0) | (tb << 4) | (cat << 6));
	}

	class AIR_CORE_API Renderable
	{
	public:
		enum EffectAttribute
		{
			EA_SpecialShading = 1UL << 0,
			EA_TransparencyBack = 1UL << 1,
			EA_TransparencyFront = 1UL << 2,
			EA_AlphaTest = 1UL << 3,
			EA_Reflection = 1UL << 4,
			EA_SimpleForward = 1UL << 5,
			EA_SSS = 1UL << 6
		};

	public:
		Renderable();
		virtual ~Renderable();

		virtual RenderEffectPtr const & getRenderEffect() const
		{
			return mEffect;
		}


		virtual RenderTechnique* getRenderTechnique() const
		{
			return mTechnique;
		}

		virtual RenderLayout& getRenderLayout() const = 0;

		virtual std::wstring const & getName() const = 0;

		virtual void onRenderBegin();
		virtual void onRenderEnd();

		virtual void onInstanceBegin(uint32_t id);
		virtual void onInstanceEnd(uint32_t id);

		virtual AABBox const & getPosAABB() const = 0;
		virtual AABBox const & getTexcoordAABB() const = 0;

		virtual bool isSkinned() const = 0;

		void addInstance(SceneObject const * obj);

		virtual void render();
		

		template<typename Iterator>
		void assignInstances(Iterator begin, Iterator end)
		{
			mInstances.resize(0);
			for (Iterator iter = begin; iter != end; ++iter)
			{
				this->addInstance(*iter);
			}
		}

		void clearInstance()
		{
			mInstances.resize(0);
		}

		uint32_t getNumInstances() const
		{
			return static_cast<uint32_t>(mInstances.size());
		}

		SceneObject const * getInstance(uint32_t index) const
		{
			return mInstances[index];
		}

		virtual void setMatrix(float4x4 const & mat);

		template<typename ForwardIterator>
		void assignSubrenderable(ForwardIterator first, ForwardIterator last)
		{
			mSubRenderables.assign(first, last);
			this->updateAABB();
		}

		RenderablePtr const & getSubRenderable(size_t id) const
		{
			return mSubRenderables[id];
		}

		uint32_t getNumSubRenderables() const
		{
			return static_cast<uint32_t>(mSubRenderables.size());
		}

		virtual bool isHWResourceReady() const
		{
			return true;
		}
		bool selectMode() const
		{
			return false;
		}


		bool isAllHWResourceReady() const
		{
			return true;
		}

		virtual void setObjectID(uint32_t id);


		void setMaterial(RenderMaterialPtr material);

		virtual bool getHWResourceReady() const;

		virtual bool isTransparencyBackFace() const;

		virtual bool isTransparencyFrontFace() const;

		virtual bool isOpaque() const;

		void setPass(PassType type);


	protected:
		virtual void updateAABB();

		virtual void updateInstanceStream();

		virtual void updateTechniques();

		virtual void bindEffect(RenderEffectPtr const & effect);

		virtual RenderTechnique* getPassTechnique(PassType type) const;


	protected:
		std::vector<SceneObject const *> mInstances;

		RenderEffectPtr mEffect;
		RenderTechnique* mTechnique;

		RenderTechnique* mSelectModeTech;
		RenderEffectParameter* mSelectModeObjectIdParam;
		float4 mSelectModeObjectId;
		bool mSelectModeOn{ false };
		float4x4 mModelMat{ float4x4::identify() };
		std::vector<RenderablePtr> mSubRenderables;
		uint32_t mEffectAttrs{ 0 };

		RenderEffectPtr mDeferredEffect;
		RenderTechnique* mSimpleForwardTech;
		RenderMaterialPtr mMaterial;

		//²ÎÊý
		RenderEffectParameter* mMVPParam;
		RenderEffectParameter* mMVParam;
		RenderEffectParameter* mWorldParam;
		RenderEffectParameter* mBaseColorRoughness;
		RenderEffectParameter* mSpecularColorMetallic;

		PassType mPassType{ PT_Custom };
	protected:
		

	};
}



#endif