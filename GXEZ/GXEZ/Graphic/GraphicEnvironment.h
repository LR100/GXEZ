#pragma once

#include "IGraphicContext.h"
#include "GraphicContextGLFWVulkan.h"

namespace GXEZ
{
    class GraphicEnvironment
    {
    public:
        
        static GraphicEnvironment& Get() noexcept { // pour obtenir le singleton
            return (_singleton);
        }

        IGraphicContext* GetContext() { return (_context); };

    private:

        GraphicEnvironment(IGraphicContext::Type context_type)
        {
            Init(context_type);
        }

        ~GraphicEnvironment()
        {
            Release();
        }

        void Init(IGraphicContext::Type context_type)
        {
            if (context_type == IGraphicContext::Type::GRAPHIC_CONTEXT_TYPE_SDL2) {
                // Load SDL2 Environment
            }
            else if (context_type == IGraphicContext::Type::GRAPHIC_CONTEXT_TYPE_GLFW_VULKAN) {
                // Load GLFW Vulkan Environement
                _context = (IGraphicContext*)GraphicContextGLFWVulkan::Get();
            }
            else {
                _context = NULL;
            }
        }

        void Release()
        {
            if (_context) {
                _context->Release();
            }
        }

        IGraphicContext*            _context;
        // THIS - Singleton Instance
        static GraphicEnvironment   _singleton;
    };

    GraphicEnvironment GraphicEnvironment::_singleton(IGraphicContext::Type::GRAPHIC_CONTEXT_TYPE_GLFW_VULKAN);
};



/// 
/// USEFULL Informations can be write down here
///
/// -> Events
///
/// Eventhanlder should be linkable to windows