#ifndef IEVENTHANDLER_H_
#define IEVENTHANDLER_H_

#include "IEvent.h"

#include <map>
#include <unordered_map>
#include <vector>
#include <future>
#include <functional>
#include <iostream>

// The Event Handler is capable of handling all type of Events:
// - keyboard (up/down..)
// - mouse (motion, scroll, click, ..)
// - or for exemple : window events (resize, focus, close)   (btw for window its possible to link an eventhandler to communicate with them)

// Events have an idDevice so is it possible to handle multiples devices of the same type

namespace GXEZ
{
	class IEventHandler
	{
	public:

		enum Priority
		{
			PERMANENT = 0, // Handlers For This Event REMAIN in the EventHandler even the method clear is called !
			HIGH = 1,
			MEDIUM = 2,
			LOW
		};

		IEventHandler();
		virtual ~IEventHandler() {};


		// Virtual
		virtual void	UpdateEvents() = 0;
		virtual void	TriggerEvent(ControlKey key);
		virtual void	TriggerEvent(ControlKey key, ControlKeyState state);
		virtual void	TriggerEvent(Event::Type type, ControlKey key, ControlKeyState state);
		virtual void	TriggerEvent(const Event& event);

		// Tools
		void			SetPriority(Priority priority);
		const Mouse*	GetMouse() const;

		// Handler Add
		template <class Function, class ...Args>
		void AddHandlerToEvent(Event::Definition def, Function&& f, Args&& ...args)
		{
			Event event(def);
			PushWork(event, f, args...);
		}

		template <class Function, class ...Args>
		void AddHandlerToEvent(ControlKey key, Function&& f, Args&& ...args)
		{
			Event event(Event::GetTypeFromControlKey(key), key, Event::GetDefaultControlKeyStateFromControlKey(key));
			PushWork(event, f, args...);
		}

		template <class Function, class ...Args>
		void AddHandlerToEvent(ControlKey key, ControlKeyState state, Function&& f, Args&& ...args)
		{
			Event event(Event::GetTypeFromControlKey(key), key, state);
			PushWork(event, f, args...);
		}

		template <class Function, class ...Args>
		void AddHandlerToEvent(Event::Type type, ControlKey key, ControlKeyState state, Function&& f, Args&& ...args)
		{
			Event event(type, key, state);
			PushWork(event, f, args...);
		}

		template <class Function, class ...Args>
		void AddHandlerToEvent(const Event& event, Function&& f, Args&& ...args)
		{
			PushWork(event, f, args...);
		}

		void AddHandlerToEvent(const Event& event, std::function<void()> function)
		{
			PushWorkFct(event, function);
		}
		// Remove All Handlers (except PERMANENT)
		void			Clear();


	protected:


		void			HandleEvent(EVENT_TYPE_HASH_ idevent);

		Mouse									_mouse;

	private:

		template <class Function, class ...Args>
		void PushWork(const Event& evt, Function&& f, Args&& ...args) {
			PushWorkFct(evt, std::bind(std::forward<Function>(f), std::forward<Args>(args)...));
		}

		void PushWorkFct(const Event& event, std::function<void()> function)
		{
			std::cout << "Add Event: " << event.ToString();
			EVENT_TYPE_HASH_ hash = event.GetHash();
			_handlers[_curPriority][hash]._works.push_back(Work(function, _curPriority));
	
		}

		struct Work
		{
			Work(std::function<void()> f, Priority priority);
			Priority							_priority;
			std::function<void()>				_function;
		};

		struct WorkList
		{
			std::vector<Work>	_works;
		};


		Priority																		_curPriority;
		std::map<Priority, std::unordered_map<EVENT_TYPE_HASH_, WorkList>>				_handlers;
		std::map<Priority, std::unordered_map<EVENT_TYPE_HASH_, WorkList>>::iterator	_handlersIt, _handlersItEnd;
	};

}

#endif // IEVENTHANDLER
