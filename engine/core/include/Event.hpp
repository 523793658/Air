#ifndef _Engine_Event_H_
#define _Engine_Event_H_
#include "PreDeclare.h"
#include <functional>
#include <unordered_map>
#include "boost/signals2.hpp"
namespace Air
{
	struct EventConnection
	{
	public:
		EventConnection(boost::signals2::connection con)
			: mConnection(con)
		{

		}
		boost::signals2::connection mConnection;
	};

	class AIR_CORE_API EventTable
	{
	public:
		EventConnection registerEvent(std::string_view const & name, std::function<void(void*)>);

		EventConnection registerEvent(size_t nameId, std::function<void(void*)>);

		void unregisterEvent(std::string_view const & name, EventConnection);

		void unregisterEvent(size_t eventId, EventConnection);

		void notify(std::string const & name, void* e);

		void notify(size_t eventId, void* e);
	private:
		std::unordered_map < size_t, std::shared_ptr<boost::signals2::signal<void(void*)>>> mEventTable;
	};
}
#endif
