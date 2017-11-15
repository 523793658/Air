#ifndef _UnifromBuffer_H_
#define _UnifromBuffer_H_
#pragma once
#include "rendersystem/include/RenderDefinitions.hpp"
#include "basic/include/AlignmentTemplates.hpp"
#include "rendersystem/include/ShaderParameters.hpp"
#include "rendersystem/include/HardwareResource.hpp"
namespace Air
{
#define UNIFORM_BUFFER_STRUCT_ALIGNMENT 16

	namespace EShaderPricisionModifier
	{
		enum Type
		{
			Float,
			Half,
			Fixel
		};
	}

	class UniformBufferStruct;


	template<typename>
	class TUniformBufferTypeInfo
	{
	public:
		enum { BaseType = UBMT_INVALID };
		enum { NumRows = 0 };
		enum { NumColumns = 0 };
		enum { NumElements = 0 };
		enum { Alignment = 1 };
		enum { IsResource = 0 };
		static const UniformBufferStruct* getStruct() { return nullptr };
	};

	struct ResourceTableEntry
	{
		std::string mUniformBufferName;
		uint16_t mType;
		uint16_t mResourceIndex;
	};

	class AIR_CORE_API UniformBufferStruct
	{
	public:
		class Member
		{
		public:
			Member(
				const char* inName,
				const char* inShaderType,
				uint32_t inOffset,
				EUniformBufferBaseType inBaseType,
				EShaderPricisionModifier::Type inPricision,
				uint32_t inNumRows,
				uint32_t inNumColumns,
				uint32_t inNumElements,
				const UniformBufferStruct* inStruct
			)
				:
				mName(inName),
				mShaderType(inShaderType),
				mOffset(inOffset),
				mBaseType(inBaseType),
				mPrecision(inPricision),
				mNumRows(inNumRows),
				mNumColumns(inNumColumns),
				mNumElements(inNumElements),
				mStruct(inStruct)
			{
			}

			const char* getName() const {
				return mName.c_str();
			}
			const char* getShaderType() const {
				return mShaderType.c_str();
			}
			uint32_t getOffset() const {
				return mOffset;
			}
			EUniformBufferBaseType getBaseType() const {
				return mBaseType;
			}

			EShaderPricisionModifier::Type getPrecision() const {
				return mPrecision;
			};

			uint32_t getNumRows() const {
				return mNumRows;
			};
			uint32_t getNumColumns() const {
				return mNumColumns;
			}
			uint32_t getNumElements() const {
				return mNumElements;
			}

			const UniformBufferStruct* getStruct() const {
				return mStruct;
			}

		private:
			const std::string mName;
			const std::string mShaderType;
			uint32_t mOffset;
			EUniformBufferBaseType mBaseType;
			EShaderPricisionModifier::Type mPrecision;
			uint32_t mNumRows;
			uint32_t mNumColumns;
			uint32_t mNumElements;
			const UniformBufferStruct* mStruct;
		};

		typedef class ShaderUniformBufferParameter* (*ConstructUniformBufferParameterType)();

		UniformBufferStruct(const std::string & inLayoutName, const std::string inStructTypeName, const std::string inShaderVariableName, ConstructUniformBufferParameterType inConstructRef, uint32_t inSize, const std::vector<Member>& inMembers, bool bRegisterForAutoBinding)
			:
			mStructTypeName(inStructTypeName),
			mShaderVariableName(inShaderVariableName),
			mConstructUniformBufferParameterRef(inConstructRef),
			mSize(inSize),
			mLayout(inLayoutName),
			mMembers(inMembers)
		{
			mGlobalListLink.push_back(this);
			bool bHasDeclaredResource = false;
			mLayout.mConstantBufferSize = inSize;
			mLayout.mResourceOffset = 0;
			for (uint32_t i = 0; i < mMembers.size(); ++i)
			{
				bool bIsResource = isUniformBufferResourceType(mMembers[i].getBaseType());
				
				BOOST_ASSERT_MSG(!bHasDeclaredResource || bIsResource, (TEXT("Constructing invalid uniform buffer struct: member '%s' is not a resource but has been declared after a resource member."), mMembers[i].getName()));

				if (bIsResource)
				{
					if (!bHasDeclaredResource)
					{
						mLayout.mConstantBufferSize = (i == 0) ? 0 : align(mMembers[i].getOffset(), UNIFORM_BUFFER_STRUCT_ALIGNMENT);
						mLayout.mResourceOffset = mMembers[i].getOffset();
					}
					mLayout.mResource.push_back(static_cast<uint8_t>(mMembers[i].getBaseType()));
				}
				bHasDeclaredResource |= bIsResource;
			}
			if (bRegisterForAutoBinding)
			{
				mGlobalListLink.assign(getStructList()->begin(), getStructList()->end());
			}

		}

