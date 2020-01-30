#include "WindowGLFWVulkan.h"

#include <iostream>
namespace GXEZ
{

WindowGLFWVulkan::WindowGLFWVulkan()
{
	InitBasics();
	Open(500, 500);
}

WindowGLFWVulkan::WindowGLFWVulkan(unsigned int width, unsigned int height, const std::string & name)
{
	InitBasics();
	Open(width, height, name);
}

//WindowGLFWVulkan::WindowGLFWVulkan(IEventHandler * eHandler, unsigned int width, unsigned int height, const std::string & name)
//{
//	InitBasics();
//	Open(width, height, name);
//	SetEventHandler(eHandler);
//}

WindowGLFWVulkan::~WindowGLFWVulkan()
{
	//BGContext//SDL::Get().NotifyDeletedItem();
}

void WindowGLFWVulkan::InitBasics()
{
	
	// BGContext//SDL::Get().NotifyCreatedItem();
	// _linkedEventHandler = NULL;
	_window = NULL;
	_open = false;
	_id = 0;
	_showCursor = true;
	_borderless = false;
}

//void WindowGLFWVulkan::SetEventHandler(IEventHandler * eHandler)
//{
//	// Remove From Old IeventHandler if present one
//	// Then Set New One
//	_linkedEventHandler = eHandler;
//	// And Assign Event
//	Event	event;
//
//	event.idDevice = GetID();
//	event.type = Event::Type::WINDOW;
//	
//	_linkedEventHandler->SetPriority(IEventHandler::Priority::PERMANENT);
//
//	event.key = ControlKey::WINDOW_CLOSE;
//	_linkedEventHandler->AddHandlerToEvent(event, &WindowGLFWVulkan::EventClose, this);
//	event.key = ControlKey::WINDOW_RESIZE;
//	_linkedEventHandler->AddHandlerToEvent(event, &WindowGLFWVulkan::EventResize, this);
//	event.key = ControlKey::WINDOW_FOCUS;
//	_linkedEventHandler->AddHandlerToEvent(event, &WindowGLFWVulkan::EventFocus, this);
//	event.key = ControlKey::WINDOW_UNFOCUS;
//	_linkedEventHandler->AddHandlerToEvent(event, &WindowGLFWVulkan::EventUnFocus, this);
//
//	_linkedEventHandler->SetPriority(IEventHandler::Priority::MEDIUM);
//}
//
//bool WindowGLFWVulkan::Open(IEventHandler * eHandler, unsigned int width, unsigned int height, const std::string & name)
//{
//	if (Open(width, height, name))
//	{
//		SetEventHandler(eHandler);
//		return (true);
//	}
//	return (false);
//}

bool WindowGLFWVulkan::Open(unsigned int width, unsigned int height, const std::string & name)
{
	if (!_open)
	{
		_width = width;
		_height = height;
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		_window = glfwCreateWindow((int)width, (int)height, name.c_str(), nullptr, NULL);

		//_window = //SDL_CreateWindow(name.c_str(), //SDL_WINDOWPOS_CENTERED, //SDL_WINDOWPOS_CENTERED,
			// width, height,  /* //SDL_WINDOW_FULLSCREEN | */ /* //SDL_WINDOW_BORDERLESS */ //SDL_WINDOW_RESIZABLE);
		if (_window == NULL)
		{
			const char* error;
			glfwGetError(&error);
			std::cerr << "GLFW: Unable to create window: " << error << std::endl;
			return (false);
		}
		_open = true;
	}
}

bool WindowGLFWVulkan::IsOpen() const
{
	return (_open);
}

void WindowGLFWVulkan::Close()
{
	if (_window)
	{
		//SDL_DestroyWindow(_window);
		_window = NULL;
		_open = false;
	}
}

bool WindowGLFWVulkan::IsFocus() const
{
	return (_focus);
}

void WindowGLFWVulkan::Focus()
{
	if (_window)
	{
		//SDL_RaiseWindow(_window);
	}
}

void WindowGLFWVulkan::Resize(const unsigned int & width, const unsigned int & height)
{
	if (_window)
	{
		_width = width;
		_height = height;
		//SDL_SetWindowSize(_window, width, height);
	}
}

void WindowGLFWVulkan::SetPosition(const int & x, const int & y)
{
	if (_window)
	{
		//SDL_SetWindowPosition(_window, x, y);
	}
}

void WindowGLFWVulkan::Refresh()
{
	if (_window)
	{
		//SDL_UpdateWindowSurface(_window);
	}
}

const unsigned int & WindowGLFWVulkan::GetWidth() const
{
	return (_width);
}

const unsigned int & WindowGLFWVulkan::GetHeight() const
{
	return (_height);
}

//IImage * WindowGLFWVulkan::GetBackBuffer() const
//{
//	return (_buffer);
//}
//
//const ColorFormat& WindowGLFWVulkan::GetColorFormat() const
//{
//	return (_format);
//}

const uint32_t & WindowGLFWVulkan::GetID() const
{
	return (_id);
}

void WindowGLFWVulkan::SetBorderless(bool state)
{
	_borderless = state;
	if (_window) {
		//SDL_SetWindowBordered(_window, (//SDL_bool)!_borderless);
	}
}

void WindowGLFWVulkan::CursorHide(bool state)
{
	_showCursor = !state;
}

void WindowGLFWVulkan::CursorToCenter()
{
	//SDL_WarpMouseInWindow(_window, (int)((float)this->_width / 2.0f), (int)((float)this->_height / 2.0f));
}

 }