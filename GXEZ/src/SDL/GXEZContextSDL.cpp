#include "GXEZ/SDL/GXEZContextSDL.h"

// GXEZ
#include "GXEZ/Graphic/SDL/RendererSDL.h"

// STD //
#include <set>

// TXEZ //
#include "TXEZ/XMLParser.h" // To Load SDL Config 

// SDL
#include <SDL.h>
#include <SDL_ttf.h>

namespace GXEZ
{
	/////////////////////////
	// GRAPHIC Context SDL //
	//
	//

	GXEZContextSDL* GXEZContextSDL::_singleton = NULL;

	void GXEZContextSDL::Init()
	{
		std::cout << "GXEZContextSDL Init()" << std::endl;
		/////////////////////////////////
		// Load SDL.config file //
		// If are Already Running -> QUIT
		atexit(SDL_Quit);
		TTF_Quit();

		_drawer = NULL;

		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		{
			std::cerr << "GXEZContextSDL::Init() Couldn't initialize SDL: " << SDL_GetError() << std::endl;
		}
		if (TTF_Init() < 0)
		{
			std::cerr << "GXEZContextSDL::Init() Couldn't initialize SDL_TTF: " << TTF_GetError() << std::endl;
		}
		
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	}

	IGXEZContext::Type GXEZContextSDL::GetType() const
	{
		return (_type);
	}

	void GXEZContextSDL::Release()
	{
		std::cout << "GXEZContextSDL::Release()" << std::endl;
		// SDL
		SDL_Quit();
		TTF_Quit();
	}

	IWindow* GXEZContextSDL::CreateWindow(int width, int height, std::string name)
	{
		std::cout << "GXEZContextSDL::CreateWindow()" << std::endl;
		// Vulkan Instance is always init here //
		WindowSDL* windowSDL = new WindowSDL(this, width, height, name); // Create GLFW Window // GLFW is alway init here
		if (!windowSDL) {
			return (NULL);
		}
		_windows[size_t(windowSDL)] = windowSDL;

		return (windowSDL);
	}


	void GXEZContextSDL::ReleaseWindow(IWindow* window)
	{
		std::cout << "GXEZContextSDL::ReleaseWindow()" << std::endl;
		// If window id in id of glfw window release it
	}

	IEventHandler* GXEZContextSDL::CreateEventHandler()
	{
		std::cout << "GXEZContextSDL::CreateEventHandler()" << std::endl;
		return (new EventHandlerSDL());
	}

	void GXEZContextSDL::ReleaseEventHandler(IEventHandler* handler)
	{
	}

	IImageDrawer2D* GXEZContextSDL::CreateImageDrawer2D()
	{
		if (_drawer == NULL)
		{
			_drawer = new ImageDrawer2DSDL(this, ColorFactory::Get().GetFormat());
			std::cout << "GXEZContextSDL::CreateImageDrawer2D()" << std::endl;
		}
		return (_drawer);
	}

	void GXEZContextSDL::ReleaseImageDrawer2D(IImageDrawer2D* drawer)
	{

	}

	IRenderer* GXEZContextSDL::GetRenderer(IWindow* window)
	{
		WindowSDL* windowSDL = dynamic_cast<WindowSDL*>(window);

		if (windowSDL) {
			return (RendererSDL::Get(windowSDL));
		}
		return (NULL);
	}

}

