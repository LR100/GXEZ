#pragma once

#include "IWindow.h"

namespace GXEZ
{
    class IGraphicContext
    {
    public:
        enum Type
        {
            GRAPHIC_CONTEXT_TYPE_SDL2,
            GRAPHIC_CONTEXT_TYPE_GLFW_VULKAN,
        };

        IGraphicContext() {};
        
        // Basics
        virtual void            Init() = 0;
        virtual void            Release() = 0;
        // Windows
        virtual IWindow*        CreateWindow() = 0;
        // Event Handler
        virtual IEventHandler*  CreateEventHandler() = 0;
    };
}