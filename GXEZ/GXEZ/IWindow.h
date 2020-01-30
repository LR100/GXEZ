#ifndef _IWINDOW_H_
#define _IWINDOW_H_

#include <stdint.h>
#include <string>

#include "Event/IEventHandler.h" // To link windows events to an eventhandler

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
		virtual bool				IsFocus() const = 0;
		virtual void				Focus() = 0;

		// Screen
		virtual void				Resize(const unsigned int& width, const unsigned int& height) = 0;
		virtual void				SetPosition(const int& x, const int& y) = 0;
		virtual void				Refresh() = 0;

		virtual const unsigned int& GetWidth() const = 0;
		virtual const unsigned int& GetHeight() const = 0;

		//virtual IImage*				GetBackBuffer() const = 0;
		//virtual const ColorFormat&	GetColorFormat() const = 0;
		virtual const uint32_t&		GetID() const = 0;

		// Property
		virtual void				SetBorderless(bool state) = 0;

		// Mouse
		virtual void				CursorHide(bool state) = 0;
		virtual void				CursorToCenter() = 0;

		// Event Handler
		virtual void				LinkEventHandler(IEventHandler* eventHandler) = 0;
	};
}

#endif /* !_IGRAPHIC2D_H_ */