#include "VulkanInstance.h"

#include <sstream>

VulkanInstance::VulkanInstance()
{
}

VulkanInstance::~VulkanInstance()
{
	if (m_VkInstance)
	{
		::vkDestroyDevice(m_VkDevice, nullptr);

		if (m_bEnableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(m_VkInstance, m_VkDebugMessenger, nullptr);
		}

		::vkDestroyInstance(m_VkInstance, nullptr);
	}
}

bool VulkanInstance::InitVulkan()
{
	// TODO: Add proper error handling

	if (!CreateInstance())
	{
		::OutputDebugString(L"Failed to create a Vulkan Instance!");
		return false;
	}

	if (!CreateDebugMessenger())
	{
		::OutputDebugString(L"Failed to create Debug messenger!");
		return false;
	}

	if (!CreatePhysicalDevice())
	{
		::OutputDebugString(L"Failed to create VkDevice!");
		return false;
	}

	if (!CreateLogicalDevice())
	{
		::OutputDebugString(L"Failed to create VkDevice!");
		return false;
	}

	return true;
}

bool VulkanInstance::CreateInstance()
{
	if (m_bEnableValidationLayers && !HasWantedValidationLayerSupport())
	{
		::OutputDebugString(L"Error: Validation layers are enabled but the required layers aren't available!");
		return false;
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

	appInfo.pApplicationName = "PlaygroundGameApp";
	appInfo.pEngineName = "No Engine";

	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

	appInfo.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

	createInfo.pApplicationInfo = &appInfo;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(m_WantedExtensions.size());
	createInfo.ppEnabledExtensionNames = m_WantedExtensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

	if (m_bEnableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(m_WantedValidationLayers.size());
		createInfo.ppEnabledLayerNames = m_WantedValidationLayers.data();

		InitialiseDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	VkResult result = ::vkCreateInstance(&createInfo, nullptr, &m_VkInstance);

	if (result != VK_SUCCESS)
	{
		::OutputDebugString(L"Error: Failed to create Vulkan instance!");
		return false;
	}

	return true;
}

bool VulkanInstance::HasWantedValidationLayerSupport()
{
	uint32_t layerCount;
	::vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	::vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* wantedLayerName : m_WantedValidationLayers)
	{
		bool layerFound = false;

		for (const auto& availableLayer : availableLayers)
		{
			if (strcmp(wantedLayerName, availableLayer.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}
	}

	return true;
}

bool VulkanInstance::CreatePhysicalDevice()
{
	uint32_t deviceCount = 0;
	::vkEnumeratePhysicalDevices(m_VkInstance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		::OutputDebugString(L"No graphics devices supporting Vulkan were found.");
		return false;
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	::vkEnumeratePhysicalDevices(m_VkInstance, &deviceCount, devices.data());

	for (const auto& device : devices)
	{
		if (IsDeviceUsable(device))
		{
			m_VkPhysicalDevice = device;
			break;
		}
	}

	if (m_VkPhysicalDevice == VK_NULL_HANDLE)
	{
		::OutputDebugString(L"Failed to find a usable Vulkan device.");
		return false;
	}

	return true;
}

bool VulkanInstance::IsDeviceUsable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices = FindQueueFamilies(device);

	return indices.IsComplete();
}

bool VulkanInstance::CreateLogicalDevice()
{
	QueueFamilyIndices indices = FindQueueFamilies(m_VkPhysicalDevice);

	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

	queueCreateInfo.queueFamilyIndex = indices.GraphicsFamily.value();
	queueCreateInfo.queueCount = 1;

	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures = {}; // TODO

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
	deviceCreateInfo.queueCreateInfoCount = 1;

	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

	deviceCreateInfo.enabledExtensionCount = 0; //static_cast<uint32_t>(m_WantedExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = nullptr; //m_WantedExtensions.data();	// TODO: This is causing an exception saying the requested extensions aren't supported :/

	if (m_bEnableValidationLayers)
	{
		deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_WantedValidationLayers.size());
		deviceCreateInfo.ppEnabledLayerNames = m_WantedValidationLayers.data();
	}
	else
	{
		deviceCreateInfo.enabledLayerCount = 0;
		deviceCreateInfo.ppEnabledLayerNames = nullptr;
	}

	VkResult result = ::vkCreateDevice(m_VkPhysicalDevice, &deviceCreateInfo, nullptr, &m_VkDevice);

	if (result != VK_SUCCESS)
	{
		::OutputDebugString(L"Failed to create logic device!");
		return false;
	}

	::vkGetDeviceQueue(m_VkDevice, indices.GraphicsFamily.value(), 0, &m_VkQueue);

	return true;
}

QueueFamilyIndices VulkanInstance::FindQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices supportedIndicies;

	uint32_t queueFamilyCount = 0;
	::vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	::vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int idx = 0;

	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			supportedIndicies.GraphicsFamily = idx;
		}

		if (supportedIndicies.IsComplete())
		{
			break;
		}

		idx++;
	}

	return supportedIndicies;
}

void VulkanInstance::InitialiseDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

	createInfo.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	createInfo.messageType =
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	createInfo.pfnUserCallback = DebugCallback;
	createInfo.pUserData = nullptr;
}

bool VulkanInstance::CreateDebugMessenger()
{
	if (!m_bEnableValidationLayers) return true;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	InitialiseDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(m_VkInstance, &createInfo, nullptr, &m_VkDebugMessenger) != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	//TEMP: Bit hacky but a quick-n-dirty way to view the output (Until I get proper console output up and running).. Basically convert 'char *' to 'LPCWSTR' for OutputDebugString() to consume.

	std::wstringstream ss;

	ss << "VK: " << pCallbackData->pMessage << std::endl;

	::OutputDebugString(ss.str().c_str());

	return VK_FALSE;
}

VkResult VulkanInstance::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)::vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else
	{
		::OutputDebugString(L"Error: Function not found - vkCreateDebugUtilsMessengerEXT");
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void VulkanInstance::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)::vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}
