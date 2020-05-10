#include "GXEZ\GXEZEnvironment.h"

// SDL
#if GXEZ_SDL
#include "GXEZ/SDL/GXEZContextSDL.h"
#endif // GXEZ_SDL

// GLFW - VULKAN
#if GXEZ_GLFW_VULKAN
#include "GXEZ/GLFW_Vulkan/GXEZContextGLFWVulkan.h"
#endif // GXEZ_GLFW_VULKAN

namespace GXEZ
{
	GXEZEnvironment GXEZEnvironment::_singleton(IGXEZContext::Type::GRAPHIC_CONTEXT_TYPE_SDL2_RENDERER);

	GXEZEnvironment::GXEZEnvironment(IGXEZContext::Type context_type)
	{
		Init(context_type);
	}

	void GXEZEnvironment::Init(IGXEZContext::Type context_type)
	{
		if (context_type == IGXEZContext::Type::GRAPHIC_CONTEXT_TYPE_SDL2_RENDERER || context_type == IGXEZContext::Type::GRAPHIC_CONTEXT_TYPE_SDL2_DRAWER) {
			// Load SDL2 Environment
#if GXEZ_SDL
			_context = (IGXEZContext*)GXEZContextSDL::Get(context_type);
#endif // GXEZ_SDL
		}
		else if (context_type == IGXEZContext::Type::GRAPHIC_CONTEXT_TYPE_GLFW_VULKAN_RENDERER) {
			// Load GLFW Vulkan Environement
#if GXEZ_GLFW_VULKAN
				//_context = (IGXEZContext*)GXEZContextGLFWVulkan::Get();
#endif // GXEZ_GLFW_VULKAN
		}
		else {
			_context = NULL;
		}
	}

	void GXEZEnvironment::Release()
	{
		if (_context) {
			_context->Release();
		}
	}

}
