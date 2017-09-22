#ifndef _RenderEffect_H_
#define _RenderEffect_H_

#include <boost/noncopyable.hpp>
#include "rendersystem/include/ShaderObject.hpp"
#include "rendersystem/include/RenderDeviceCaps.hpp"

namespace Air
{
	class AIR_CORE_API RenderVariable : boost::noncopyable
	{
	public:
		RenderVariable();
		virtual ~RenderVariable();
		virtual std::unique_ptr<RenderVariable> clone() = 0;
		virtual RenderVariable& operator = (bool const & value);
		virtual RenderVariable& operator = (uint32_t const & value);
		virtual RenderVariable& operator = (int32_t const & value);
		virtual RenderVariable& operator = (float const & value);
		virtual RenderVariable& operator = (uint2 const & value);
		virtual RenderVariable& operator = (uint3 const & value);
		virtual RenderVariable& operator = (uint4 const & value);
		virtual RenderVariable& operator = (int2 const & value);
		virtual RenderVariable& operator = (int3 const & value);
		virtual RenderVariable& operator = (int4 const & value);
		virtual RenderVariable& operator = (float2 const & value);
		virtual RenderVariable& operator = (float3 const & value);
		virtual RenderVariable& operator = (float4 const & value);
		virtual RenderVariable& operator = (float4x4 const & value);
		virtual RenderVariable& operator = (TexturePtr const & value);
		virtual RenderVariable& operator = (GraphicsBufferPtr const & value);
		virtual RenderVariable& operator = (std::string const & value);
		virtual RenderVariable& operator = (std::vector<bool> const & value);
		virtual RenderVariable& operator = (std::vector<uint32_t> const & value);


		virtual RenderVariable& operator = (std::vector<int32_t> const & value);
		virtual RenderVariable& operator = (std::vector<float> const & value);
		virtual RenderVariable& operator = (std::vector<uint2> const & value);
		virtual RenderVariable& operator = (std::vector<uint3> const & value);
		virtual RenderVariable& operator = (std::vector<uint4> const & value);
		virtual RenderVariable& operator = (std::vector<int2> const & value);
		virtual RenderVariable& operator = (std::vector<int3> const & value);

		virtual RenderVariable& operator = (std::vector<int4> const & value);

		virtual RenderVariable& operator = (std::vector<float2> const & value);
		virtual RenderVariable& operator = (std::vector<float3> const & value);
		virtual RenderVariable& operator = (std::vector<float4> const & value);

		virtual RenderVariable& operator = (std::vector<float4x4> const & value);

		virtual void setValue(bool & value) const;
		virtual void setValue(uint32_t & value) const;
		virtual void setValue(int32_t & value) const;
		virtual void setValue(float & value) const;
		virtual void setValue(uint2 & value) const;
		virtual void setValue(uint3 & value) const;
		virtual void setValue(uint4 & value) const;
		virtual void setValue(int2 & value) const;
		virtual void setValue(int3 & value) const;
		virtual void setValue(int4 & value) const;
		virtual void setValue(float2 & value) const;
		virtual void setValue(float3 & value) const;
		virtual void setValue(float4 & value) const;
		virtual void setValue(float4x4 & value) const;
		virtual void setValue(TexturePtr & value) const;
		virtual void setValue(std::string & value) const;
		virtual void setValue(std::vector<bool>& value) const;
		virtual void setValue(std::vector<uint32_t>& value) const;
		virtual void setValue(std::vector<int32_t>& value) const;
		virtual void setValue(std::vector<float>& value) const;
		virtual void setValue(std::vector<uint2>& value) const;
		virtual void setValue(std::vector<uint3>& value) const;
		virtual void setValue(std::vector<uint4>& value) const;
		virtual void setValue(std::vector<int2>& value) const;
		virtual void setValue(std::vector<int3>& value) const;
		virtual void setValue(std::vector<int4>& value) const;
		virtual void setValue(std::vector<float2>& value) const;
		virtual void setValue(std::vector<float3>& value) const;
		virtual void setValue(std::vector<float4>& value) const;
		virtual void setValue(std::vector<float4x4>& value) const;

		virtual void bindToCBuffer(RenderEffectConstantBuffer& cbuff, uint32_t offset, uint32_t stride);
		virtual void rebindToCBuffer(RenderEffectConstantBuffer& cbuff);
		virtual bool inCBuffer() const;
		virtual uint32_t getCBufferOffset() const;
		virtual uint32_t getStride() const;


	};

	template<typename T>
	class RenderVariableConcrete : public RenderVariable
	{
	public:
		RenderVariableConcrete()
			: mInCBuffer(false)
		{
			new (mData.val) T;
		}
	protected:
		bool mInCBuffer;
		union VarData
		{
			struct CBufferDesc
			{
				RenderEffectConstantBuffer* mCBuffer;
				uint32_t offset;
				uint32_t stride;
			};
			CBufferDesc mCbufferDesc;
			uint8_t val[sizeof(T)];
		};
		VarData mData;
	};


	class AIR_CORE_API RenderTechnique : boost::noncopyable
	{
	public:
#if AIR_IS_DEV_PLATFORM
		void load(RenderEffect& effect, XMLNodePtr const & node, uint32_t tech_index);
#endif
		float getWeight() const;

