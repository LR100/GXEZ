// GXEZ.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "GraphicEnvironment.h"

using namespace GXEZ;

class Program
{
public :

    void Init()
    {
        _window = graphicContext->CreateWindow();
        _eventHandler = graphicContext->CreateEventHandler();
        _window->Open(800, 600, "Vulkan window");
        _window->LinkEventHandler(_eventHandler);
        _eventHandler->AddHandlerToEvent(ControlKey::KEY_ESCAPE, ControlKeyState::RELEASED, &Program::Quit, this);
    }

    void Loop()
    {
        while (_window->IsOpen())
        {
            _eventHandler->UpdateEvents();
            _window->Refresh();
        }
    }

    void Quit()
    {
        _window->Close();
    }

private:

    GXEZ::GraphicEnvironment&   graphicEnv = GXEZ::GraphicEnvironment::Get();
    GXEZ::IGraphicContext*      graphicContext = graphicEnv.GetContext();
    GXEZ::IEventHandler*        _eventHandler;
    GXEZ::IWindow*              _window;
};

int main()
{
   
    Program program;

    program.Init();
    program.Loop();
    program.Quit();
    

    
    
    /*glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::cout << extensionCount << " extensions supported" << std::endl;

    glm::mat4 matrix;
    glm::vec4 vec;
    auto test = matrix * vec;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);*/


    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
