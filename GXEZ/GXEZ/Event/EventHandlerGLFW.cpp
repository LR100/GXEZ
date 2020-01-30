#include "EventHandlerGLFW.h"



namespace GXEZ
{
	EventHandlerGLFW::EventHandlerGLFW()
	{
		/// KEYS

		// Basics
		_keysLink[GLFW_KEY_ENTER] = ControlKey::KEY_RETURN;
		_keysLink[GLFW_KEY_ESCAPE] = ControlKey::KEY_ESCAPE;
		_keysLink[GLFW_KEY_RIGHT_SHIFT] = ControlKey::KEY_RSHIFT;
		_keysLink[GLFW_KEY_LEFT_SHIFT] = ControlKey::KEY_LSHIFT;
		_keysLink[GLFW_KEY_LEFT_CONTROL] = ControlKey::KEY_LCTRL;
		_keysLink[GLFW_KEY_RIGHT_CONTROL] = ControlKey::KEY_RCTRL;
		_keysLink[GLFW_KEY_SPACE] = ControlKey::KEY_SPACE;
		_keysLink[GLFW_KEY_TAB] = ControlKey::KEY_TAB;
		// Arrows
		_keysLink[GLFW_KEY_UP] = ControlKey::KEY_UP;
		_keysLink[GLFW_KEY_DOWN] = ControlKey::KEY_DOWN;
		_keysLink[GLFW_KEY_RIGHT] = ControlKey::KEY_RIGHT;
		_keysLink[GLFW_KEY_LEFT] = ControlKey::KEY_LEFT;
		// Chars
		_keysLink[GLFW_KEY_A] = ControlKey::KEY_A;
		_keysLink[GLFW_KEY_B] = ControlKey::KEY_B;
		_keysLink[GLFW_KEY_C] = ControlKey::KEY_C;
		_keysLink[GLFW_KEY_D] = ControlKey::KEY_D;
		_keysLink[GLFW_KEY_E] = ControlKey::KEY_E;
		_keysLink[GLFW_KEY_F] = ControlKey::KEY_F;
		_keysLink[GLFW_KEY_G] = ControlKey::KEY_G;
		_keysLink[GLFW_KEY_H] = ControlKey::KEY_H;
		_keysLink[GLFW_KEY_I] = ControlKey::KEY_I;
		_keysLink[GLFW_KEY_J] = ControlKey::KEY_J;
		_keysLink[GLFW_KEY_K] = ControlKey::KEY_K;
		_keysLink[GLFW_KEY_L] = ControlKey::KEY_L;
		_keysLink[GLFW_KEY_M] = ControlKey::KEY_M;
		_keysLink[GLFW_KEY_N] = ControlKey::KEY_N;
		_keysLink[GLFW_KEY_O] = ControlKey::KEY_O;
		_keysLink[GLFW_KEY_P] = ControlKey::KEY_P;
		_keysLink[GLFW_KEY_Q] = ControlKey::KEY_Q;
		_keysLink[GLFW_KEY_R] = ControlKey::KEY_R;
		_keysLink[GLFW_KEY_S] = ControlKey::KEY_S;
		_keysLink[GLFW_KEY_T] = ControlKey::KEY_T;
		_keysLink[GLFW_KEY_U] = ControlKey::KEY_U;
		_keysLink[GLFW_KEY_V] = ControlKey::KEY_V;
		_keysLink[GLFW_KEY_W] = ControlKey::KEY_W;
		_keysLink[GLFW_KEY_X] = ControlKey::KEY_X;
		_keysLink[GLFW_KEY_Y] = ControlKey::KEY_Y;
		_keysLink[GLFW_KEY_Z] = ControlKey::KEY_Z;
		// Nums
		_keysLink[GLFW_KEY_0] = ControlKey::KEY_NUM0;
		_keysLink[GLFW_KEY_1] = ControlKey::KEY_NUM1;
		_keysLink[GLFW_KEY_2] = ControlKey::KEY_NUM2;
		_keysLink[GLFW_KEY_3] = ControlKey::KEY_NUM3;
		_keysLink[GLFW_KEY_4] = ControlKey::KEY_NUM4;
		_keysLink[GLFW_KEY_5] = ControlKey::KEY_NUM5;
		_keysLink[GLFW_KEY_6] = ControlKey::KEY_NUM6;
		_keysLink[GLFW_KEY_7] = ControlKey::KEY_NUM7;
		_keysLink[GLFW_KEY_8] = ControlKey::KEY_NUM8;
		_keysLink[GLFW_KEY_9] = ControlKey::KEY_NUM9;

		// States //
		_keyStateLink[GLFW_PRESS] = ControlKeyState::PRESSED;
		_keyStateLink[GLFW_RELEASE] = ControlKeyState::RELEASED;

		/// MOUSE
		_mouseLink[GLFW_MOUSE_BUTTON_LEFT] = ControlKey::MOUSE_LEFT;
		_mouseLink[GLFW_MOUSE_BUTTON_RIGHT] = ControlKey::MOUSE_RIGHT;
		
		/// WINDOW
		// TODO:_windowLink[SDL_WINDOWEVENT_MOVED] = ControlKey::WINDOW_MOVE;
		// TODO:_windowLink[SDL_WINDOWEVENT_CLOSE] = ControlKey::WINDOW_CLOSE;
		_windowLink[GLFW_FOCUSED] = ControlKey::WINDOW_FOCUS;
		// TODO:_windowLink[SDL_WINDOWEVENT_FOCUS_LOST] = ControlKey::WINDOW_UNFOCUS;
		// TODO:_windowLink[SDL_WINDOWEVENT_RESIZED] = ControlKey::WINDOW_RESIZE;

		/// TYPE
		// TODO:_typeLink[SDL_QUIT] = &EventHandlerGLFW::HandleTypeQuit;
		//_typeLink[GLFW_PRESS] = &EventHandlerGLFW::HandleTypeKeyDown;
		//_typeLink[GLFW_RELEASE] = &EventHandlerGLFW::HandleTypeKeyUp;
		// TODO: _typeLink[SDL_MOUSEWHEEL] = &EventHandlerGLFW::HandleTypeMouseWheel;
		//_typeLink[GLFW_PRESS] = &EventHandlerGLFW::HandleTypeMouseButtonDown;
		//_typeLink[GLFW_RELEASE] = &EventHandlerGLFW::HandleTypeMouseButtonUp;
		// TODO:_typeLink[SDL_MOUSEMOTION] = &EventHandlerGLFW::HandleTypeMouseMotion;
		// TODO:_typeLink[SDL_WINDOWEVENT] = &EventHandlerGLFW::HandleTypeWindow;
	}

