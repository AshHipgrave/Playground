#pragma once

#define VK_USE_PLATFORM_WIN32_KHR

#include <Windows.h>

#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_win32.h>

#include <optional>
#include <vector>

struct QueueFamilyIndices
{
	std::optional<uint32_t> GraphicsFamily;
	std::optional<uint32_t> PresentFamily;

	bool IsComplete()
	{
		return GraphicsFamily.has_value() && PresentFamily.has_value();
	}
};

class VulkanInstance
{
public:
	VulkanInstance();
	~VulkanInstance();

	bool InitVulkan(HWND mainWindowHandle);

private:
	bool CreateInstance();
	bool CreateDebugMessenger();
	bool HasWantedValidationLayerSupport();

	bool CreatePhysicalDevice();
	bool IsDeviceUsable(VkPhysicalDevice device);

	bool CreateLogicalDevice();

	bool CreateSurface();

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	void InitialiseDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

	// Proxy functions to VK extension functions.
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

private:
	HWND m_hMainWindowHandle  = nullptr;

	VkInstance m_VkInstance = VK_NULL_HANDLE;

	VkPhysicalDevice m_VkPhysicalDevice = VK_NULL_HANDLE;

	VkDevice m_VkDevice = VK_NULL_HANDLE;

	VkQueue m_VkGraphicsQueue = VK_NULL_HANDLE;
	VkQueue m_VkPresentQueue = VK_NULL_HANDLE;
	
	VkSurfaceKHR m_VkSurfaceKhr = VK_NULL_HANDLE;

	VkDebugUtilsMessengerEXT m_VkDebugMessenger = nullptr;

private:
	const std::vector<const char*> m_WantedValidationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> m_WantedExtensions = {
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
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