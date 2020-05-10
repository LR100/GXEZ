#pragma once

// GXEZ
#include "GXEZ/IGXEZContext.h"
#include "GXEZ/Graphic/SDL/WindowSDL.h" 
#include "GXEZ/Graphic/SDL/ImageDrawer2DSDL.h"
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

        static GXEZContextSDL* Get(GXEZ::IGXEZContext::Type type) noexcept { // pour obtenir le singleton
            if (_singleton == NULL)
            {
                _singleton = new GXEZContextSDL(type);
            }
            return (_singleton);
        }

        // Basics
        virtual Type            GetType() const override;
        virtual void            Release() override;
        // Windows
        virtual IWindow*        CreateWindow(int width, int height, std::string name = "default") override;
        virtual void            ReleaseWindow(IWindow* window) override;
        // Events
        virtual IEventHandler*  CreateEventHandler() override;
        virtual void            ReleaseEventHandler(IEventHandler* handler) override;
        // Drawer
        virtual IImageDrawer2D*      CreateImageDrawer2D() override;
        virtual void            ReleaseImageDrawer2D(IImageDrawer2D* drawer) override;
        // Renderer
        virtual IRenderer*      GetRenderer(IWindow* window) override;

    private:

        GXEZContextSDL(GXEZ::IGXEZContext::Type type)
        {
            _type = type;
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
        ImageDrawer2DSDL*                                    _drawer;

        // Renderer (only one to avoid duplicating texture, image, fonts, ....)
        IRenderer*                                      _renderer;

        // THIS - Singleton Instance
        static GXEZContextSDL* _singleton;
        IGXEZContext::Type     _type;
    };
}