		virtual ~UniformBufferStruct()
		{
			mGlobalListLink.clear();
			getNameStructMap().erase(getNameStructMap().find(mStructTypeName));
		}

		void addResourceTableEntries(std::unordered_map<std::string, ResourceTableEntry>& ResourceTableMap, std::unordered_map<std::string, uint32_t>& resourceTableLayoutHashes) const
		{
			uint16_t resouceIndex = 0;
			for (int32_t memberIndex = 0; memberIndex < mMembers.size(); ++memberIndex)
			{
				const Member& member = mMembers[memberIndex];
				if (isUniformBufferResourceType(member.getBaseType()))
				{
					std::string key = mShaderVariableName + "_" + member.getName();
					auto it = ResourceTableMap.find(key);
					if (it == ResourceTableMap.end())
					{
						ResourceTableEntry v;
						v.mUniformBufferName = mShaderVariableName;
						v.mType = static_cast<uint16_t>(member.getBaseType());
						v.mResourceIndex = resouceIndex++;
						ResourceTableMap.emplace(key, v);
					}
				}
			}
			resourceTableLayoutHashes.emplace(mShaderVariableName, getLayout().getHash());
		}

		const std::string getStructTypeName() const {
			return mStructTypeName;
		};
		const std::string getShaderVariableName() const {
			return mShaderVariableName;
		}
		const uint32_t getSize() const {
			return mSize;
		}
		const RHIUniformBufferLayout& getLayout() const {
			return mLayout;
		}

		const std::vector<Member> & getMembers() const {
			return mMembers;
		}

		ShaderUniformBufferParameter* getConstructTypedParameter() const {
			return (*mConstructUniformBufferParameterRef)();
		}



		static std::list<UniformBufferStruct*>*& getStructList();
		static std::unordered_map<std::string, UniformBufferStruct*>& getNameStructMap();



	private:
		const std::string mStructTypeName;
		const std::string mShaderVariableName;
		ConstructUniformBufferParameterType mConstructUniformBufferParameterRef;
		uint32_t mSize;
		RHIUniformBufferLayout mLayout;
		std::vector<Member> mMembers;
		std::list<UniformBufferStruct*> mGlobalListLink;
	};


	template<typename BufferStruct>
	class UnifromBuffer : public RenderResource
	{
	public:
	private:
		UniformBufferRHIPtr mUniformBufferRHI;
		uint8_t * mContents;
	};


