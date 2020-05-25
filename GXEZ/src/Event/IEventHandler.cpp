#include "GXEZ/Event/IEventHandler.h"

namespace GXEZ
{
	IEventHandler::IEventHandler()
	{
		_curPriority = Priority::MEDIUM;
		_handlers[Priority::PERMANENT] = std::unordered_map<EVENT_TYPE_HASH_, WorkList>();
		_handlers[Priority::HIGH] = std::unordered_map<EVENT_TYPE_HASH_, WorkList>();
		_handlers[Priority::MEDIUM] = std::unordered_map<EVENT_TYPE_HASH_, WorkList>();
		_handlers[Priority::LOW] = std::unordered_map<EVENT_TYPE_HASH_, WorkList>();
	}

	void IEventHandler::TriggerEvent(ControlKey key)
	{
		Event event(Event::GetTypeFromControlKey(key), key, Event::GetDefaultControlKeyStateFromControlKey(key));
		TriggerEvent(event);
	}

	void IEventHandler::TriggerEvent(ControlKey key, ControlKeyState state)
	{
		Event event(Event::GetTypeFromControlKey(key), key, state);
		TriggerEvent(event);
	}

	void IEventHandler::TriggerEvent(Event::Type type, ControlKey key, ControlKeyState state)
	{
		Event event(type, key, state);
		TriggerEvent(event);
	}

	void IEventHandler::TriggerEvent(const Event& event)
	{
		HandleEvent(event.GetHash());
	}

	void IEventHandler::SetPriority(Priority priority)
	{
		_curPriority = priority;
	}

	void IEventHandler::Clear()
	{
		_handlersIt = _handlers.begin();
		_handlersIt++; // Clear Everything except PERMANENT
		_handlersItEnd = _handlers.end();
		for (; _handlersIt != _handlersItEnd; _handlersIt++)
			_handlersIt->second.clear();
	}

	const Mouse* IEventHandler::GetMouse() const
	{
		return (&_mouse);
	}

#include <iostream> // TMP

	void IEventHandler::HandleEvent(EVENT_TYPE_HASH_ idevent)
	{
		_handlersIt = _handlers.begin();
		_handlersItEnd = _handlers.end();
		// std::cout << "Handle Event Start (" << _handlersIt->first << ")" << std::endl;
		for (; _handlersIt != _handlersItEnd; _handlersIt++)
		{
			// std::cout << "Handle Event (" << _handlersIt->first << ")" << std::endl;
			if (_handlersIt->second.count(idevent))
			{
				// Do Work Permanent First
				WorkList* workList = &_handlersIt->second.at(idevent);
				// std::cout << "WorkList Size (" << workList->_works.size() << ")" << std::endl;
				for (size_t i = 0; i < workList->_works.size(); i += 1)
				{
					// std::cout << "HandleEvent (" << idevent << ")" << std::endl;
					workList->_works.at(i)._function();
				}
			}
		}
	}

	IEventHandler::Work::Work(std::function<void()> f, Priority priority) : _function(f), _priority(priority)
	{
	}

}