#pragma once

#include <vulkan\vulkan.hpp>

class VulkanInstance
{
public:
	VulkanInstance();
	~VulkanInstance();

	bool CreateInstance();

private:
	bool HasWantedValidationLayerSupport();

	void InitialiseDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	bool CreateDebugMessenger();

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

private:
	VkInstance m_VkInstance = VK_NULL_HANDLE;
	
	VkDebugUtilsMessengerEXT m_VkDebugMessenger = nullptr;

	const std::vector<const char*> m_WantedValidationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> m_WantedExtensions = {
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};

#if defined(_DEBUG) || defined(DEBUG)
	const bool m_bEnableValidationLayers = true;
#else
	const bool m_bEnableValidationLayers = false;
#endif

private:

	
	/*
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

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

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}*/
};