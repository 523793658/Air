#include "Context.h"
#include "core/include/Event.hpp"
namespace Air
{
	EventConnection EventTable::registerEvent(std::string_view const & name, std::function<void(void*)> f)
	{
		return registerEvent(boost::hash_range(name.begin(), name.end()), f);
	}

	EventConnection EventTable::registerEvent(size_t nameId, std::function<void(void*)> f)
	{
		auto table = mEventTable.find(nameId);
		if (table == mEventTable.end())
		{
			mEventTable.emplace(nameId, MakeSharedPtr<boost::signals2::signal<void(void*)>>());
			table = mEventTable.find(nameId);
		}
		return EventConnection(table->second->connect(f));
	}

	void EventTable::unregisterEvent(std::string_view const & name, EventConnection con)
	{
		unregisterEvent(boost::hash_range(name.begin(), name.end()), con);
	}

	void  EventTable::unregisterEvent(size_t eventId, EventConnection con)
	{
		auto it = mEventTable.find(eventId);
		if (it != mEventTable.end())
		{
			con.mConnection.disconnect();
			if (it->second->empty())
			{
				mEventTable.erase(it);
			}
		}

	}

	void EventTable::notify(std::string const & name, void* e)
	{
		notify(boost::hash_range(name.begin(), name.end()), e);
	}

	void EventTable::notify(size_t eventId, void* e)
	{
		auto table = mEventTable.find(eventId);
		if (table != mEventTable.end())
		{
			(*(table->second))(e);
		}
	}

}