#ifndef _Air_Texture_H_
#define _Air_Texture_H_
#include "basic/include/ArrayRef.hpp"
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
		enum CubeFaces
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
		virtual void createHWResource(ArrayRef<ElementInitData> init_data) = 0;

	protected:
		uint32_t		mNumMipMaps;
		uint32_t		mArraySize;

		ElementFormat	mFormat;
		TextureType		mType;
		uint32_t		mSampleCount, mSampleQuality;
		uint32_t		mAccessHint;
	};

	AIR_CORE_API void getImageInfo(std::string const & tex_name, Texture::TextureType& type, uint32_t& width, uint32_t& height, uint32_t & depth, uint32_t& num_mipmaps, uint32_t& array_size, ElementFormat& format, uint32_t& row_pitch, uint32_t& slice_pitch);

	AIR_CORE_API void getImageInfo(ResIdentifierPtr const & tex_res, Texture::TextureType& type,
		uint32_t& width, uint32_t& height, uint32_t& depth, uint32_t& num_mipmaps, uint32_t& array_size,
		ElementFormat& format, uint32_t& row_pitch, uint32_t& slice_pitch);


	AIR_CORE_API void loadTexture(std::string const & tex_name, Texture::TextureType& type,
		uint32_t& width, uint32_t& height, uint32_t& depth, uint32_t& num_mipmaps, uint32_t& array_size,
		ElementFormat& format, std::vector<ElementInitData>& init_data, std::vector<uint8_t>& data_block);
	AIR_CORE_API void loadTexture(ResIdentifierPtr const & tex_res, Texture::TextureType& type,
		uint32_t& width, uint32_t& height, uint32_t& depth, uint32_t& num_mipmaps, uint32_t& array_size,
		ElementFormat& format, std::vector<ElementInitData>& init_data, std::vector<uint8_t>& data_block);
	AIR_CORE_API TexturePtr syncLoadTexture(std::string const & tex_name, uint32_t access_hint);
	AIR_CORE_API TexturePtr aSyncLoadTexture(std::string const & tex_name, uint32_t access_hint);

// 	AIR_CORE_API void saveTexture(std::string const & tex_name, Texture::TextureType type,
// 		uint32_t width, uint32_t height, uint32_t depth, uint32_t num_mipmaps, uint32_t array_size,
// 		ElementFormat format, ArrayRef<ElementInitData> init_data);
	AIR_CORE_API void saveTexture(TexturePtr const & texture, std::string const & tex_name);

	AIR_CORE_API void resizeTexture(void* dst_data, uint32_t dst_row_pitch, uint32_t dst_slice_pitch, ElementFormat dst_format,
		uint32_t dst_width, uint32_t dst_height, uint32_t dst_depth,
		void const * src_data, uint32_t src_row_pitch, uint32_t src_slice_pitch, ElementFormat src_format,
		uint32_t src_width, uint32_t src_height, uint32_t src_depth,
		bool linear);

	// return the lookat and up vector in cubemap view
	//////////////////////////////////////////////////////////////////////////////////
// 	template <typename T>
// 	std::pair<Vector_T<T, 3>, Vector_T<T, 3>> CubeMapViewVector(Texture::CubeFaces face);
}


#endif