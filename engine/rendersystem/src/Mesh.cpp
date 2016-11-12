#include "Engine.h"

#include "ResLoader.h"
#include "basic/include/Util.h"
#include "rendersystem/include/Mesh.hpp"

namespace
{
	using namespace Air;
	uint32_t const MODEL_BIN_VERSION = 1.0;
	class RenderModelLoadingDesc : public ResLoadingDesc
	{
	private:
		struct ModelDesc 
		{
			std::string mResName;
			uint32_t mAccessHint;
		};
	public:
		RenderModelLoadingDesc(std::string const &resName, uint32_t accessHint)
		{

		}
		uint64_t getType() const
		{
			static uint64_t const type = CT_HASH("RenderModelLoadingDesc");
			return type;
		}
		bool getStateLess() const
		{
			return false;
		}



		void subThreadStage()
		{

		}
		std::shared_ptr<void> mainThreadStage()
		{

		}

		bool hasSubThreadStage() const
		{

		}

		bool match(ResLoadingDesc const & rhs) const
		{

		}
		std::shared_ptr<void> cloneResourceFrom(std::shared_ptr<void> const& resource)
		{

		}

		std::shared_ptr<void> getResource() const
		{

		}
	};
}