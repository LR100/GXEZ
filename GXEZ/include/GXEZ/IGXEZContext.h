#pragma once

#include "GXEZ/Graphic/IWindow.h"
#include "GXEZ/Graphic/IDrawer2D.h"
#include "GXEZ/Event/IEventHandler.h"

namespace GXEZ
{
    class IGXEZContext
    {
    public:
        enum Type
        {
            GRAPHIC_CONTEXT_TYPE_SDL2,
            GRAPHIC_CONTEXT_TYPE_GLFW_VULKAN,
        };

        IGXEZContext() {};
        
        // Basics
        virtual void            Init() = 0;
        virtual void            Release() = 0;
        // Windows
        virtual IWindow*        CreateWindow(int width, int height, std::string name = "default") = 0;
        virtual void            ReleaseWindow(IWindow* window) = 0;
        // Event Handler
        virtual IEventHandler*  CreateEventHandler() = 0;
        virtual void            ReleaseEventHandler(IEventHandler* handler) = 0;
        // Drawer
        virtual IDrawer2D*      CreateDrawer2D() = 0;
        virtual void            ReleaseDrawer2D(IDrawer2D* drawer) = 0;
    };
}