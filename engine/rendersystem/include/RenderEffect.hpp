#ifndef _RenderEffect_H_
#define _RenderEffect_H_

#include <boost/noncopyable.hpp>
#include "rendersystem/include/ShaderObject.hpp"
#include "rendersystem/include/RenderDeviceCaps.hpp"



namespace Air
{
	struct AIR_CORE_API TextureSubresource 
	{
		TexturePtr mTex;
		uint32_t mFirstArrayIndex;
		uint32_t mNumItems;
		uint32_t mFirstLevel;
		uint32_t mNumLevels;
		TextureSubresource()
		{

		}
		TextureSubresource(TexturePtr const & t, uint32_t fai, uint32_t ni, uint32_t fl, uint32_t nl)
			: mTex(t), mFirstArrayIndex(fai), mNumItems(ni), mFirstLevel(fl), mNumLevels(nl)
		{

		}
	};



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
		virtual RenderVariable& operator = (TextureSubresource const & value);
		virtual RenderVariable& operator = (ShaderDesc const & value);
		virtual RenderVariable& operator = (SamplerStateObjectPtr const & value);
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

		virtual void getValue(bool & value) const;
		virtual void getValue(uint32_t & value) const;
		virtual void getValue(int32_t & value) const;
		virtual void getValue(float & value) const;
		virtual void getValue(uint2 & value) const;
		virtual void getValue(uint3 & value) const;
		virtual void getValue(uint4 & value) const;
		virtual void getValue(int2 & value) const;
		virtual void getValue(int3 & value) const;
		virtual void getValue(int4 & value) const;
		virtual void getValue(float2 & value) const;
		virtual void getValue(float3 & value) const;
		virtual void getValue(float4 & value) const;
		virtual void getValue(float4x4 & value) const;
		virtual void getValue(TexturePtr & value) const;
		virtual void getValue(TextureSubresource& val) const;
		virtual void getValue(SamplerStateObjectPtr& val) const;
		virtual void getValue(GraphicsBufferPtr& /*value*/) const;
		virtual void getValue(ShaderDesc& val) const;
		virtual void getValue(std::string & value) const;
		virtual void getValue(std::vector<bool>& value) const;
		virtual void getValue(std::vector<uint32_t>& value) const;
		virtual void getValue(std::vector<int32_t>& value) const;
		virtual void getValue(std::vector<float>& value) const;
		virtual void getValue(std::vector<uint2>& value) const;
		virtual void getValue(std::vector<uint3>& value) const;
		virtual void getValue(std::vector<uint4>& value) const;
		virtual void getValue(std::vector<int2>& value) const;
		virtual void getValue(std::vector<int3>& value) const;
		virtual void getValue(std::vector<int4>& value) const;
		virtual void getValue(std::vector<float2>& value) const;
		virtual void getValue(std::vector<float3>& value) const;
		virtual void getValue(std::vector<float4>& value) const;
		virtual void getValue(std::vector<float4x4>& value) const;

