#include "Engine.h"

#include "rendersystem/include/Texture.hpp"

namespace Air
{

	Texture::Texture(TextureType type, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint)
		:mType(type), mSampleCount(sample_count), mSampleQuality(sample_quality), mAccessHint(access_hint)
	{

	}

	Texture::~Texture()
	{

	}

	uint32_t Texture::getNumMipMaps() const
	{
		return mNumMipMaps;
	}
	uint32_t Texture::getArraySize() const
	{
		return mArraySize;
	}
	ElementFormat Texture::getFormat() const
	{
		return mFormat;
	}
	Texture::TextureType Texture::getType() const
	{
		return mType;
	}
	uint32_t Texture::getSampleCount() const
	{
		return mSampleCount;
	}
	uint32_t Texture::getSampleQuality() const
	{
		return mSampleQuality;
	}

	uint32_t Texture::getAccesshint() const
	{
		return mAccessHint;
	}
}