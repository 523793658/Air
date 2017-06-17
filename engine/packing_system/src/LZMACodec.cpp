#include "Engine.h"
#include "basic/include/Thread.h"
#include "basic/include/ResIdentifier.h"
#include "packing_system/include/LZMACodec.h"
#include "../../external/lzma/c/LzmaLib.h"
namespace Air
{
	LZMACodec::LZMACodec()
	{

	}

	LZMACodec::~LZMACodec()
	{

	}

	uint64_t LZMACodec::encode(std::ostream& os, ResIdentifierPtr const & res, uint64_t len)
	{
		std::vector<uint8_t> input(static_cast<size_t>(len));
		res->read(&input[0], len);
		std::vector<uint8_t> output;
		this->encode(output, &input[0], len);
		os.write(reinterpret_cast<char*>(&output[0]), output.size()* sizeof(output[0]));
		return output.size();
	}


	uint64_t LZMACodec::encode(std::ostream& os, void const * input, uint64_t len)
	{
		std::vector<uint8_t> output;
		this->encode(output, input, len);
		os.write(reinterpret_cast<char*>(&output[0]), output.size());
		return output.size();
	}

	void LZMACodec::encode(std::vector<uint8_t>& output, ResIdentifierPtr const & res, uint64_t len)
	{
		std::vector<uint8_t> input(len);
		res->read(&input[0], static_cast<size_t>(len));
		this->encode(output, &input[0], len);
	}
	void LZMACodec::encode(std::vector<uint8_t>& output, void const * input, uint64_t len)
	{
		SizeT out_len = static_cast<SizeT>(std::max(len * 11 / 10, static_cast<uint64_t>(32)));
		output.resize(LZMA_PROPS_SIZE + out_len);
		SizeT out_props_size = LZMA_PROPS_SIZE;
		LzmaCompress(&output[LZMA_PROPS_SIZE], &out_len, static_cast<Byte const *>(input), static_cast<SizeT>(len),
			&output[0], &out_props_size, 5, std::min<uint32_t>(static_cast<uint32_t>(len), 1UL << 24), 3, 0, 2, 32, 1);
		output.resize(LZMA_PROPS_SIZE + out_len);
	}

	uint64_t LZMACodec::decode(std::ostream& os, ResIdentifierPtr const & res, uint64_t len, uint64_t original_len)
	{
		std::vector<uint8_t> in_data(static_cast<size_t>(len));
		res->read(&in_data[0], len);
		std::vector<uint8_t> output;
		this->decode(output, &in_data[0], len, original_len);
		os.write(reinterpret_cast<char*>(&output[0]), static_cast<std::streamsize>(output.size()));
		return output.size();
	}
	uint64_t LZMACodec::decode(std::ostream& os, void const * input, uint64_t len, uint64_t original_len)
	{
		std::vector<uint8_t> output(static_cast<uint32_t>(original_len));
		this->decode(&output[0], input, len, original_len);
		os.write(reinterpret_cast<char*>(&output[0]), original_len);
		return output.size();
	}
	void LZMACodec::decode(std::vector<uint8_t>& output, ResIdentifierPtr const & res, uint64_t len, uint64_t original_len)
	{
		std::vector<uint8_t> input_data(static_cast<uint32_t>(len));
		res->read(&input_data[0], static_cast<uint32_t>(len));
		this->decode(output, &input_data[0], len, original_len);

	}
	void LZMACodec::decode(std::vector<uint8_t>& output, void const * input, uint64_t len, uint64_t original_len)
	{
		output.resize(static_cast<uint32_t>(original_len));
		this->decode(&output[0], input, len, original_len);
	}
	void LZMACodec::decode(void* output, void const * input, uint64_t len, uint64_t original_len)
	{
		uint8_t const *p = static_cast<uint8_t const *>(input);
		std::vector<uint8_t> in_data(static_cast<size_t>(len));
		std::memcpy(&in_data[0], p, static_cast<std::streamsize>(in_data.size()));

		SizeT out_len = static_cast<SizeT>(original_len);
		SizeT src_len = static_cast<SizeT>(len - LZMA_PROPS_SIZE);

		LzmaUncompress(static_cast<Byte*>(output), &out_len, &in_data[LZMA_PROPS_SIZE], &src_len, &in_data[0], LZMA_PROPS_SIZE);
	}



	
}