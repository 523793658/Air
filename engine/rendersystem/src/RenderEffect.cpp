#include <boost/functional/hash.hpp>

#include <fstream>
#include "Engine.h"
#include "basic/include/ErrorHanding.hpp"
#include "basic/include/ResIdentifier.h"
#include "core/include/ResLoader.h"
#include "basic/include/XMLDom.hpp"
#include "rendersystem/include/RenderEffect.hpp"

namespace
{
	using namespace Air;
	uint32_t const KFX_VERSION = 0x0110;
	std::mutex singleton_mutex;

	class type_define {
	public:
		static type_define& getInstance()
		{
			if (!mInstance)
			{
			std::lock_guard<std::mutex> lock(singleton_mutex);
			if (!mInstance)
			{
				mInstance = MakeUniquePtr<type_define>();
			}
			}
			return *mInstance;
		}
		uint32_t getTypeCode(std::string_view name) const
		{
			size_t const name_hash = boost::hash_range(name.begin(), name.end());
			for (uint32_t i = 0; i < mTypesHash.size(); ++i)
			{
				if (mTypesHash[i] == name_hash)
				{
					return i;
				}
			}
			AIR_UNREACHABLE("Invalid type name");
		}

		std::string const & getTypeName(uint32_t code) const
		{
			if (code < mTypes.size())
			{
				return mTypes[code];
			}

		}

		type_define()
		{
			mTypes.push_back("bool");
			mTypes.push_back("string");
			mTypes.push_back("texture1D");
			mTypes.push_back("texture2D");
			mTypes.push_back("texture3D");
			mTypes.push_back("textureCUBE");
			mTypes.push_back("texture1DArray");
			mTypes.push_back("texture2DArray");
			mTypes.push_back("texture3DArray");
			mTypes.push_back("textureCUBEArray");
			mTypes.push_back("sampler");
			mTypes.push_back("shader");
			mTypes.push_back("uint");
			mTypes.push_back("uint2");
			mTypes.push_back("uint3");
			mTypes.push_back("uint4");
			mTypes.push_back("int");
			mTypes.push_back("int2");
			mTypes.push_back("int3");
			mTypes.push_back("int4");
			mTypes.push_back("float");
			mTypes.push_back("float2");
			mTypes.push_back("float2x2");
			mTypes.push_back("float2x3");
			mTypes.push_back("float2x4");
			mTypes.push_back("float3");
			mTypes.push_back("float3x2");
			mTypes.push_back("float3x3");
			mTypes.push_back("float3x4");
			mTypes.push_back("float4");
			mTypes.push_back("float4x2");
			mTypes.push_back("float4x3");
			mTypes.push_back("float4x4");
			mTypes.push_back("buffer");
			mTypes.push_back("structured_buffer");
			mTypes.push_back("byte_address_buffer");
			mTypes.push_back("rw_buffer");
			mTypes.push_back("rw_structured_buffer");
			mTypes.push_back("rw_texture1D");
			mTypes.push_back("rw_texture2D");
			mTypes.push_back("rw_texture3D");
			mTypes.push_back("rw_texture1DArray");
			mTypes.push_back("rw_texture2DArray");
			mTypes.push_back("rw_byte_address_buffer");
			mTypes.push_back("append_structured_buffer");
			mTypes.push_back("consume_structured_buffer");

			mTypesHash.resize(mTypes.size());
			for (size_t i = 0; i < mTypes.size(); ++i)
			{
				mTypesHash[i] = RT_HASH(mTypes[i].c_str());
			}
		}

	private:
		std::vector<std::string> mTypes;
		std::vector<size_t> mTypesHash;
		static std::unique_ptr<type_define> mInstance;
	};
	std::unique_ptr<type_define> type_define::mInstance;



#if AIR_IS_DEV_PLATFORM
	bool BoolFromStr(std::string_view name)
	{
		if (("true" == name) || ("1" == name))
		{
			return true;
		}
		else
		{
			return false;
		}
	}



