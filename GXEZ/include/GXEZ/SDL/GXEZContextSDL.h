#pragma once

// GXEZ
#include "GXEZ/IGXEZContext.h"
#include "GXEZ/Graphic/SDL/WindowSDL.h" 
#include "GXEZ/Graphic/SDL/Drawer2DSDL.h"
#include "GXEZ/Event/SDL/EventHandlerSDL.h"

// STD
#include <unordered_map>
#include <cstring>
#include <cstdlib>
#include <optional>
#include <iostream> // TMP ?

namespace GXEZ
{
    // Might Inherit From GXEZContextGLFW  one day ...
    class GXEZContextSDL : public IGXEZContext
    {
    public:

        static GXEZContextSDL* Get() noexcept { // pour obtenir le singleton
            if (_singleton == NULL)
            {
                _singleton = new GXEZContextSDL();
            }
            return (_singleton);
        }

        // Basics
        virtual void            Release() override;
        // Windows
        virtual IWindow*        CreateWindow(int width, int height, std::string name = "default") override;
        virtual void            ReleaseWindow(IWindow* window) override;
        // Events
        virtual IEventHandler*  CreateEventHandler() override;
        virtual void            ReleaseEventHandler(IEventHandler* handler) override;
        // Drawer
        virtual IDrawer2D*      CreateDrawer2D() override;
        virtual void            ReleaseDrawer2D(IDrawer2D* drawer) override;

    private:

        GXEZContextSDL()
        {
            Init();
        }

        // Basics
        virtual void Init() override;

     
        // Config //
        std::unordered_map<std::string, std::string>    _config;

        // Windows //
        std::unordered_map<size_t, WindowSDL*>          _windows;

        // Event Handlers //
        std::unordered_map<size_t, IEventHandler*>      _eventHandlers;

        // Drawer (only one to avoid duplicating texture, image, fonts, ....)
        Drawer2DSDL*                                    _drawer;


        // THIS - Singleton Instance
        static GXEZContextSDL* _singleton;
    };
}