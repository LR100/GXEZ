#ifndef WINDOWSDL_H_
#define WINDOWSDL_H_

// STD
#include <stdlib.h>
#include <string>
#include <map>

// GXEZ
#include "GXEZ/GXEZDefinitions.h"
#include "GXEZ/Graphic/IWindow.h"
#include "GXEZ/Graphic/SDL/ImageSDL.h"
#include "GXEZ/Graphic/SDL/ImageDrawer2DSDL.h"

#if GXEZ_SDL

// SDL
#include <SDL.h>

namespace GXEZ
{
	// An IEventHandler Can be linked To Window SDL 
	// to interact with closing / Resizing event Automatically

	class GXEZContextSDL;

	class WindowSDL : public IWindow
	{
	public:

		WindowSDL(GXEZContextSDL* context);
		WindowSDL(GXEZContextSDL* context, unsigned int width, unsigned int height, const std::string& name = "Default Window Name");
		WindowSDL(GXEZContextSDL* context, IEventHandler* eHandler, unsigned int width, unsigned int height, const std::string& name = "Default Window Name");

		~WindowSDL();


		/// Hérité via IWindow
		// Basics
		bool						Open(IEventHandler* eHandler, unsigned int width, unsigned int height, const std::string& name = "Default Window Name");
		virtual bool				Open(unsigned int width, unsigned int height, const std::string& name = "Default Window Name") override;
		virtual bool				IsOpen() const override;
		virtual void				Close() override;
		virtual bool				IsFocus() const override;
		virtual void				Focus() override;
		// Screen
		virtual void				Resize(const unsigned int& width, const unsigned int& height) override;
		virtual void				SetPosition(const int& x, const int& y) override;
		virtual void				Refresh() override;

		virtual const unsigned int& GetWidth() const override;
		virtual const unsigned int& GetHeight() const override;
		virtual const uint32_t&		GetID() const override;

		// Property
		virtual void				SetBorderless(bool state) override;
		virtual bool				IsBorderless() const override;
		virtual void				SetFullScreen(bool state) override;
		virtual bool				IsFullScreen() const override;

		// Mouse
		virtual void				CursorHide(bool state) override;
		virtual void				CursorToCenter() override;

		// Event Handler
		virtual void				LinkEventHandler(IEventHandler* eventHandler) override;
		virtual IEventHandler*		GetEventHandler() override;
		// Drawer
		virtual void				LinkImageDrawer2D(IImageDrawer2D* drawer) override;
		virtual void				UseImageDrawer2D() override;
		virtual const ColorFormat&	GetColorFormat() const override;

		// SDL - Only! And deprecated
		ImageSDL*					GetSDLBackBuffer() const;
		SDL_Window*					GetSDLWindow() const;
		void						SetSDLRenderer(SDL_Renderer* renderer);
		SDL_Renderer*				GetSDLRenderer() const;

	protected:

		// Init For Construction
		void				InitBasics(GXEZContextSDL* context);

		// Event Handlers
		virtual void		LinkEventFunction(ControlKey evt, std::function<void()> function);

		void				EventResize();
		void				EventClose();
		void				EventFocus();
		void				EventUnFocus();

		bool			_open;
		bool			_showCursor;
		bool			_focus;
		bool			_borderless;
		bool			_fullscreen;

		unsigned int	_width, _height;

		
		// SDL
		SDL_Window*		_window;
		SDL_Rect		_rect;
		SDL_Surface*	_screen;
		SDL_Renderer*	_renderer;
		// GXEZ
		uint32_t		_id;
		ColorFormat		_format;
		ImageSDL*		_buffer;

		IEventHandler*	_eventHandler;

		ImageDrawer2DSDL*	_drawer;

		

		GXEZContextSDL* _context;
	};

}

#endif // GXEZ_SDL

#endif // !WINDOWSDL_H_
