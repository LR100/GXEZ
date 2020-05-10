#pragma once

#include "GXEZ/Graphic/IWindow.h"
#include "GXEZ/Graphic/IImageDrawer2D.h"
#include "GXEZ/Graphic/IRenderer.h"
#include "GXEZ/Event/IEventHandler.h"

namespace GXEZ
{
    class IGXEZContext
    {
    public:
        enum Type
        {
            GRAPHIC_CONTEXT_TYPE_SDL2_RENDERER,
            GRAPHIC_CONTEXT_TYPE_SDL2_DRAWER, // DEPRECATED
            GRAPHIC_CONTEXT_TYPE_GLFW_VULKAN_RENDERER,
        };

        IGXEZContext() {};
        
        // Basics
        virtual void            Init() = 0;
        virtual void            Release() = 0;
        virtual Type            GetType() const = 0;
        // Windows
        virtual IWindow*        CreateWindow(int width, int height, std::string name = "default") = 0;
        virtual void            ReleaseWindow(IWindow* window) = 0;
        // Event Handler
        virtual IEventHandler*  CreateEventHandler() = 0;
        virtual void            ReleaseEventHandler(IEventHandler* handler) = 0;
        // Drawer
        virtual IImageDrawer2D* CreateImageDrawer2D() = 0;
        virtual void            ReleaseImageDrawer2D(IImageDrawer2D* drawer) = 0;
        // Renderer
        virtual IRenderer*      GetRenderer(IWindow* window) = 0;

        
    };
}