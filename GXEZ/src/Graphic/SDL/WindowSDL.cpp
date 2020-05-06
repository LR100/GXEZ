// GXEZ
#include "GXEZ/Graphic/SDL/WindowSDL.h"
#include "GXEZ/SDL/GXEZContextSDL.h"
#include "GXEZ/Graphic/SDL/Drawer2DSDL.h"


// STD
#include <iostream>

namespace GXEZ
{
	WindowSDL::WindowSDL(GXEZContextSDL* context)
	{
		InitBasics();
		Open(500, 500);
	}

	WindowSDL::WindowSDL(GXEZContextSDL* context, unsigned int width, unsigned int height, const std::string& name)
	{
		InitBasics();
		Open(width, height, name);
	}

	WindowSDL::WindowSDL(GXEZContextSDL* context, IEventHandler* eHandler, unsigned int width, unsigned int height, const std::string& name)
	{
		InitBasics();
		Open(width, height, name);
		LinkEventHandler(eHandler);
	}

	WindowSDL::~WindowSDL()
	{
	}

	void WindowSDL::InitBasics()
	{
		_linkedEventHandler = NULL;
		_window = NULL;
		_open = false;
		_id = 0;
		_showCursor = true;
		_borderless = false;
	}

	bool WindowSDL::Open(IEventHandler* eHandler, unsigned int width, unsigned int height, const std::string& name)
	{
		if (Open(width, height, name))
		{
			LinkEventHandler(eHandler);
			return (true);
		}
		return (false);
	}

