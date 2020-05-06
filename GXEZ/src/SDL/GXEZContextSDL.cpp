#include "GXEZ/SDL/GXEZContextSDL.h"

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

		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
		TTF_Init();

		_drawer = NULL;
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

	IDrawer2D* GXEZContextSDL::CreateDrawer2D()
	{
		if (_drawer == NULL)
		{
			_drawer = new Drawer2DSDL(this, ColorFactory::Get().GetFormat());
			std::cout << "GXEZContextSDL::CreateDrawer2D()" << std::endl;
		}
		return (_drawer);
	}

	void GXEZContextSDL::ReleaseDrawer2D(IDrawer2D* drawer)
	{

	}

}