		virtual void bindToCBuffer(RenderEffectConstantBuffer& cbuff, uint32_t offset, uint32_t stride);
		virtual void rebindToCBuffer(RenderEffectConstantBuffer& cbuff);
		virtual bool isInCBuffer() const
		{
			return false;
		}
		virtual uint32_t getCBufferOffset() const
		{
			return 0;
		}
		virtual uint32_t getStride() const
		{
			return 0;
		}


	};

	enum ConstantBufferType
	{
		CBT_Object,
		CBT_Frame,
		CBT_Global
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
	T& retriveT()
	{
		union Raw2T
		{
			uint8_t* raw;
			T* t;
		}r2t;
		r2t.raw = mData.val;
		return *r2t.t;
	}

	T const & retriveT() const
	{
		union Raw2T
		{
			uint8_t const * raw;
			T const * t;
		}r2t;
		r2t.raw = mData.val;
		return *r2t.t;
	}

	std::unique_ptr<RenderVariable> clone() override
	{
		auto ret = MakeUniquePtr<RenderVariableConcrete<T>>();
		if (mInCBuffer)
		{
			if (!ret->mInCBuffer)
			{
				ret->retriveT().~T();
			}
			ret->mData = mData;
		}
		ret->mInCBuffer = mInCBuffer;
		T val;
		this->getValue(val);
		*ret = val;
		return std::move(ret);
	}

	virtual RenderVariable& operator = (T const & value) override
	{
		if (mInCBuffer)
		{
			T& val_in_cbuff = *(mData.mCbufferDesc.mCBuffer->template getVariableInBuff<T>(mData.mCbufferDesc.offset));
			if (val_in_cbuff != value)
			{
				val_in_cbuff = value;
				mData.mCbufferDesc.mCBuffer->setDirty(true);
			}
		}
		else
		{
			this->retriveT() = value;
		}
		return *this;
	}

	virtual void getValue(T& val) const override
	{
		if (mInCBuffer)
		{
			val = *(mData.mCbufferDesc.mCBuffer->template getVariableInBuff<T>(mData.mCbufferDesc.offset));
		}
		else
		{
			val = this->retriveT();
		}
	}

	virtual void bindToCBuffer(RenderEffectConstantBuffer& cbuff, uint32_t offset, uint32_t stride) override
	{
		if (!mInCBuffer)
		{
			T val;
			this->getValue(val);
			this->retriveT().~T();
			mInCBuffer = true;
			mData.mCbufferDesc.mCBuffer = &cbuff;
			mData.mCbufferDesc.offset = offset;
			mData.mCbufferDesc.stride = stride;
			this->operator =(val);
		}
	}

	virtual void rebindToCBuffer(RenderEffectConstantBuffer& cbuff) override
	{
		BOOST_ASSERT(mInCBuffer);
		mData.mCbufferDesc.mCBuffer = &cbuff;
	}
	virtual bool isInCBuffer() const override
	{
		return mInCBuffer;
	}
	virtual uint32_t getCBufferOffset() const override
	{
		return mData.mCbufferDesc.offset;
	}
	virtual uint32_t getStride() const override
	{
		return mData.mCbufferDesc.stride;
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

	template<typename T>
	class AIR_CORE_API RenderVariableArray : public RenderVariableConcrete<std::vector<T>>
	{
	public:
		std::unique_ptr<RenderVariable> clone() override
		{
			auto ret = MakeUniquePtr<RenderVariableArray<T>>();
			if (this->mInCBuffer)
			{
				if (!ret->mInCBuffer)
				{
					ret->retriveT().~vector();
				}
				ret->RenderVariableConcrete<std::vector<T>>::mData = this->mData;
			}
			ret->RenderVariableConcrete<std::vector<T>>::mInCBuffer = mInCBuffer;
			std::vector<T> val;
			this->getValue(val);
			*ret = val;
			return std::move(ret);
		}

		virtual RenderVariable& operator = (std::vector<T> const & value) override
		{
			if (this->mInCBuffer)
			{
				uint8_t* target = this->mData.mCbufferDesc.mCBuffer->template getVariableInBuff<uint8_t>(this->mData.mCbufferDesc.offset);
				mSize = static_cast<uint32_t>(value.size());
				for (size_t i = 0; i < value.size(); ++i)
				{
					memcpy(target + i * this->mData.mCbufferDesc.stride, &value[i], sizeof(value[i]));
				}
				this->mData.mCbufferDesc.mCBuffer->setDirty(true);
			}
			else
			{
				this->retriveT() = value;
			}
			return *this;
		}

		virtual void getValue(std::vector<T> & val) const override
		{
			if (this->mInCBuffer)
			{
				uint8_t const * src = this->mData.mCbufferDesc.mCBuffer->template getVariableInBuff<uint8_t>(this->mData.mCbufferDesc.offset);
				val.resize(mSize);
				for (size_t i = 0; i < mSize; ++i)
				{
					memcpy(&val[i], src + i * this->mData.mCbufferDesc.stride, sizeof(val[i]));
				}
			}
			else
			{
				val = this->retriveT();
			}
		}
	private:
		uint32_t mSize;
	};

	class RenderVariableFloat4x4 : public RenderVariableConcrete<float4x4>
	{
		std::unique_ptr<RenderVariable> clone() override;

		virtual RenderVariable& operator=(float4x4 const & value) override;
		virtual void getValue(float4x4& val) const override;

	private:
		uint32_t mSize;
	};

	class RenderVariableFloat4x4Array : public
		RenderVariableConcrete<std::vector<float4x4>>
	{
	public:
		std::unique_ptr<RenderVariable> clone() override;
		virtual RenderVariable& operator=(std::vector<float4x4> const & value) override;
		virtual void getValue(std::vector<float4x4>& val) const;
	private:
		uint32_t mSize;
	};

	class RenderVariableTexture : public RenderVariable
	{
	public:
		std::unique_ptr<RenderVariable> clone() override;

		virtual RenderVariable& operator= (TexturePtr const & value);
		virtual RenderVariable& operator= (TextureSubresource const & value);
		virtual RenderVariable& operator= (std::string const & value);

		virtual void getValue(TexturePtr& val) const;
		virtual void getValue(TextureSubresource& val) const;
		virtual void getValue(std::string& val) const;

	protected:
		mutable TextureSubresource mVal;
		std::string mElemType;

	};

	class RenderVariableBuffer : public RenderVariable
	{
	public:
		std::unique_ptr<RenderVariable> clone() override;
		virtual RenderVariable& operator= (GraphicsBufferPtr const value);
		virtual RenderVariable& operator = (std::string const value);
		virtual void getValue(GraphicsBufferPtr& val) const;
		virtual void getValue(std::string & val) const;
	protected:
		GraphicsBufferPtr mVal;
		std::string mElemType;

	};

	class RenderVariableByteAddressBuffer : public RenderVariable
	{
	public:
		std::unique_ptr<RenderVariable> clone() override;
		virtual RenderVariable& operator= (GraphicsBufferPtr const & value);
		virtual RenderVariable& operator=(std::string const & value);
		virtual void getValue(GraphicsBufferPtr& val) const;
		virtual void getValue(std::string & val) const;
	protected:
		GraphicsBufferPtr mVal;
		std::string mElemType;
	};



	typedef RenderVariableConcrete<bool> RenderVariableBool;
	typedef RenderVariableConcrete<uint32_t> RenderVariableUInt;
	typedef RenderVariableConcrete<int32_t> RenderVariableInt;
	typedef RenderVariableConcrete<float> RenderVariableFloat;
	typedef RenderVariableConcrete<uint2> RenderVariableUInt2;
	typedef RenderVariableConcrete<uint3> RenderVariableUInt3;
	typedef RenderVariableConcrete<uint4> RenderVariableUInt4;
	typedef RenderVariableConcrete<int2> RenderVariableInt2;
	typedef RenderVariableConcrete<int3> RenderVariableInt3;
	typedef RenderVariableConcrete<int4> RenderVariableInt4;
	typedef RenderVariableConcrete<float2> RenderVariableFloat2;
	typedef RenderVariableConcrete<float3> RenderVariableFloat3;
	typedef RenderVariableConcrete<float4> RenderVariableFloat4;
	typedef RenderVariableConcrete<SamplerStateObjectPtr> RenderVariableSampler;
	typedef RenderVariableConcrete<std::string> RenderVariableString;
	typedef RenderVariableConcrete<ShaderDesc> RenderVariableShader;
	typedef RenderVariableArray<bool> RenderVariableBoolArray;
	typedef RenderVariableArray<uint32_t> RenderVariableUIntArray;
	typedef RenderVariableArray<int32_t> RenderVariableIntArray;
	typedef RenderVariableArray<float> RenderVariableFloatArray;
	typedef RenderVariableArray<int2> RenderVariableInt2Array;
	typedef RenderVariableArray<int3> RenderVariableInt3Array;
	typedef RenderVariableArray<int4> RenderVariableInt4Array;
	typedef RenderVariableArray<float2> RenderVariableFloat2Array;
	typedef RenderVariableArray<float3> RenderVariableFloat3Array;
	typedef RenderVariableArray<float4> RenderVariableFloat4Array;


	class AIR_CORE_API RenderTechnique : boost::noncopyable
	{
	public:
#if AIR_IS_DEV_PLATFORM
		void load(RenderEffect& effect, XMLNodePtr const & node, uint32_t tech_index);
		void streamOut(RenderEffect const & effect, std::ostream & os, uint32_t tech_index) const;
#endif
		bool streamIn(RenderEffect& effect, ResIdentifierPtr const & res, uint32_t tech_index);
		float getWeight() const;

		size_t getNameHash() const;

		uint32_t getNumMacros() const
		{
			return mMacros ? static_cast<uint32_t>(mMacros->size()) : 0;
		}

		std::pair<std::string, std::string> const & getMacroByIndex(uint32_t n) const
		{
			BOOST_ASSERT(n < getNumMacros());
			return (*mMacros)[n];
		}
		uint32_t getNumPasses() const
		{
			return static_cast<uint32_t>(mPasses.size());
		}

		RenderPass const & getPass(uint32_t n) const
		{
			BOOST_ASSERT(n < this->getNumPasses());
			return *mPasses[n];
		}

		bool hasTessellation() const
		{
			return mHasTessellation;
		}
	private:
		std::string mName;
		size_t mNameHash;
		std::vector<RenderPassPtr> mPasses;
		std::shared_ptr<std::vector<RenderEffectAnnotationPtr>> mAnnotations;
		std::shared_ptr<std::vector<std::pair<std::string, std::string>>> mMacros;

		float mWeight{ 0 };
		bool mTransparent{ false };
		bool mIsValidate{ true };
		bool mHasDiscard;
		bool mHasTessellation{ false };
	};


	class AIR_CORE_API RenderEffectConstantBuffer : boost::noncopyable
	{
	public:
		RenderEffectConstantBuffer();
#if AIR_IS_DEV_PLATFORM
		void load(std::string const & name, ConstantBufferType type);
#endif
		void streamIn(ResIdentifierPtr const & res);

#if AIR_IS_DEV_PLATFORM
		void streamOut(std::ostream& os) const;
#endif

		std::shared_ptr<RenderEffectConstantBuffer> clone(RenderEffect& src_effect, RenderEffect& dst_effect);

		std::string const & getName() const
		{
			return mName->first;
		}

		size_t getNameHash() const;

		void addParameters(uint32_t index) const;

		uint32_t getParametersIndex(uint32_t index) const;

		void resize(uint32_t size);

		ConstantBufferType const getType() const
		{
			return mType;
		}

		template<typename T>
		T const * getVariableInBuff(uint32_t offset) const
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

		uint32_t getNumParameters() const
		{
			return mParamIndices ? static_cast<uint32_t>(mParamIndices->size()) : 0;
		}


	private:
		std::shared_ptr<std::pair<std::string, size_t>> mName;
		std::shared_ptr<std::vector<uint32_t>> mParamIndices;
		GraphicsBufferPtr mHWBuffer;
		std::vector<uint8_t> mBuffer;
		ConstantBufferType mType{ CBT_Object };
		bool mIsDirty;
		int id{ 0 };
	};

	class AIR_CORE_API SharedConstantBuffer : boost::noncopyable
	{
	public:
		SharedConstantBuffer();

		RenderEffectConstantBufferPtr getCBuffer();

		RenderEffectParameter* getParameterByIndex(uint32_t index) const;

		uint32_t const getNumParams() const
		{
			return mParams.size();
		}

		bool isInit() const
		{
			return mIsInit;
		}
		void setInit()
		{
			mIsInit = true;
		}
	public:
		RenderEffectConstantBufferPtr mCBuffer;
		std::vector<std::unique_ptr<RenderEffectParameter>> mParams;
		bool mIsInit{ false };
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

		uint32_t getType() const
		{
			return mType;
		}

		RenderVariable const & getVar() const
		{
			return *mVar;
		}

		std::shared_ptr<std::string> const & getArraySize() const
		{
			return mArraySize;
		}

		std::string const & getName() const
		{
			return mName->first;
		}

		size_t getNameHash() const
		{
			return mName->second;
		}
		bool hasSematic() const
		{
			return !!mSemantic;
		}

		std::string const & getSemantic() const
		{
			return mSemantic->first;
		}

		size_t getSemanticHash() const
		{
			return mSemantic->second;
		}

		uint32_t getNumAnnotation() const
		{
			return mAnnotations ? static_cast<uint32_t>(mAnnotations->size()) : 0;
		}

		RenderEffectAnnotation const & getAnnotation(uint32_t n) const
		{
			BOOST_ASSERT(n < this->getNumAnnotation());
			return *(*mAnnotations)[n];
		}

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

		ShaderObjectPtr const & getShaderObjectByIndex(uint32_t n) const
		{
			BOOST_ASSERT(n < mShaderObjs.size());
			return mShaderObjs[n];
		}

		uint32_t addShaderObject();

		std::string const & getHLSHShader() const;

	private:
		RenderEffectTemplatePtr mEffectTemplate;
		std::vector<std::unique_ptr<RenderEffectParameter>> mParams;
		std::vector<std::shared_ptr<RenderEffectConstantBuffer>> mCbuffers;
		std::vector<SharedConstantBuffer*> mSharedBuffers;
		std::vector<ShaderObjectPtr> mShaderObjs;
		bool isClone{ false };


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
		uint32_t getNumTechniques() const
		{
			return static_cast<uint32_t>( mTechniques.size());
		}
		RenderTechnique* getTechniqueByName(std::string_view name) const;
		RenderTechnique* getTechniqueByIndex(uint32_t index) const;

		std::string const & getResourceName()
		{
			return mResName;
		}

		size_t const getResourceNameHash()
		{
			return mResNameHash;
		}

		uint32_t getNumShaderFragments() const
		{
			return static_cast<uint32_t>(mShaderFrags.size());
		}
		RenderShaderFragment const & getShaderFragmentByIndex(uint32_t n) const
		{
			BOOST_ASSERT(n < this->getNumShaderFragments());
			return mShaderFrags[n];
		}

		uint32_t addShaderDesc(ShaderDesc const &sd);
		ShaderDesc& getShaderDesc(uint32_t id);
		ShaderDesc const & getShaderDesc(uint32_t id) const;
		uint32_t getNumMacros() const;
		std::pair<std::string, std::string> const & getMacroByIndex(uint32_t n) const;
		std::string const & getTypeName(uint32_t code) const;

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
		std::string const & getName() const
		{
			return mName;
		}

		size_t getNameHash() const
		{
			return mNameHash;
		}

		void bind(RenderEffect const & effect) const;

		void unbind(RenderEffect const & effect) const;
		bool isValidate() const
		{
			return mIsValidate;
		}

		ShaderObjectPtr const & getShaderObject(RenderEffect const & effect) const
		{
			return effect.getShaderObjectByIndex(mShaderObjectIndex);
		}

		uint32_t getNumMacros() const
		{
			return mMacros ? static_cast<uint32_t>(mMacros->size()) : 0;
		}

		std::pair<std::string, std::string> const & getMacroByIndex(uint32_t n) const 
		{
			BOOST_ASSERT(n < getNumMacros());
			return (*mMacros)[n];
		}
	private:
		std::string mName;
		size_t mNameHash;
		std::shared_ptr<std::vector<RenderEffectAnnotationPtr>> mAnnotations;
		std::shared_ptr<std::vector<std::pair<std::string, std::string>>> mMacros;
		std::array<uint32_t, ShaderObject::ST_NumShaderTypes> mShaderDescIds;
		RenderStateObjectPtr mRenderStateObject;
		uint32_t mShaderObjectIndex;
		bool mIsValidate;
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

		ShaderObject::ShaderType getType() const
		{
			return mType;
		}
		ShaderModel getVersion() const
		{
			return mVer;
		}

		std::string const & getString() const
		{
			return mStr;
		}

		ShaderObject::ShaderType mType;
		ShaderModel mVer;
		std::string mStr;
	};



	AIR_CORE_API RenderEffectPtr syncLoadRenderEffect(std::string const & effect_name);
	AIR_CORE_API RenderEffectPtr asyncLoadRenderEffect(std::string const & effect_name);
}


#endif