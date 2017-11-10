#include "Context.h"
#include "basic/include/Util.h"
#include "input_system/include/InputFactory.hpp"

#include "MInput.hpp"
#include "MInputFactory.hpp"


namespace Air
{
	class MInputFactory : public InputFactory
	{
	public:
		std::wstring const & getName() const
		{
			static std::wstring const name(L"MsgInput Input Factory");
			return name;
		}
	private:

		virtual std::unique_ptr<InputEngine> doMakeInputEngine() override
		{
			return MakeUniquePtr<MsgInputEngine>();
		}

		virtual void doSuspend() override
		{

		}

		virtual void doResume() override
		{

		}
	};
}

void makeInputFactory(std::unique_ptr<Air::InputFactory>& ptr)
{
	ptr = Air::MakeUniquePtr<Air::MInputFactory>();
}