	bool WindowSDL::Open(unsigned int width, unsigned int height, const std::string& name)
	{
		if (!_open)
		{
			_width = width;
			_height = height;
			_window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				width, height,  /* SDL_WINDOW_FULLSCREEN | */ /* SDL_WINDOW_BORDERLESS */ SDL_WINDOW_RESIZABLE);
			if (this->_window == NULL)
			{
				std::cerr << "SDL: Unable to create window: " << SDL_GetError() << std::endl;
				return (false);
			}

			SDL_UpdateWindowSurface(_window);
			_screen = SDL_GetWindowSurface(_window);

			SDL_DisplayMode modeScreen;
			SDL_GetCurrentDisplayMode(0, &modeScreen);

			SDL_Log("WINDOW OPEN SDL_GetDisplayMode(0, 0, &mode):\t\t%i bpp\t%i x %i", SDL_BITSPERPIXEL(modeScreen.format), modeScreen.w, modeScreen.h);
			ColorFormat format = ImageSDL::GetFormatBySDLFormat(_screen->format);
			std::cout << "WINDOW OPEN: Default Image Format (" << ColorFormatToString(format) << ")" << std::endl;
			ColorFactory::Get().SetFormat(format);
			std::cout << "COLOR FACTORY FORMAT : (" << ColorFormatToString(ColorFactory::Get().GetFormat()) << ")" << std::endl;
			//Uint32 f =  SDL_GetWindowPixelFormat(_window);
			//format = ImageSDL::GetFormatBySDLFormat(f);
			//std::cout << "WINDOW REAL PIXEL FORMAT (" << IImage::FormatToString(format) << ")" << std::endl;

			_buffer = new ImageSDL();
			_buffer->InitFromSurface(_screen);
			_open = true;
			_id = SDL_GetWindowID(_window);

			SDL_SetWindowBordered(_window, (SDL_bool)!_borderless);
		}
	}

	bool WindowSDL::IsOpen() const
	{
		return (_open);
	}

	void WindowSDL::Close()
	{
		if (_window)
		{
			SDL_DestroyWindow(_window);
			_window = NULL;
			_open = false;
			if (_buffer)
			{
				_screen = NULL;
				delete (_buffer);
				_buffer = NULL;
			}
		}
	}

	bool WindowSDL::IsFocus() const
	{
		return (_focus);
	}

	void WindowSDL::Focus()
	{
		if (_window)
		{
			SDL_RaiseWindow(_window);
		}
	}

	void WindowSDL::Resize(const unsigned int& width, const unsigned int& height)
	{
		if (_window)
		{
			_width = width;
			_height = height;
			SDL_SetWindowSize(_window, width, height);
		}
	}

	void WindowSDL::SetPosition(const int& x, const int& y)
	{
		if (_window)
			SDL_SetWindowPosition(_window, x, y);
	}

	void WindowSDL::Refresh()
	{
		if (_window)
			SDL_UpdateWindowSurface(_window);
	}

	const unsigned int& WindowSDL::GetWidth() const
	{
		return (_width);
	}

	const unsigned int& WindowSDL::GetHeight() const
	{
		return (_height);
	}

	IImage* WindowSDL::GetBackBuffer() const
	{
		return (_buffer);
	}

	const ColorFormat& WindowSDL::GetColorFormat() const
	{
		return (_format);
	}

	const uint32_t& WindowSDL::GetID() const
	{
		return (_id);
	}

	void WindowSDL::SetBorderless(bool state)
	{
		_borderless = state;
		if (_window)
			SDL_SetWindowBordered(_window, (SDL_bool)!_borderless);
	}

	bool WindowSDL::IsBorderless() const
	{
		return false;
	}

	void WindowSDL::SetFullScreen(bool state)
	{
	}

	bool WindowSDL::IsFullScreen() const
	{
		return false;
	}

	void WindowSDL::CursorHide(bool state)
	{
		_showCursor = !state;
	}

	void WindowSDL::CursorToCenter()
	{
		SDL_WarpMouseInWindow(_window, (int)((float)this->_width / 2.0f), (int)((float)this->_height / 2.0f));
	}

	void WindowSDL::LinkEventHandler(IEventHandler* eventHandler)
	{
		if (eventHandler == NULL) {
			return;
		}
		// Remove From Old IeventHandler if present one
		// Then Set New One
		_linkedEventHandler = eventHandler;
		// And Assign Event
		Event::Definition	eventDef;

		eventDef.idDevice = GetID();
		eventDef.type = Event::Type::WINDOW;

		_linkedEventHandler->SetPriority(IEventHandler::Priority::PERMANENT);

		eventDef.key = ControlKey::WINDOW_CLOSE;
		_linkedEventHandler->AddHandlerToEvent(eventDef, &WindowSDL::EventClose, this);
		eventDef.key = ControlKey::WINDOW_RESIZE;
		_linkedEventHandler->AddHandlerToEvent(eventDef, &WindowSDL::EventResize, this);
		eventDef.key = ControlKey::WINDOW_FOCUS;
		_linkedEventHandler->AddHandlerToEvent(eventDef, &WindowSDL::EventFocus, this);
		eventDef.key = ControlKey::WINDOW_UNFOCUS;
		_linkedEventHandler->AddHandlerToEvent(eventDef, &WindowSDL::EventUnFocus, this);

		_linkedEventHandler->SetPriority(IEventHandler::Priority::MEDIUM);
	}

	void WindowSDL::LinkDrawer2D(IDrawer2D* drawer)
	{
		// Only a drawer2D SDL is managed for the moment
		_drawer = dynamic_cast<Drawer2DSDL*>(drawer);
	}

	void WindowSDL::UseDrawer2D()
	{
		if (_drawer) {
			_drawer->SetCurrentImage(GetBackBuffer());
		}
	}

	void WindowSDL::EventResize()
	{
		if (_window)
		{
			std::cout << "WindowSDL::EventResize" << std::endl;
			int w, h;
			SDL_GetWindowSize(_window, &w, &h);
			_width = (unsigned int)w;
			_height = (unsigned int)h;
			Refresh();
			_screen = SDL_GetWindowSurface(_window);
			_buffer->InitFromSurface(_screen);
		}
	}

	void WindowSDL::EventClose()
	{
		std::cout << "WindowSDL::EventClose" << std::endl;
		Close();
	}

	void WindowSDL::EventFocus()
	{
		_focus = true;
		SDL_ShowCursor(_showCursor); // Set Cursor Mode
	}

	void WindowSDL::EventUnFocus()
	{
		_focus = false;
		SDL_ShowCursor(true); // Set Cursor Mode
	}

}
