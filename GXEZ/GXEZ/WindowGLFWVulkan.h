#pragma once

#include "IWindow.h"
#include <stdint.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace GXEZ
{
	class WindowGLFWVulkan : public IWindow
	{
	public:

		WindowGLFWVulkan();
		WindowGLFWVulkan(unsigned int width, unsigned int height, const std::string& name = "Default Window Name");
		//WindowGLFWVulkan(IEventHandler* eHandler, unsigned int width, unsigned int height, const std::string& name = "Default Window Name");

		~WindowGLFWVulkan();

		// EventHandler
		//void	SetEventHandler(IEventHandler* eHandler);

		/// Hérité via IWindow
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

		/*virtual IImage* GetBackBuffer() const override;
		virtual const ColorFormat& GetColorFormat() const override;
		*/
		virtual const uint32_t&		GetID() const override;

		// Property
		virtual void				SetBorderless(bool state) override;

		// Mouse
		virtual void				CursorHide(bool state) override;
		virtual void				CursorToCenter() override;

	protected:

		// Init For Construction
		void						InitBasics();

		bool			_open;
		bool			_showCursor;
		bool			_focus;
		bool			_borderless;

		unsigned int	_width, _height;
		uint32_t		_id;

		// GLFW part
		GLFWwindow*		_window;

	};

}
