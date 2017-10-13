#ifndef _InputeFactory_H_
#define _InputeFactory_H_ 
#include <boost/utility.hpp>
#include "PreDeclare.h"
#include <string.h>
#include "input_system/include/Input.hpp"

namespace Air
{
	class AIR_CORE_API InputFactory : boost::noncopyable
	{
	public:
		virtual ~InputFactory()
		{

		}
		virtual std::wstring const & getName() const = 0;
		InputEngine& getInputEngineInstance();
		void suspend();
		void resume();
	private:
		virtual std::unique_ptr<InputEngine> doMakeInputEngine() = 0;

		virtual void doSuspend() = 0;
		virtual void doResume() = 0;

	protected:
		std::unique_ptr<InputEngine> mInputEngine;

	};
}


#endif