	EventHandlerGLFW::~EventHandlerGLFW()
	{
		// BGContextSDL::Get().NotifyDeletedItem(); // MUST NOW BE CREATED BY THE CONTEXT DIRECTLY
	}

#include <iostream>

	void EventHandlerGLFW::UpdateEvents()
	{
		glfwPollEvents();

		//SDL_Event	evt;
		//int			mouseX, mouseY;

		//glfwSetUserPointer(window_, this);
		//glfwSetKeyCallback(window_, key_callback_);

		//SDL_GetMouseState(&mouseX, &mouseY);
		//_mouse.x = (unsigned int)mouseX;
		//_mouse.y = (unsigned int)mouseY;

		//// Pressed Retainer
		//std::unordered_map<ControlKey, Event>::iterator itpr = _pressedRetainer.begin();
		//std::unordered_map<ControlKey, Event>::iterator itprEnd = _pressedRetainer.end();
		//while (itpr != itprEnd)
		//{
		//	//std::cout << "Function Hash(" << itpr->second.GetHash() << ")" << std::endl;
		//	HandleEvent(itpr->second.GetHash());
		//	itpr++;
		//}
		////std::cout << "UpdateEvents" << std::endl;;
		//while (SDL_PollEvent(&evt))
		//{
		//	_evt.key = ControlKey::KEY_NONE;
		//	_evt.type = Event::Type::NONE;
		//	_evt.state = ControlKeyState::NONE;
		//	_evt.idDevice = 0; // No set yet for keyboard and mouses
		//	if (_typeLink.count(evt.type))
		//		(this->*_typeLink.at(evt.type))(evt);
		//	//std::cout << "Event: " << _evt.ToString();

		//	HandleEvent(_evt.GetHash());
		//}
	}

	Event EventHandlerGLFW::GetEventFromGLFWEventKey(int key, int scancode, int action, int mods)
	{
		ControlKey keycode;
		if (_keysLink.count(key))
			keycode = _keysLink.at(key);
		ControlKeyState keystate;
		if (_keyStateLink.count(action))
			keystate = _keyStateLink.at(action);
		Event evt(Event::GetTypeFromControlKey(keycode), keycode, keystate);
		return (evt);
	}

	void EventHandlerGLFW::TriggerGLFWEventKey(int key, int scancode, int action, int mods)
	{
		TriggerEvent(GetEventFromGLFWEventKey(key, scancode, action, mods));
	}

	void EventHandlerGLFW::TriggerGLFWEventMouseMotion(double x, double y)
	{
		_mouse.x = x;
		_mouse.y = y;
		Event evt;
		evt.type = Event::MOUSE;
		evt.key = ControlKey::MOUSE_MOTION;
		evt.state = Event::GetDefaultControlKeyStateFromControlKey(evt.key);
		TriggerEvent(evt);
	}