	template<typename BufferStruct>
	class UnifromBufferRef : public UniformBufferRHIPtr
	{
	public:

	};
#define BEGIN_UNIFORM_BUFFER_STRUCT_EX(StructTypeName, PrefixKeywords, ConstructorSuffix) \
	AIR_ALIGN(UNIFORM_BUFFER_STRUCT_ALIGNMENT) class PrefixKeywords StructTypeName \
	{	\
	public:	\
		StructTypeName() ConstructorSuffix	\
		static UniformBufferStruct mStaticStruct;	\
		static ShaderUniformBufferParameter* constructUniformBufferParameter(){return new TShaderUniformBufferParameter<StructTypeName>();} \
		static UniformBufferRHIPtr createUniformBuffer(const StructTypeName& inContents, EUniformBufferUsage inUsage)   \
		{	\
			return RHICreateUniformBuffer(&inContents, mStaticStruct.getLayout(), inUsage);	\
		}	\
	private:	\
		typedef StructTypeName zzTThisStruct;	\
		struct zzFirstMemberId { enum {HasDeclaredResource = 0};};	\
		static std::vector<UniformBufferStruct::Member> zzGetMembersBefore(zzFirstMemberId) \
		{	\
			return std::vector<UniformBufferStruct::Member>();	\
		}	\
		typedef zzFirstMemberId

#define BEGIN_UNIFORM_BUFFER_STRUCT(StructTypeName, PrefixKeywords) BEGIN_UNIFORM_BUFFER_STRUCT_EX(StructTypeName, PrefixKeywords,, {})

#define BEGIN_UNIFORM_BUFFER_STRUCT_WITH_CONSTRUCTOR(StructTypeName, PrefixKeywords) BEGIN_UNIFORM_BUFFER_STRUCT_EX(StructTypeName, PrefixKeywords, ;)


#define END_UNIFORM_BUFFER_STRUCT(Name) \
		zzLastMemberId;	\
		static std::vector<UniformBufferStruct::Member> zzGetMembers() {return zzGetMembersBefore(zzLastMemberId());} \
		} AIR_ALIGN(UNIFORM_BUFFER_STRUCT_ALIGNMENT); \
		template<> class TUniformBufferTypeInfo<Name>	\
		{	\
		public:	\
			enum {BaseType = UBMT_STRUCT};	\
			enum {NumRows = 1};			\
			enum {NumColumns = 1};	\
			enum {Alignment = UNIFORM_BUFFER_STRUCT_ALIGNMENT};	\
			static const UniformBufferStruct* getStruct() {return &Name::mStaticStruct;}	\
		};


#define DECALRE_UNIFORM_BUFFER_STRUCT_MEMBER_EXPLICIT (MemberType, MemberName, ArrayDecl, Precision, optionalShaderType)	  \
		zzMemberId##MemberName;		\
	public:	\
		typedef MemberType zzA##MemberName ArrayDecl;	\
		typedef TUniformBufferTypeInfo<zzA##MemberName>::TAlignedType zzT##MemberName; \
		zzT##MemberName MemberName;		\
		static_assert(TUniformBufferTypeInfo<zzA##MemberName>::mBaseType != UBMT_INVALID, "Invalid type " #MemberType " of member " #MemberName ".");	  \
		static_assert(TUniformBufferTypeInfo<zzA##MemberName>::mBaseType != UBMT_UAV, "UAV is not yet supported in resource tables for " #MemberName " of type " #MemberType ".");  \
	private:	\
	struct zzNextMemberId##MemberName{ enum {	\
 HasDeclaredResource = zzMemberId##MemberName::mHasDeclaredResource || TUniformBufferTypeInfo<zzT##MemberName>::IsResource}; };	\
		static std::vector<UniformBufferStruct::Member> zzGetMembersBefore(zzNextMemberId##MemberName) \
		{	\
			static_assert(TUniformBufferTypeInfo<zzT##MemberName>::IsResource == 1 || zzMemberId##MemberName::mHasDeclaredResource == 0, "All resources must be declared last for " #MemberName ".")	\
			static_assert(TUniformBufferTypeInfo<zzT##MemberName>::IsResource == 0, "No shader type for " #MemberName ".");	  \
			std::vector<UniformBufferStruct::Member> OutMembers = zzGetMembersBefore(zzMemberId##MemberName());	\
			OutMembers.push_back(UniformBufferStruct::Member(	\
				TEXT(#MemberName),	\
				OptionalShaderType,		\
				STRUCT_OFFSET(zzTThisStruct, MemberName),	\
				(EUniformBufferBaseType)TUniformBufferTypeInfo<zzA##MemberName>::BaseType,	\
				Precision,	\
				TUniformBufferTypeInfo<zzA##MemberName>::NumRows,	\
				TUniformBufferTypeInfo<zzA##MemberName>::NumColumns,	\
				TUniformBufferTypeInfo<zzA##MemberName>::NumElements,	\
				TUniformBufferTypeInfo<zzA##MemberName>::getStruct()	\
				));	\
			static_assert(	\
				(STRUCT_OFFSET(zzTThisStruct, MemberName) & (TUniformBufferTypeInfo<zzA##MemberName>::mAlignment -1)) == 0,		\
					"Misaligned uniform buffer struct member " #MemberName ".");	\
			return OutMembers;	\
		}\
		typedef zzNextMemberId##MemberName


#define DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER_EX(MemberType, MemberName, Precision) DECALRE_UNIFORM_BUFFER_STRUCT_MEMBER_EXPLICIT(MemberType, MemberName,, Precision, TEXT(""))
			








}




#endif
