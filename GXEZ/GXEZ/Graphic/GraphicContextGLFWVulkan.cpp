#include "GraphicContextGLFWVulkan.h"




#include "../Tools/XMLParser.h" // To Load Vulkan Config 



namespace GXEZ
{
	const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}


	/////////////////////////////////
	// GRAPHIC Context GLFW Vuklan //
	//
	//

	GraphicContextGLFWVulkan* GraphicContextGLFWVulkan::_singleton = NULL;

	void GraphicContextGLFWVulkan::Init()
	{
		std::cout << "GraphicContextGLFWVulkan Init()" << std::endl;
		/////////////////////////////////
		// Load GLFWVulkan.config file //

		// Config Values
		_config["appname"] = "default Application Name";

		// XML Config File Parsing 
		XMLParser	parser;
		XMLNode*	tree;
		XMLNode*	cursor;

		if (parser.Parse("GXEZ.xml")) {
			if ((tree = parser.GetTree())) {
				// Config Debug Display
				std::cout << "Tree:\n" << tree->ToString() << std::endl;
				// App Name
				if ((cursor = tree->GetNodeNamed("AppName"))) {
					_config.at("appname") = cursor->GetValue();
				}
			}
		}

		//////////
		// INIT //
		//////////

		// GLFW //

		glfwInit();

		// Vulkan //

		VulkanCreateInstance();
		VulkanSetupDebugMessenger();
		VulkanPickPhysicalDevice();
		VulkanCreateLogicalDevice();
	}

	//////////
	// GLFW //
	//////////

	std::vector<const char*> GraphicContextGLFWVulkan::GLFWGetRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		return extensions;
	}


	////////////
	// Vulkan //
	////////////

	// Instance

	void GraphicContextGLFWVulkan::VulkanCreateInstance()
	{
		if (enableValidationLayers && !VulkanCheckValidationLayerSupport()) {
			throw std::runtime_error("Validation layers requested, but not available!");
		}

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = _config.at("appname").c_str(); // application name
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = GLFWGetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();

			VulkanPopulateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else {
			createInfo.enabledLayerCount = 0;

			createInfo.pNext = nullptr;
		}

		if (vkCreateInstance(&createInfo, nullptr, &_vkInstance) != VK_SUCCESS) {
			throw std::runtime_error("Vulkan Failed to create instance!");
		}
	}

	// Validation

	bool GraphicContextGLFWVulkan::VulkanCheckValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	// Debug Messenger 

	void GraphicContextGLFWVulkan::VulkanPopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
	}

	void GraphicContextGLFWVulkan::VulkanSetupDebugMessenger()
	{
		if (!enableValidationLayers) return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		VulkanPopulateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(_vkInstance, &createInfo, nullptr, &_vkDebugMessenger) != VK_SUCCESS) {
			throw std::runtime_error("Vulkan Failed to set up debug messenger!");
		}
	}

	// Physical Device

	void GraphicContextGLFWVulkan::VulkanPickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, nullptr);

		if (deviceCount == 0) {
			throw std::runtime_error("Vulkan Failed to find GPUs with Vulkan support!");
		}

		std::vector<VkPhysicalDevice> vkdevices(deviceCount);
		vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, vkdevices.data());

		// Use an ordered map to automatically sort candidates by increasing score
		std::multimap<int, PhysicalDevice>		candidates;
		PhysicalDevice							device;

		for (const auto& vkdevice : vkdevices) {
			// Device
			device.device = vkdevice;
			// Properties
			vkGetPhysicalDeviceProperties(device.device, &device.deviceProperties);
			// Features
			vkGetPhysicalDeviceFeatures(device.device, &device.deviceFeatures);
			// Get Rate Score
			int score = VulkanRateDeviceSuitability(device);
			candidates.insert(std::make_pair(score, device));
		}

		// Check if the best candidate is suitable at all
		if (candidates.rbegin()->first > 0) {
			_vkPhysicalDevice = candidates.rbegin()->second;
			std::cout << "Vulkan Chosen Device : (" << _vkPhysicalDevice.deviceProperties.deviceName << ")" << std::endl;
		}
		else {
			throw std::runtime_error("Vulkan Failed to find a suitable GPU!");
		}

		if (_vkPhysicalDevice.device == VK_NULL_HANDLE) {
			throw std::runtime_error("Vulkan Failed to find a suitable GPU!");
		}
	}

	int GraphicContextGLFWVulkan::VulkanRateDeviceSuitability(PhysicalDevice device)
	{
		int score = 0;

		// Discrete GPUs have a significant performance advantage
		if (device.deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			score += 1000;
		}

		// Maximum possible size of textures affects graphics quality
		score += device.deviceProperties.limits.maxImageDimension2D;

		// Application can't function without geometry shaders
		if (!device.deviceFeatures.geometryShader) {
			return 0;
		}

		return score;
	}

	// Queue

	GraphicContextGLFWVulkan::QueueFamilyIndices GraphicContextGLFWVulkan::VulkanFindQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}

			i++;
		}

		return indices;
	}

	void GraphicContextGLFWVulkan::VulkanCreateLogicalDevice()
	{
		QueueFamilyIndices indices = VulkanFindQueueFamilies(_vkPhysicalDevice.device);

		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
		queueCreateInfo.queueCount = 1;

		float queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		VkPhysicalDeviceFeatures deviceFeatures = {};

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.queueCreateInfoCount = 1;

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = 0;

		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(_vkPhysicalDevice.device, &createInfo, nullptr, &_vkDevice) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create logical device!");
		}

		vkGetDeviceQueue(_vkDevice, indices.graphicsFamily.value(), 0, &_vkQraphicsQueue);
	}


	void GraphicContextGLFWVulkan::Release()
	{
		// Vulkan
		if (enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(_vkInstance, _vkDebugMessenger, nullptr);
		}
		if (_vkInstance)
		{
			vkDestroyInstance(_vkInstance, nullptr);
			_vkInstance = NULL;
		}
		// GLFW
		std::cout << "GraphicContextGLFWVulkan Release()" << std::endl;
		glfwTerminate(); // Release GLFW
	}

	IWindow* GraphicContextGLFWVulkan::CreateWindow()
	{
		return (new WindowGLFWVulkan());
	}

	IEventHandler* GraphicContextGLFWVulkan::CreateEventHandler()
	{
		return (new EventHandlerGLFW());
	}

	IDrawer2D* GraphicContextGLFWVulkan::CreateDrawer2D()
	{
		return nullptr;
	}
}

