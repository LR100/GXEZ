#pragma once

#include "IGraphicContext.h"
#include "WindowGLFWVulkan.h"
#include "../Event/EventHandlerGLFW.h"


// Vulkan Debug Messenger
#include <cstring>
#include <cstdlib>
#include <optional>

#include <iostream>

namespace GXEZ
{
    class GraphicContextGLFWVulkan : public IGraphicContext
    {
    public:

        static GraphicContextGLFWVulkan* Get() noexcept { // pour obtenir le singleton
            if (_singleton == NULL)
            {
                _singleton = new GraphicContextGLFWVulkan();
            }
            return (_singleton);
        }
        

        // Basics
        virtual void Release() override;
        // Windows
        virtual IWindow*        CreateWindow() override;
        // Events
        virtual IEventHandler*  CreateEventHandler() override;
        // Drawer
        virtual IDrawer2D*      CreateDrawer2D() override;

    private:

        GraphicContextGLFWVulkan()
        {
            Init();
        }

        // Basics
        virtual void Init() override;

        //////////
        // GLFW //
        //////////

        std::vector<const char*>    GLFWGetRequiredExtensions();

        ////////////
        // Vulkan //
        ////////////

        struct QueueFamilyIndices {
            std::optional<uint32_t> graphicsFamily;
            bool isComplete() {
                return graphicsFamily.has_value();
            }
        };

        struct PhysicalDevice
        {
            VkPhysicalDevice            device = VK_NULL_HANDLE;
            VkPhysicalDeviceProperties  deviceProperties;
            VkPhysicalDeviceFeatures    deviceFeatures;
        };

        // Instance
        void                        VulkanCreateInstance();
        // Validation 
        bool                        VulkanCheckValidationLayerSupport();
        // Debug Messenger
        void                        VulkanPopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        void                        VulkanSetupDebugMessenger();
        // Physical Device
        void                        VulkanPickPhysicalDevice();
        int                         VulkanRateDeviceSuitability(PhysicalDevice device);
        // Queue
        QueueFamilyIndices          VulkanFindQueueFamilies(VkPhysicalDevice device);
        // Logical Device
        void                        VulkanCreateLogicalDevice();

        

        
        

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
            std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
            return VK_FALSE;
        }

        VkInstance                  _vkInstance;
        VkApplicationInfo           _vkAppInfo;
        VkDebugUtilsMessengerEXT    _vkDebugMessenger;
        PhysicalDevice              _vkPhysicalDevice;
        VkDevice                    _vkDevice;
        VkQueue                     _vkQraphicsQueue;
   
        
        // Config //
        std::unordered_map<std::string, std::string>    _config;
        

        // THIS - Singleton Instance
        static GraphicContextGLFWVulkan* _singleton;
    };
}