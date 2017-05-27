#include "core/include/PreDeclare.h"
namespace Air
{
	class AIR_CORE_API LZMACodec
	{
	public:
		LZMACodec();
		~LZMACodec();

		uint64_t encode(std::ostream& os, ResIdentifierPtr const & res, uint64_t len);
		uint64_t encode(std::ostream& os, void const * input, uint64_t len);

		void encode(std::vector<uint8_t>& output, ResIdentifierPtr const & res, uint64_t len);
		void encode(std::vector<uint8_t>& output, void const * input, uint64_t len);

		uint64_t decode(std::ostream& os, ResIdentifierPtr const & res, uint64_t len, uint64_t original_len);
		uint64_t decode(std::ostream& os, void const * input, uint64_t len, uint64_t original_len);
		void decode(std::vector<uint8_t>& output, ResIdentifierPtr const & res, uint64_t len, uint64_t original_len);
		void decode(std::vector<uint8_t>& output, void const * input, uint64_t len, uint64_t original_len);
		void decode(void* output, void const * input, uint64_t len, uint64_t original_len);
	};


}