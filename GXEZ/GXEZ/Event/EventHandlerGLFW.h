#pragma once

#include "IEventHandler.h"
#include "IEvent.h"
// STD 
#include <unordered_map>
#include <stdint.h>

namespace GXEZ
{

class EventHandlerGLFW : public IEventHandler
{
public:
	EventHandlerGLFW();
	~EventHandlerGLFW();

	virtual void	UpdateEvents() override;

private:



	/*void			HandleTypeQuit(const SDL_Event& event);
	void			HandleTypeKeyDown(const SDL_Event& event);
	void			HandleTypeKeyUp(const SDL_Event& event);
	void			HandleTypeMouseWheel(const SDL_Event& event);
	void			HandleTypeMouseButtonDown(const SDL_Event& event);
	void			HandleTypeMouseButtonUp(const SDL_Event& event);
	void			HandleTypeMouseMotion(const SDL_Event& event);
	void			HandleTypeWindow(const SDL_Event& event);*/

	//typedef			void(EventHandlerGLFW::*HandleTypeFunction)(const SDL_Event&);

	std::unordered_map<ControlKey, Event>			_pressedRetainer;
	//std::unordered_map<uint32_t, HandleTypeFunction>	_typeLink;
	std::unordered_map<uint16_t, ControlKey>		_keysLink;
	std::unordered_map<uint8_t, ControlKey>			_mouseLink;
	std::unordered_map<uint16_t, ControlKey>		_windowLink;
	Event											_evt;
};

}

