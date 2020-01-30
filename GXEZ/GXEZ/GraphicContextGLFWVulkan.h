#pragma once

#include "IGraphicContext.h"
#include "WindowGLFWVulkan.h"

#include <iostream>

namespace GXEZ
{
    class GraphicContextGLFWVulkan : public IGraphicContext
    {
    public:
        GraphicContextGLFWVulkan()
        {
            Init();
        }

        // Basics
        virtual void Init() override
        {
            std::cout << "GraphicContextGLFWVulkan Init()" << std::endl;
            glfwInit(); // Init GLFW
        }

        virtual void Release() override
        {
            std::cout << "GraphicContextGLFWVulkan Release()" << std::endl;
            glfwTerminate(); // Release GLFW
        }

        // Windows
        virtual IWindow* CreateWindow() override
        {
            return (new WindowGLFWVulkan());
        }
    };
}