#pragma once

#include <vulkan\vulkan.hpp>
#include <optional>

struct QueueFamilyIndices
{
	std::optional<uint32_t> GraphicsFamily;

	bool IsComplete()
	{
		return GraphicsFamily.has_value();
	}
};

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

	bool CreatePhysicalDevice();
	bool IsDeviceUsable(VkPhysicalDevice device);

	bool CreateLogicalDevice();

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	void InitialiseDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

	// Proxy functions to VK extension functions.
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

private:
	VkInstance m_VkInstance = VK_NULL_HANDLE;

	VkPhysicalDevice m_VkPhysicalDevice = VK_NULL_HANDLE;

	VkDevice m_VkDevice = VK_NULL_HANDLE;

	VkQueue m_VkQueue = VK_NULL_HANDLE; // (Should I be using nullptr here??)
	
	VkDebugUtilsMessengerEXT m_VkDebugMessenger = nullptr;

private:
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