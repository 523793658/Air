#ifndef _Renderable_H_
#define _Renderable_H_
#pragma once

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
		PT_OpaqueDepth = MakePassType<PRT_None, PTB_Opaque, PC_Depth>::value,
		PT_TransparencyBackDepth = MakePassType<PRT_None, PTB_TransparencyBack, PC_Depth>::value,
		PT_TransparencyFrontDepth = MakePassType<PRT_None, PTB_TransparencyFront, PC_Depth>::value,

		PT_OpaqueGBufferRT0 = MakePassType<PRT_RT0, PTB_Opaque, PC_GBuffer>::value,
		PT_TransparencyBackGBufferRT0 = MakePassType<PRT_RT0, PTB_TransparencyBack, PC_GBuffer>::value,
		PT_TransparencyFrontGBufferRT0 = MakePassType<PRT_RT0, PTB_TransparencyFront, PC_GBuffer>::value,
		PT_OpaqueGBufferRT1 = MakePassType<PRT_RT1, PTB_Opaque, PC_GBuffer>::value,
		PT_TransparencyBackGBufferRT1 = MakePassType<PRT_RT1, PTB_TransparencyBack, PC_GBuffer>::value,
		PT_TransparencyFrontGBufferRT1 = MakePassType<PRT_RT1, PTB_TransparencyFront, PC_GBuffer>::value,
		PT_OpaqueGBufferMRT = MakePassType<PRT_MRT, PTB_Opaque, PC_GBuffer>::value,
		PT_TransparencyBackGBufferMRT = MakePassType<PRT_MRT, PTB_TransparencyBack, PC_GBuffer>::value,
		PT_TransparencyFrontGBufferMRT = MakePassType<PRT_MRT, PTB_TransparencyFront, PC_GBuffer>::value,

		PT_GenShadowMap = MakePassType<PRT_ShadowMap, PTB_None, PC_ShadowMap>::value,
		PT_GenShadowMapWODepthTexture = MakePassType<PRT_ShadowMapWODepth, PTB_None, PC_ShadowMap>::value,
		PT_GenCascadedShadowMap = MakePassType<PRT_CascadedShadowMap, PTB_None, PC_ShadowMap>::value,
		PT_GenReflectiveShadowMap = MakePassType<PRT_ReflectiveShadowMap, PTB_None, PC_ShadowMap>::value,

		PT_Shadowing = MakePassType<PRT_None, PTB_None, PC_Shadowing>::value,

		PT_IndirectLighting = MakePassType<PRT_None, PTB_None, PC_IndirectLighting>::value,

		PT_OpaqueShading = MakePassType<PRT_None, PTB_Opaque, PC_Shading>::value,
		PT_TransparencyBackShading = MakePassType<PRT_None, PTB_TransparencyBack, PC_Shading>::value,
		PT_TransparencyFrontShading = MakePassType<PRT_None, PTB_TransparencyFront, PC_Shading>::value,

		PT_OpaqueReflection = MakePassType<PRT_None, PTB_Opaque, PC_Reflection>::value,
		PT_TransparencyBackReflection = MakePassType<PRT_None, PTB_TransparencyBack, PC_Reflection>::value,
		PT_TransparencyFrontReflection = MakePassType<PRT_None, PTB_TransparencyFront, PC_Reflection>::value,

		PT_OpaqueSpecialShading = MakePassType<PRT_None, PTB_Opaque, PC_SpecialShading>::value,
		PT_TransparencyBackSpecialShading = MakePassType<PRT_None, PTB_TransparencyBack, PC_SpecialShading>::value,
		PT_TransparencyFrontSpecialShading = MakePassType<PRT_None, PTB_TransparencyFront, PC_SpecialShading>::value,

		PT_SimpleForward = MakePassType<PRT_None, PTB_None, PC_SimpleForward>::value
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

		virtual AABBox const & getAABB() const = 0;
		virtual AABBox const & getTexcoordAABB() const = 0;

		void addInstance(SceneObject const * obj);

		virtual void addToRenderQueue();

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

		uint32_t getNumInstances() const
		{
			return static_cast<uint32_t>(mInstances.size());
		}

		SceneObject const * getInstance(uint32_t index) const
		{
			return mInstances[index];
		}

		virtual void setLocalMatrix(float4x4 const & mat);

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


		bool isAllHWResourceReady() const;

		virtual void setObjectID(uint32_t id);




		virtual bool getHWResourceReady() const;


	protected:
		virtual void updateAABB();

		virtual void updateInstanceStream();

	protected:
		std::vector<SceneObject const *> mInstances;

		RenderEffectPtr mEffect;
		RenderTechnique* mTechnique;

		std::vector<RenderablePtr> mSubRenderables;
	};
}



#endif