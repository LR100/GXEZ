#pragma once

#include "GXEZ/GXEZDefinitions.h"

#if GXEZ_GLFW_VULKAN

#include "../IWindow.h"
#include <stdint.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace GXEZ
{
	class GXEZContextGLFWVulkan;

	class WindowGLFWVulkan : public IWindow
	{
	public:

		WindowGLFWVulkan(GXEZContextGLFWVulkan* context);
		WindowGLFWVulkan(GXEZContextGLFWVulkan* context, unsigned int width, unsigned int height, const std::string& name = "Default Window Name");
		WindowGLFWVulkan(GXEZContextGLFWVulkan* context, IEventHandler* eHandler, unsigned int width, unsigned int height, const std::string& name = "Default Window Name");

		~WindowGLFWVulkan();


		/// H�rit� via IWindow
		// Basics
		//bool						Open(IEventHandler* eHandler, unsigned int width, unsigned int height, const std::string& name = "Default Window Name");
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

		/*
		virtual IImage* GetBackBuffer() const override;
		virtual const ColorFormat& GetColorFormat() const override;
		*/
		virtual const uint32_t&		GetID() const override;

		// Property
		virtual void				SetBorderless(bool state) override;
		virtual bool				IsBorderless() const override;
		virtual void				SetFullScreen(bool state) override;
		virtual bool				IsFullScreen() const override;

		// Mouse
		virtual void				CursorHide(bool state) override;
		virtual void				CursorToCenter() override;

		// Events
		virtual void				LinkEventHandler(IEventHandler* eventHandler) override;

		// Drawer
		virtual void				LinkImageDrawer2D(IRenderer* renderer) override;


		//////////
		// GLFW //
		//////////

		GLFWwindow*					GLFWGetWindow() const;
		
		////////////
		// Vulkan //
		////////////

		///// ------

	protected:

		// Init For Construction
		void						InitBasics(GXEZContextGLFWVulkan* context);

		bool			_open;
		bool			_showCursor;
		bool			_focus;
		bool			_fullscreen;
		bool			_borderless;

		int				_refreshRate;
		unsigned int	_width, _height;
		int				_posX, _posY;

		uint32_t		_id;

		IEventHandler*	_eventHandler;

		// GLFW part
		GLFWwindow*		_window;
		// Vulkan part

		// The CONTEXT //
		GXEZContextGLFWVulkan*	_context;
	};

}

#endif 