		size_t getNameHash() const;
	private:
		std::string mName;
		size_t mNameHash;
		std::vector<RenderPassPtr> mPasses;
		std::shared_ptr<std::vector<RenderEffectAnnotationPtr>> mAnnotations;
		std::shared_ptr<std::vector<std::pair<std::string, std::string>>> mMacros;

		float mWeight{ 0 };
		bool mTransparent{ false };
		bool mIsValidate{ true };
		bool mHasTessellation{ false };
	};


	class AIR_CORE_API RenderEffectConstantBuffer : boost::noncopyable
	{
	public:
		RenderEffectConstantBuffer();
#if AIR_IS_DEV_PLATFORM
		void load(std::string const & name);
#endif
		void streamIn(ResIdentifierPtr const & res);

#if AIR_IS_DEV_PLATFORM
		void streamOut(std::ostream& os) const;
#endif

		std::unique_ptr<RenderEffectConstantBuffer> clone(RenderEffect& src_effect, RenderEffect& dst_effect);

		std::string const & getName() const;

		size_t getNameHash() const;

		void addParameters(uint32_t index) const;

		uint32_t getParametersIndex(uint32_t index) const;

		void resize(uint32_t size);

		template<typename T>
		T const * getVariableInBuff(uint32_t offset)
		{
			union Raw2T
			{
				uint8_t const * raw;
				T const * t;
			}r2t;
			r2t.raw = &mBuffer[offset];
			return r2t.t;
		}

		template <typename T>
		T* getVariableInBuff(uint32_t offset)
		{
			union Raw2T
			{
				uint8_t* raw;
				T* t;
			}r2t;
			r2t.raw = &mBuffer[offset];
			return r2t.t;
		}

		void setDirty(bool dirty);
		
		bool isDirty() const;

		void update();
		GraphicsBufferPtr const & getHWbuffer() const;

		void bindHWBuffer(GraphicsBufferPtr const & buff);



	private:
		std::shared_ptr<std::pair<std::string, size_t>> mName;
		std::shared_ptr<std::vector<uint32_t>> mParamIndices;
		GraphicsBufferPtr mHWBuffer;
		std::vector<uint8_t> mBuffer;
		bool mIsDirty;
	};



	class AIR_CORE_API RenderEffectAnnotation : boost::noncopyable
	{
	public:
#if AIR_IS_DEV_PLATFORM
		void load(XMLNodePtr const & node);
#endif
		void streamIn(ResIdentifierPtr const & res);
#if AIR_IS_DEV_PLATFORM
		void streamOut(std::ostream& os) const;
#endif
		uint32_t getType() const;
		std::string const & getName() const;
		template <typename T>
		void getValue(T& val) const
		{
			mVal->getValue(val);
		}
	private:
		uint32_t mType;
		std::string mName;
		std::unique_ptr<RenderVariable> mVal;
	};

	class AIR_CORE_API RenderEffectParameter : boost::noncopyable
	{
	public:
#if AIR_IS_DEV_PLATFORM
		void load(XMLNodePtr const & node);
#endif
		void streamIn(ResIdentifierPtr const & res);

#if AIR_IS_DEV_PLATFORM
		void streamOut(std::ostream& os) const;
#endif
		std::unique_ptr<RenderEffectParameter> clone();

		uint32_t getType() const;

		RenderVariable const & getVar() const;

		std::shared_ptr<std::string> const & getArraySize() const;

		std::string const & getName() const;

		size_t getNameHash() const;
		bool hasSematic() const;

		std::string const & getSemantic() const;

		size_t getSemanticHash() const;

		uint32_t getNumAnnotation() const;

		RenderEffectAnnotation const & getAnnotation(uint32_t n) const;

		template<typename T>
		RenderEffectParameter& operator = (T const & value)
		{
			*mVar = value;
			return *this;
		}

		template<typename T>
		void getValue(T& val) const
		{
			mVar->getValue(val);
		}

		void bindToCBuffer(RenderEffectConstantBuffer& cbuff, uint32_t offset, uint32_t stride);

		void rebindToCBuffer(RenderEffectConstantBuffer& cbuff);

		RenderEffectConstantBuffer& getCBuffer() const;

		bool inCBuffer() const;

		uint32_t getCBufferOffset() const;

		uint32_t getStride() const;

		template<typename T>
		T const * getMenoryInCBuffer() const
		{
			return mCBuffer->getVariableInBuff<T>(mVar->getCBufferOffset());
		}

		template<typename T>
		T* getMenoryInCBuffer()
		{
			return mCBuffer->getVariableInBuff<T>(mVar->getCBufferOffset());
		}

	private:
		std::shared_ptr<std::pair<std::string, size_t>> mName;
		std::shared_ptr<std::pair<std::string, size_t>> mSemantic;
		uint32_t mType;
		std::unique_ptr<RenderVariable> mVar;
		std::shared_ptr<std::string> mArraySize;
		std::shared_ptr<std::vector<std::unique_ptr<RenderEffectAnnotation>>> mAnnotations;
		RenderEffectConstantBuffer* mCBuffer;
	};

