#ifndef _GraphicsBuffer_H_
#define _GraphicsBuffer_H_
#pragma once
#include "PreDeclare.h"
#include "boost/noncopyable.hpp"
namespace Air
{
	enum BufferUsage
	{
		BU_Static,
		BU_Dynamic
	};

	enum BufferAccess
	{
		BA_Read_Only,
		BA_Write_Only,
		BA_Read_Write,
		BA_Write_No_Overwrite
	};

	class AIR_CORE_API GraphicsBuffer
	{
	public:
		class Mapper : boost::noncopyable
		{
			friend class GraphicsBuffer;
		public:
			Mapper(GraphicsBuffer & buffer, BufferAccess ba)
				:mBuffer(buffer)
			{
				mData = buffer.map(ba);
			}
			~Mapper()
			{
				mBuffer.unmap();
			}
			template<typename T>
			const T* pointer() const
			{
				return static_cast<T*>(mData);
			}

			template<typename T>
			T* pointer()
			{
				return static_cast<T*>(mData);
			}
		private:
			GraphicsBuffer& mBuffer;
			void* mData;
		};
	public:
		GraphicsBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte);
		virtual ~GraphicsBuffer();

		uint32_t getSize() const
		{
			return mSizeInByte;
		}

		BufferUsage getUsage() const
		{
			return mUsage;
		}

		uint32_t getAccessHint() const
		{
			return mAccessHint;
		}

		virtual void copyToBuffer(GraphicsBuffer& rhs) = 0; 
		virtual void createHWResouce(void const * init_data) = 0;
		virtual void deleteHWResource() = 0;
		virtual void updateSubResource(uint32_t offset, uint32_t size, void const * data) = 0;

	private:
		virtual void * map(BufferAccess ba) = 0;
		virtual void unmap() = 0;

	protected:
		BufferUsage mUsage;
		uint32_t mAccessHint;
		uint32_t mSizeInByte;

	};



}


#endif