	std::unique_ptr<RenderVariable> read_var(XMLNodePtr const & node, uint32_t type, uint32_t array_size)
	{
		std::unique_ptr<RenderVariable> var;
		XMLAttributePtr attr;
		switch (type)
		{
		case REDT_bool:
			if (0 == array_size)
			{
				attr = node->getAttrib("value");
				bool tmp = false;
				if (attr)
				{
					std::string value_str = attr->getValueString();
					tmp = BoolFromStr(value_str);
				}
				var = MakeUniquePtr<RenderVariableBool
			}
			break;
		default:
			break;
		}
	}


#endif
}


namespace Air
{
	class EffectLoadingDesc : public ResLoadingDesc
	{
	private:
		struct EffectDesc
		{
			std::string mResName;
			RenderEffectPtr mEffect;
		};
	public:
		explicit EffectLoadingDesc(std::string const & name)
		{
			mEffectDesc.mResName = name;
		}

		uint64_t getType() const override
		{
			static uint64_t const type = CT_HASH("EffectLoadingDesc");
			return type;
		}
		bool getStateLess() const override
		{
			return false;
		}

		void subThreadStage() override
		{

		}
		void mainThreadStage() override
		{
			mEffectDesc.mEffect = MakeSharedPtr<RenderEffect>();
			mEffectDesc.mEffect->load(mEffectDesc.mResName);
		}

		bool hasSubThreadStage() const override
		{
			return false;
		}

		bool match(ResLoadingDesc const & rhs) const
		{
			if (this->getType() == rhs.getType())
			{
				EffectLoadingDesc const & eld = static_cast<EffectLoadingDesc const &>(rhs);
				return (mEffectDesc.mResName == eld.mEffectDesc.mResName);
			}
			return false;
		}

		void copyDataFrom(ResLoadingDesc const & rhs)
		{
			BOOST_ASSERT(this->getType() == rhs.getType());
			EffectLoadingDesc const & eld = static_cast<EffectLoadingDesc const &>(rhs);
			mEffectDesc.mResName = eld.mEffectDesc.mResName;
		}

		std::shared_ptr<void> cloneResourceFrom(std::shared_ptr<void> const& resource)
		{
			mEffectDesc.mEffect = std::static_pointer_cast<RenderEffect>(resource)->clone();
			return std::static_pointer_cast<void>(mEffectDesc.mEffect);
		}
		std::shared_ptr<void> getResource() const override
		{
			return mEffectDesc.mEffect;
		}
	private:
		EffectDesc mEffectDesc;
	};

#if AIR_IS_DEV_PLATFORM
	void RenderTechnique::load(RenderEffect& effect, XMLNodePtr const & node, uint32_t tech_index)
	{
		mName = node->getAttrib("name")->getValueString();
		mNameHash = boost::hash_range(mName.begin(), mName.end());
		RenderTechnique* parent_tech = nullptr;
		XMLAttributePtr inherit_attr = node->getAttrib("inherit");
		if (inherit_attr)
		{
			std::string inherit = inherit_attr->getValueString();
			BOOST_ASSERT(inherit != mName);
			parent_tech = effect.getTechniqueByName(inherit);
			BOOST_ASSERT(parent_tech);
		}
		{
			XMLNodePtr anno_node = node->getFirstNode("annotation");
			if (anno_node)
			{
				mAnnotations = MakeSharedPtr < std::remove_reference<decltype(*mAnnotations)>::type>();
				if (parent_tech && parent_tech->mAnnotations)
				{
					*mAnnotations = *parent_tech->mAnnotations;
				}

				for (; anno_node; anno_node = anno_node->getNextSibling("annotation"))
				{
					RenderEffectAnnotationPtr annotation = MakeSharedPtr<RenderEffectAnnotation>();
					mAnnotations->push_back(annotation);
					annotation->load(anno_node);
				}
			}
			else if (parent_tech)
			{
				mAnnotations = parent_tech->mAnnotations;
			}
		}
	}
#endif
	
	float  RenderTechnique::getWeight() const
	{
		return mWeight;
	}

	size_t RenderTechnique::getNameHash() const
	{
		return mNameHash;
	}