	class AIR_CORE_API RenderEffect : boost::noncopyable
	{
		friend class RenderEffectTemplate;
	public:
		void load(std::string const & name);
		RenderEffectPtr clone();
		std::string const & getResName() const;
		size_t getResNameHash() const;
		uint32_t getNumParameters() const;
		RenderEffectParameter* getParameterBySemantic(std::string_view semantic) const;
		RenderEffectParameter* getParameterByName(std::string_view name) const;
		RenderEffectParameter* getParameterByIndex(uint32_t n) const; 

		uint32_t getNumCBuffers() const;

		RenderEffectConstantBuffer* getCBufferByName(std::string_view name) const;
		RenderEffectConstantBuffer* getCBufferByIndex(uint32_t n) const;
		uint32_t getNumTechniques() const;
	
		RenderTechnique* getTechniqueByName(std::string_view name) const;
		RenderTechnique* getTechniqueByIndex(uint32_t n) const;
		RenderShaderFragment const & getShaderFragmentByIndex(uint32_t n) const;
		uint32_t addShaderDesc(ShaderDesc const & sd);
		ShaderDesc& getShaderDesc(uint32_t id);
		ShaderDesc const & getShaderDesc(uint32_t id) const;

		uint32_t getNumMacros() const;
		std::pair<std::string, std::string> const & getMacroByIndex(uint32_t n) const;

		std::string const & getTypeName(uint32_t code) const;


	private:
		RenderEffectTemplatePtr mEffectTemplate;
		std::vector<std::unique_ptr<RenderEffectParameter>> mParams;
		std::vector<std::unique_ptr<RenderEffectConstantBuffer>> mCbuffers;
		std::vector<ShaderObjectPtr> mShaderObjs;


	private:
		
	};

	class AIR_CORE_API RenderEffectTemplate : boost::noncopyable
	{
	public:
		void load(std::string const & name, RenderEffect& effect);
		bool streamIn(ResIdentifierPtr const & source, RenderEffect& effect);
#if AIR_IS_DEV_PLATFORM
		void streamOut(std::ostream& os, RenderEffect const & effect)
			const;
#endif

#if AIR_IS_DEV_PLATFORM
		void GenHLSLShaderText(RenderEffect const & effect);
		std::string const & getHLSLShaderText() const;
#endif
		size_t getNumTechniques() const
		{
			return mTechniques.size();
		}
		RenderTechnique* getTechniqueByName(std::string_view name) const;
		RenderTechnique* getTechniqueByIndex(uint32_t index) const;

	private:
#if AIR_IS_DEV_PLATFORM
		void recursiveIncludeNode(XMLNode const & root, std::vector<std::string>& include_names) const;
		void insertIncludeNodes(XMLDocument& target_doc, XMLNode& target_root, XMLNodePtr const & target_place, XMLNode const & include_root) const;
#endif



	private:
		std::string mResName;
		size_t mResNameHash;
#if AIR_IS_DEV_PLATFORM
		uint64_t mTimestamp;
#endif
		std::vector<std::unique_ptr<RenderTechnique>> mTechniques;
		std::shared_ptr<std::vector<std::pair<std::pair<std::string, std::string>, bool>>> mMacros;
		std::vector<RenderShaderFragment> mShaderFrags;
#if AIR_IS_DEV_PLATFORM
		std::string mHLSLShader;
#endif
		std::vector<ShaderDesc> mShaderDescs;
	};

	class AIR_CORE_API RenderPass : boost::noncopyable
	{
	public:
#if AIR_IS_DEV_PLATFORM
		void load(RenderEffect& effect, XMLNodePtr const & node, uint32_t tech_index, uint32_t pass_index, RenderPass const * inherit_pass);
		void load(RenderEffect& effect, uint32_t tech_index, uint32_t pass_index, RenderPass const * inhert_pass);
#endif

		bool streamIn(RenderEffect& effect, ResIdentifierPtr const & res, uint32_t tech_index, uint32_t pass_index);
#if AIR_IS_DEV_PLATFORM
		void streamOut(RenderEffect const & effect, std::ostream & os, uint32_t tech_index, uint32_t pass_index) const;
#endif // AIR_IS_DEV_PLATFORM
		std::string const & getName() const;

		size_t getNameHash() const;

		void bind(RenderEffect const & effect) const;

		void unbind(RenderEffect const & effect) const;
		bool isValidate() const;

		
	};

	class AIR_CORE_API RenderShaderFragment
	{
	public:
#if AIR_IS_DEV_PLATFORM
		void load(XMLNodePtr const & node);
#endif
		void streamIn(ResIdentifierPtr const & res);
#if AIR_IS_DEV_PLATFORM
		void streamOut(std::ostream& os) const;
#endif
		ShaderObject::ShaderType mType;
		ShaderModel mVer;
		std::string mStr;
	};

	AIR_CORE_API RenderEffectPtr syncLoadRenderEffect(std::string const & effect_name);
	AIR_CORE_API RenderEffectPtr asyncLoadRenderEffect(std::string const & effect_name);
}


#endif