	void EventHandlerGLFW::LinkWindow(GLFWwindow* window)
	{
		std::cout << "EventHandlerGLFW::LinkWindow" << std::endl;
		glfwSetWindowUserPointer(window, this);

		// PROBABLY COULD BE OPTIMIZED BY STORING DIRECTLY A FUNC BY EVENT 

		// Keys //
		auto funcKey = [](GLFWwindow* w, int key, int scancode, int action, int mods) {
			static_cast<EventHandlerGLFW*>(glfwGetWindowUserPointer(w))->TriggerGLFWEventKey(key, scancode, action, mods);
		};
		glfwSetKeyCallback(window, funcKey);

		// Mouse //

		// Position
		auto funcMousePos = [](GLFWwindow* w, double x, double y) {
			static_cast<EventHandlerGLFW*>(glfwGetWindowUserPointer(w))->TriggerGLFWEventMouseMotion(x, y);
		};
		glfwSetCursorPosCallback(window, funcMousePos);
	}

	//void EventHandlerGLFW::HandleTypeQuit(const SDL_Event& evt)
	//{
	//	/*_evt.type = Event::QUIT;*/
	//}

	//void EventHandlerGLFW::HandleTypeKeyDown(const SDL_Event& evt)
	//{
	//	/*if (_keysLink.count(evt.key.keysym.sym))
	//		_evt.key = _keysLink.at(evt.key.keysym.sym);
	//	_evt.type = Event::KEY;
	//	_evt.state = ControlKeyState::PRESSED;*/
	//}

	//void EventHandlerGLFW::HandleTypeKeyUp(const SDL_Event& evt)
	//{
	//	/*if (_keysLink.count(evt.key.keysym.sym))
	//		_evt.key = _keysLink.at(evt.key.keysym.sym);
	//	_evt.type = Event::KEY;
	//	_evt.state = ControlKeyState::RELEASED;*/
	//}

	//void EventHandlerGLFW::HandleTypeMouseWheel(const SDL_Event& evt)
	//{
	//	/*_evt.type = Event::MOUSE;
	//	_evt.key = ControlKey::MOUSE_SCROLL;
	//	_evt.state = Event::GetDefaultControlKeyStateFromControlKey(_evt.key);
	//	_mouse.xScroll = evt.wheel.x;
	//	_mouse.yScroll = evt.wheel.y;*/
	//}

	//void EventHandlerGLFW::HandleTypeMouseButtonDown(const SDL_Event& evt)
	//{
	//	//_evt.type = Event::MOUSE;
	//	//if (_mouseLink.count(evt.button.button))
	//	//	_evt.key = _mouseLink.at(evt.button.button);
	//	//_evt.state = ControlKeyState::PRESSED;
	//	//// Correct Specification of SDL Handling by Retaining mouse pressed
	//	////std::cout << "Mouse Down" << std::endl;
	//	//_pressedRetainer[_evt.key] = _evt;
	//}

	//void EventHandlerGLFW::HandleTypeMouseButtonUp(const SDL_Event& evt)
	//{
	//	//_evt.type = Event::MOUSE;
	//	//if (_mouseLink.count(evt.button.button))
	//	//	_evt.key = _mouseLink.at(evt.button.button);
	//	//// Release Mouse ControlKey Pressed
	//	//if (_pressedRetainer.count(_evt.key))
	//	//	_pressedRetainer.erase(_evt.key);
	//	////std::cout << "Mouse Up" << std::endl;
	//	//_evt.state = ControlKeyState::RELEASED;
	//}

	//void EventHandlerGLFW::HandleTypeMouseMotion(const SDL_Event& evt)
	//{
	//	/*_evt.type = Event::MOUSE;
	//	_evt.key = ControlKey::MOUSE_MOTION;
	//	_evt.state = Event::GetDefaultControlKeyStateFromControlKey(_evt.key);*/
	//}

	//void EventHandlerGLFW::HandleTypeWindow(const SDL_Event& evt)
	//{
	//	//_evt.type = Event::WINDOW;
	//	//_evt.idDevice = evt.window.windowID;
	//	//if (_windowLink.count(evt.window.evt))
	//	//{
	//	//	_evt.key = _windowLink.at(evt.window.evt);
	//	//	_evt.state = Event::GetDefaultControlKeyStateFromControlKey(_evt.key);
	//	//}
	//	//// !! HANDLE EVENT WITH ID DEVICE !!
	//	//HandleEvent(_evt.GetHash());
	//	//// Then Set It for Handling WITHOUT Id Device
	//	//// This Procedure allow the user of EventHandler 
	//	//// to handle any kind of Window Event whatever the window it is
	//	//_evt.idDevice = 0;
	//}

}