	RenderEffectConstantBuffer::RenderEffectConstantBuffer() : mIsDirty(true)
	{

	}
#if AIR_IS_DEV_PLATFORM
	void RenderEffectConstantBuffer::load(std::string const & name)
	{
		mName = MakeSharedPtr<std::remove_reference<decltype(*mName)>::type>();
		mName->first = name;
		mName->second = boost::hash_range(mName->first.begin(), mName->first.end());
		mParamIndices = MakeSharedPtr<std::remove_reference<decltype(*mParamIndices)>::type>();
	}
#endif
	void RenderEffectConstantBuffer::streamIn(ResIdentifierPtr const & res)
	{
		mName = MakeSharedPtr<std::remove_reference<decltype(*mName)>::type>();
		mName->first = readShortString(res);
		mName->second = boost::hash_range(mName->first.begin(), mName->first.end());
		mParamIndices = MakeSharedPtr < std::remove_reference<decltype(*mParamIndices)>::type>();
		uint16_t len;
		res->read(&len, sizeof(len));
		len = LE2Native(len);
		mParamIndices->resize(len);
		res->read(&(*mParamIndices)[0], len * sizeof((*mParamIndices)[0]));
		for (uint32_t i = 0; i < len; i++)
		{
			(*mParamIndices)[i] = LE2Native((*mParamIndices)[i]);
		}
	}

#if AIR_IS_DEV_PLATFORM
	void RenderEffectConstantBuffer::streamOut(std::ostream& os) const
	{
		writeShortString(os, mName->first);
		uint16_t len = Native2LE(static_cast<uint16_t>(mParamIndices->size()));
		os.write(reinterpret_cast<char const *>(&len), sizeof(len));
		for (size_t i = 0; i < mParamIndices->size(); ++i)
		{
			uint32_t tmp = Native2LE((*mParamIndices)[i]);
			os.write(reinterpret_cast<char const*>(&tmp), sizeof(tmp));
		}
	}
#endif



	std::unique_ptr<RenderEffectConstantBuffer> RenderEffectConstantBuffer::clone(RenderEffect& src_effect, RenderEffect& dst_effect)
	{
		auto ret = MakeUniquePtr<RenderEffectConstantBuffer>();
		ret->mName = mName;
		ret->mParamIndices = mParamIndices;
		ret->mBuffer = mBuffer;
		ret->resize(static_cast<uint32_t>(mBuffer.size()));
		for (size_t i = 0; i < mParamIndices->size(); ++i)
		{
			RenderEffectParameter* src_param = src_effect.getParameterByIndex((*mParamIndices)[i]);
			if (src_param->inCBuffer())
			{
				RenderEffectParameter* dst_param = dst_effect.getParameterByIndex((*mParamIndices)[i]);
				dst_param->rebindToCBuffer(*ret);
			}
		}
		return ret;
	}

	void RenderEffect::load(std::string const & name)
	{
		mEffectTemplate = MakeSharedPtr<RenderEffectTemplate>();
		mEffectTemplate->load(name, *this);
	}

	RenderTechnique* RenderEffect::getTechniqueByName(std::string_view name) const
	{
		return mEffectTemplate->getTechniqueByName(name);
	}
	RenderTechnique* RenderEffect::getTechniqueByIndex(uint32_t n) const
	{
		return mEffectTemplate->getTechniqueByIndex(n);
	}

	RenderTechnique* RenderEffectTemplate::getTechniqueByName(std::string_view name) const
	{
		size_t const name_hash = boost::hash_range(name.begin(), name.end());
		for (auto const & tech : mTechniques)
		{
			if (name_hash == tech->getNameHash())
			{
				return tech.get();
			}
		}
		return nullptr;
	}

	RenderTechnique* RenderEffectTemplate::getTechniqueByIndex(uint32_t index) const
	{
		BOOST_ASSERT(index < this->getNumTechniques());
		return mTechniques[index].get();
	}

