#pragma once

#include <vulkan\vulkan.hpp>

class VulkanInstance
{
public:
	VulkanInstance();
	~VulkanInstance();

	bool InitVulkan();

private:
	bool CreateInstance();
	bool CreateDebugMessenger();
	bool HasWantedValidationLayerSupport();

	bool CreateDevice();
	bool IsDeviceUsable(VkPhysicalDevice device);

	void InitialiseDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

	// Proxy functions to VK extension functions.
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

private:
	VkInstance m_VkInstance = VK_NULL_HANDLE;

	VkPhysicalDevice m_VkDevice = VK_NULL_HANDLE;
	
	VkDebugUtilsMessengerEXT m_VkDebugMessenger = nullptr;

	const std::vector<const char*> m_WantedValidationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> m_WantedExtensions = {
		VK_KHR_SURFACE_EXTENSION_NAME,
#if defined(_DEBUG) || defined(DEBUG)
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
	};

#if defined(_DEBUG) || defined(DEBUG)
	const bool m_bEnableValidationLayers = true;
#else
	const bool m_bEnableValidationLayers = false;
#endif
};