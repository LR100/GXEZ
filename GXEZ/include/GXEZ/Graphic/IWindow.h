#ifndef _IWINDOW_H_
#define _IWINDOW_H_

// STD
#include <stdint.h>
#include <string>
#include <future>
#include <functional>

// GXEZ
#include "GXEZ/Event/IEventHandler.h" // To link windows events to an eventhandler
#include "GXEZ/Graphic/IImageDrawer2D.h"

namespace GXEZ
{
	class IWindow
	{
	public:
		virtual ~IWindow() {};
		
		// Basics
		virtual bool				Open(unsigned int width, unsigned int height, const std::string & = "Default Window Name") = 0;
		virtual bool				IsOpen() const = 0;
		virtual void				Close() = 0;
		// Focus
		virtual bool				IsFocus() const = 0;
		virtual void				Focus() = 0;

		// Screen
		virtual void				Resize(const unsigned int& width, const unsigned int& height) = 0;
		virtual void				SetPosition(const int& x, const int& y) = 0;

		virtual void				Refresh() = 0;

		// Size
		virtual const unsigned int& GetWidth() const = 0;
		virtual const unsigned int& GetHeight() const = 0;

		//virtual IImage*				GetBackBuffer() const = 0;
		//virtual const ColorFormat&	GetColorFormat() const = 0;
		virtual const uint32_t&		GetID() const = 0;

		// Property
		virtual void				SetBorderless(bool state) = 0;
		virtual bool				IsBorderless() const = 0;
		virtual void				SetFullScreen(bool state) = 0;
		virtual bool				IsFullScreen() const = 0;

		// Mouse
		virtual void				CursorHide(bool state) = 0;
		virtual void				CursorToCenter() = 0;

		// Event Handler
		virtual void				LinkEventHandler(IEventHandler* eventHandler) = 0;
		virtual IEventHandler*		GetEventHandler() = 0;

		template <class Function, class ...Args>
		void						LinkEvent(ControlKey evt, Function&& f, Args&& ...args)
		{
			LinkEventFunction(evt, std::bind(std::forward<Function>(f), std::forward<Args>(args)...));
		}

		// Drawer
		virtual void				LinkImageDrawer2D(IImageDrawer2D* drawer) = 0;
		virtual void				UseImageDrawer2D() = 0;
		virtual const ColorFormat&	GetColorFormat() const = 0;

		///////////////
		// PROTECTED //

	protected:

		virtual void				LinkEventFunction(ControlKey evt, std::function<void()> function) = 0;
	};
}

#endif /* !_IGRAPHIC2D_H_ */