	void RenderEffectTemplate::load(std::string const & name, RenderEffect& effect)
	{
		std::string fxml_name = ResLoader::getInstance().locate(name);
		if (fxml_name.empty())
		{
			fxml_name = name;
		}
		std::string kfx_name = fxml_name.substr(0, fxml_name.rfind(".")) + ".kfx";
#if AIR_IS_DEV_PLATFORM
		ResIdentifierPtr source = ResLoader::getInstance().open(fxml_name);
#endif
		ResIdentifierPtr kfx_source = ResLoader::getInstance().open(kfx_name);
#if AIR_IS_DEV_PLATFORM
		std::unique_ptr<XMLDocument> doc;
		XMLNodePtr root;
#endif
		mResName = fxml_name;
		mResNameHash = boost::hash_range(fxml_name.begin(), fxml_name.end());
#if AIR_IS_DEV_PLATFORM
		if (source)
		{
			mTimestamp = source->getTimestamp();
			doc = MakeUniquePtr<XMLDocument>();
			root = doc->parse(source);
			std::vector<std::string> include_names;
			this->recursiveIncludeNode(*root, include_names);
			for (auto const & include_name : include_names)
			{
				ResIdentifierPtr include_source = ResLoader::getInstance().open(include_name);
				if (include_source)
				{
					mTimestamp = std::max(mTimestamp, include_source->getTimestamp());
				}
			}
		}
		else
		{
			mTimestamp = 0;
		}
#endif
		if (!this->streamIn(kfx_source, effect))
		{
#if AIR_IS_DEV_PLATFORM
			if (source)
			{
				effect.mParams.clear();
				effect.mCbuffers.clear();
				effect.mShaderObjs.clear();
				mMacros.reset();
				mShaderFrags.clear();
				mHLSLShader.clear();
				mTechniques.clear();

				mShaderDescs.resize(1);
				XMLAttributePtr attr;
				std::vector<std::unique_ptr<XMLDocument>> include_docs;
				std::vector<std::string> whole_include_names;
				for (XMLNodePtr node = root->getFirstNode("include");
					node;)
				{
					attr = node->getAttrib("name");
					BOOST_ASSERT(attr);
					std::string include_name = attr->getValueString();
					include_docs.push_back(MakeUniquePtr<XMLDocument>());
					XMLNodePtr include_root = include_docs.back()->parse(ResLoader::getInstance().open(include_name));
					std::vector<std::string> include_names;
					this->recursiveIncludeNode(*include_root, include_names);
					if (!include_names.empty())
					{
						for (auto iter = include_names.begin(); iter != include_names.end();)
						{
							bool found = false;
							for (auto iter_w = whole_include_names.begin(); iter_w != whole_include_names.end(); ++iter_w)
							{
								if (*iter == *iter_w)
								{
									found = true;
									break;
								}
							}
							if (found)
							{
								iter = include_names.erase(iter);
							}
							else
							{
								include_docs.push_back(MakeUniquePtr<XMLDocument>());
								XMLNodePtr recursive_include_root = include_docs.back()->parse(ResLoader::getInstance().open(*iter));
								this->insertIncludeNodes(*doc, *root, node, *recursive_include_root);
								whole_include_names.push_back(*iter);
								++iter;
							}
						}
					}
					bool found = false;
					for (auto iter_w = whole_include_names.begin(); iter_w != whole_include_names.end(); ++iter_w)
					{
						if (include_name == *iter_w)
						{
							found = true;
							break;
						}
					}
					if (!found)
					{
						this->insertIncludeNodes(*doc, *root, node, *include_root);
						whole_include_names.push_back(include_name);
					}
					XMLNodePtr node_next = node->getNextSibling("include");
					root->removeNode(node);
					node = node_next;
				}
				{
					XMLNodePtr macro_node = root->getFirstNode("macro");
					if (macro_node)
					{
						mMacros = MakeSharedPtr<std::remove_reference<decltype(*mMacros)>::type>();
					}
					for (; macro_node; macro_node = macro_node->getNextSibling("macro"))
					{
						mMacros->emplace_back(std::make_pair(macro_node->getAttrib("name")->getValueString(), macro_node->getAttrib("value")->getValueString()), true);
					}
				}
				std::vector<XMLNodePtr> parameter_nodes;

				for (XMLNodePtr node = root->getFirstNode(); node; node->getNextSibling())
				{
					if ("parameter" == node->getName())
					{
						parameter_nodes.push_back(node);
					}
					else if ("cbuffer" == node->getName())
					{
						for (XMLNodePtr sub_node = node->getFirstNode("parameter"); sub_node; sub_node = sub_node->getNextSibling("parameter"))
						{
							parameter_nodes.push_back(sub_node);
						}
					}
				}
				for (uint32_t param_index = 0; param_index < parameter_nodes.size(); ++param_index)
				{
					XMLNodePtr const & node = parameter_nodes[param_index];
					uint32_t type = type_define::getInstance().getTypeCode(node->getAttrib("type")->getValueString());
					if ((type != REDT_sampler) && (type != REDT_texture1D) && (type != REDT_texture2D) && (type != REDT_texture3D) && (type != REDT_textureCUBE) && (type != REDT_texture1DArray) && (type != REDT_texture2DArray) && (type != REDT_texture3DArray) && (type != REDT_textureCUBEArray) && (type != REDT_structured_buffer) && (type != REDT_byte_address_buffer) && (type != REDT_rw_buffer)
						&& (type != REDT_rw_structured_buffer) && (type != REDT_rw_texture1D)
						&& (type != REDT_rw_texture2D) && (type != REDT_rw_texture3D)
						&& (type != REDT_rw_texture1DArray) && (type != REDT_rw_texture2DArray)
						&& (type != REDT_rw_byte_address_buffer) && (type != REDT_append_structured_buffer)
						&& (type != REDT_consume_structured_buffer))
					{
						RenderEffectConstantBuffer* cbuff = nullptr;
						XMLNodePtr parent_node = node->getParent();
						std::string cbuff_name = parent_node->getAttribString("name", "global_cb");
						size_t const cbuff_name_hash = RT_HASH(cbuff_name.c_str());
						bool found = false;
						for (size_t i = 0; i < effect.mCbuffers.size(); i++)
						{
							if (effect.mCbuffers[i]->getNameHash() == cbuff_name_hash)
							{
								cbuff = effect.mCbuffers[i].get();
								found = true;
								break;
							}
						}
						if (!found)
						{
							effect.mCbuffers.push_back(MakeUniquePtr<RenderEffectConstantBuffer>());
							cbuff = effect.mCbuffers.back().get();
							cbuff->load(cbuff_name);
						}
						BOOST_ASSERT(cbuff);
						cbuff->addParameters(param_index);
					}

					effect.mParams.push_back(MakeUniquePtr<RenderEffectParameter>());
					effect.mParams.back()->load(node);
				}
				for (XMLNodePtr shader_node = root->getFirstNode("shader"); shader_node; shader_node = shader_node->getNextSibling("shader"))
				{
					mShaderFrags.push_back(RenderShaderFragment());
					mShaderFrags.back().load(shader_node);
				}
				this->GenHLSLShaderText(effect);
				uint32_t index = 0;
				for (XMLNodePtr node = root->getFirstNode("technique"); node; node = node->getNextSibling("technique"), ++index)
				{
					mTechniques.push_back(MakeUniquePtr<RenderTechnique>());
					mTechniques.back()->load(effect, node, index);
				}
			}
			std::ofstream ofs(kfx_name.c_str(), std::ios_base::binary | std::ios_base::out);
			this->streamOut(ofs, effect);
#endif
		}

	}

#if AIR_IS_DEV_PLATFORM
	void RenderEffectTemplate::GenHLSLShaderText(RenderEffect const & effect)
	{

	}
	std::string const & RenderEffectTemplate::getHLSLShaderText() const
	{
		return mHLSLShader;
	}
#endif

	RenderEffectPtr syncLoadRenderEffect(std::string const & effect_name)
	{
		return ResLoader::getInstance().syncQueryT<RenderEffect>(MakeUniquePtr<EffectLoadingDesc>(effect_name));
	}

#if AIR_IS_DEV_PLATFORM
	void RenderEffectAnnotation::load(XMLNodePtr const & node)
	{
		mType = type_define::getInstance().getTypeCode(node->getAttribString("type"));
		mName = node->getAttribString("name");
		mVal = read_var()
	}
#endif
}