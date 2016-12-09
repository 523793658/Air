#ifndef _Air_Texture_H_
#define _Air_Texture_H_
namespace Air
{
	enum TextureMapAccess
	{
		TMA_Read_Only,
		TMA_Write_Only,
		TMA_Read_Write
	};

	class AIR_CORE_API Texture
	{
	public:
		enum TextureType
		{
			TT_1D,
			TT_2D,
			TT_3D,
			TT_Cube
		};
		enum CubeFace
		{
			CF_Positive_X = 0,
			CF_Negative_X = 1,
			CF_Positive_Y = 2,
			CF_Negative_Y = 3,
			CF_Positive_Z = 4,
			CF_Negative_Z = 5
		};
	public:
		Texture(TextureType type, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint);
		~Texture();


		uint32_t getNumMipMaps() const;
		uint32_t getArraySize() const;
		ElementFormat getFormat() const;
		TextureType getType() const;
		uint32_t getSampleCount() const;
		uint32_t getSampleQuality() const;
		uint32_t getAccesshint() const;


		//子类去实现，由于1d，2d，3d纹理的长、宽、深度定义不同
		virtual uint32_t getWidth(uint32_t level) const = 0;
		virtual uint32_t getHeight(uint32_t level) const = 0;
		virtual uint32_t getDepth(uint32_t level) const = 0;

	public:
		virtual void deleteHWResource() = 0;
		virtual bool isHWResourceReady() const = 0;
		virtual void createHWResource(ElementInitData const * init_data) = 0;

	protected:
		uint32_t		mNumMipMaps;
		uint32_t		mArraySize;

		ElementFormat	mFormat;
		TextureType		mType;
		uint32_t		mSampleCount, mSampleQuality;
		uint32_t		mAccessHint;
	};
}


#endif