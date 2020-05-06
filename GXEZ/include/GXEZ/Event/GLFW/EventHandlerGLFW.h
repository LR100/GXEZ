#pragma once

// GXEZ
#include "GXEZ/GXEZDefinitions.h"
#include "GXEZ/Event/IEventHandler.h"
#include "GXEZ/Event/IEvent.h"
// STD 
#include <unordered_map>
#include <stdint.h>

#if GXEZ_GLFW_VULKAN

// GLFW
#include <GLFW/glfw3.h>

namespace GXEZ
{

	class EventHandlerGLFW : public IEventHandler
	{
	public:
		EventHandlerGLFW();
		~EventHandlerGLFW();

		virtual void	UpdateEvents() override;

		// GLFW Event
		Event			GetEventFromGLFWEventKey(int key, int scancode, int action, int mods);
		// GLFW	Callback
		void			TriggerGLFWEventKey(int key, int scancode, int action, int mods);
		void			TriggerGLFWEventMouseMotion(double x, double y);

		// GLFW Window Link
		void			LinkWindow(GLFWwindow* window);

	private:

		/*void			HandleTypeQuit(const SDL_Event& event)
		void			HandleTypeKeyDown(const SDL_Event& event);
		void			HandleTypeKeyUp(const SDL_Event& event);
		void			HandleTypeMouseWheel(const SDL_Event& event);
		void			HandleTypeMouseButtonDown(const SDL_Event& event);
		void			HandleTypeMouseButtonUp(const SDL_Event& event);

		void			HandleTypeWindow(const SDL_Event& event);*/

		//typedef			void(EventHandlerGLFW::*HandleTypeFunction)(const SDL_Event&);

		std::unordered_map<ControlKey, Event>			_pressedRetainer;
		std::unordered_map<uint8_t, ControlKeyState>	_keyStateLink;
		std::unordered_map<uint16_t, ControlKey>		_keysLink;
		std::unordered_map<uint8_t, ControlKey>			_mouseLink;
		std::unordered_map<uint16_t, ControlKey>		_windowLink;
	};

}

#endif // GXEZ_GLFW_VULKAN