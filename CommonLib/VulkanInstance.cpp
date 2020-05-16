#include "VulkanInstance.h"

#include <cstring>
#include <iostream>
#include <sstream>

VulkanInstance::VulkanInstance()
{
}

VulkanInstance::~VulkanInstance()
{
	if (m_VkInstance)
	{
		if (m_bEnableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(m_VkInstance, m_VkDebugMessenger, nullptr);
		}

		::vkDestroyInstance(m_VkInstance, nullptr);
	}
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

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

	if (m_bEnableValidationLayers)
	{
		createInfo.enabledExtensionCount = static_cast<uint32_t>(m_WantedExtensions.size());
		createInfo.ppEnabledExtensionNames = m_WantedExtensions.data();

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

	if (!CreateDebugMessenger())
	{
		::OutputDebugString(L"Failed to create Debug messenger!");
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

void VulkanInstance::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
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
