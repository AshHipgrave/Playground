#include "VulkanInstance.h"

VulkanInstance::VulkanInstance()
{
}

VulkanInstance::~VulkanInstance()
{
	if (m_VkInstance)
	{
		for (auto imageView : m_SwapChainImageViews)
		{
			::vkDestroyImageView(m_VkDevice, imageView, nullptr);
		}

		::vkDestroySwapchainKHR(m_VkDevice, m_VkSwapChainKhr, nullptr);

		::vkDestroyDevice(m_VkDevice, nullptr);

		if (m_bEnableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(m_VkInstance, m_VkDebugMessenger, nullptr);
		}

		::vkDestroySurfaceKHR(m_VkInstance, m_VkSurfaceKhr, nullptr);

		::vkDestroyInstance(m_VkInstance, nullptr);
	}
}

bool VulkanInstance::InitVulkan(HWND mainWindowHandle)
{
	m_hMainWindowHandle = mainWindowHandle;

	// TODO: Have a better way to do this rather than lots of if checks.

	if (!CreateInstance())
		return false;

	if (!CreateDebugMessenger())
		return false;

	if (!CreateSurface())
		return false;

	if (!CreatePhysicalDevice())
		return false;

	if (!CreateLogicalDevice())
		return false;

	if (!CreateSwapChain())
		return false;

	if (!CreateImageViews())
		return false;

	if (!CreateGraphicsPipeline())
		return false;

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

	createInfo.enabledExtensionCount = static_cast<uint32_t>(m_WantedInstanceExtensions.size());
	createInfo.ppEnabledExtensionNames = m_WantedInstanceExtensions.data();

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

bool VulkanInstance::CreateSurface()
{
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;

	surfaceCreateInfo.hwnd = m_hMainWindowHandle;
	surfaceCreateInfo.hinstance = ::GetModuleHandle(nullptr);

	VkResult result = ::vkCreateWin32SurfaceKHR(m_VkInstance, &surfaceCreateInfo, nullptr, &m_VkSurfaceKhr);

	if (result != VK_SUCCESS)
	{
		::OutputDebugString(L"Failed to create window surface!");
		return false;
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

bool VulkanInstance::CreateLogicalDevice()
{
	QueueFamilyIndices indices = FindQueueFamilies(m_VkPhysicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

	const float queuePriority = 1.0f;

	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;

		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {}; // TODO

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_WantedDeviceExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = m_WantedDeviceExtensions.data();

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

	::vkGetDeviceQueue(m_VkDevice, indices.GraphicsFamily.value(), 0, &m_VkGraphicsQueue); 
	::vkGetDeviceQueue(m_VkDevice, indices.PresentFamily.value(), 0, &m_VkPresentQueue);

	return true;
}

bool VulkanInstance::CreateSwapChain()
{
	assert(m_VkInstance && m_VkDevice && m_VkSurfaceKhr);

	SwapChainSupportDetails swapChainSupportDetails = QuerySwapChainSupport(m_VkPhysicalDevice);

	VkSurfaceFormatKHR surfaceFormat = SelectSwapSurfaceFormat(swapChainSupportDetails.SurfaceFormats);
	VkPresentModeKHR presentMode = SelectSwapPresentMode(swapChainSupportDetails.PresentModes);
	VkExtent2D extent = SelectSwapExtent(swapChainSupportDetails.SurfaceCapabilities);

	uint32_t imageCount = swapChainSupportDetails.SurfaceCapabilities.minImageCount + 1;

	if (swapChainSupportDetails.SurfaceCapabilities.maxImageCount > 0 && imageCount > swapChainSupportDetails.SurfaceCapabilities.maxImageCount)
	{
		imageCount = swapChainSupportDetails.SurfaceCapabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

	swapChainCreateInfo.surface = m_VkSurfaceKhr;
	swapChainCreateInfo.minImageCount = imageCount;
	swapChainCreateInfo.imageFormat = surfaceFormat.format;
	swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapChainCreateInfo.imageExtent = extent;
	swapChainCreateInfo.imageArrayLayers = 1;
	swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //VK_IMAGE_USAGE_TRANSFER_DST_BIT - For deferred style rendering where everything goes to a buffer first

	QueueFamilyIndices indices = FindQueueFamilies(m_VkPhysicalDevice);
	uint32_t queueFamilyIndicies[] = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

	if (indices.GraphicsFamily != indices.PresentFamily)
	{
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapChainCreateInfo.queueFamilyIndexCount = 2;
		swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndicies;
	}
	else
	{
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainCreateInfo.queueFamilyIndexCount = 0;
		swapChainCreateInfo.pQueueFamilyIndices = nullptr;
	}

	swapChainCreateInfo.preTransform = swapChainSupportDetails.SurfaceCapabilities.currentTransform;
	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapChainCreateInfo.presentMode = presentMode;
	swapChainCreateInfo.clipped = VK_TRUE;
	swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE; // TODO: Add support for swap chain recreation (e.g. Window resize!)

	VkResult result = ::vkCreateSwapchainKHR(m_VkDevice, &swapChainCreateInfo, nullptr, &m_VkSwapChainKhr);

	if (result != VK_SUCCESS)
	{
		::OutputDebugString(L"Failed to create SwapChain!");
		return false;
	}

	::vkGetSwapchainImagesKHR(m_VkDevice, m_VkSwapChainKhr, &imageCount, nullptr);
	m_SwapChainImages.resize(imageCount);
	::vkGetSwapchainImagesKHR(m_VkDevice, m_VkSwapChainKhr, &imageCount, m_SwapChainImages.data());

	m_VkSwapChainFormat = surfaceFormat.format;
	m_VkSwapChainExtent2D = extent;

	return true;
}

bool VulkanInstance::CreateImageViews()
{
	m_SwapChainImageViews.resize(m_SwapChainImages.size());

	for (size_t i = 0; i < m_SwapChainImages.size(); i++)
	{
		VkImageViewCreateInfo imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

		imageViewCreateInfo.image = m_SwapChainImages[i];
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = m_VkSwapChainFormat;

		imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 0;

		VkResult result = ::vkCreateImageView(m_VkDevice, &imageViewCreateInfo, nullptr, &m_SwapChainImages[i]);

		if (result != VK_SUCCESS)
		{
			::OutputDebugString(L"Failed to create image views!");
			return false;
		}
	}

	return true;
}

bool VulkanInstance::CreateGraphicsPipeline()
{
	return false;
}

VkSurfaceFormatKHR VulkanInstance::SelectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR VulkanInstance::SelectSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanInstance::SelectSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
{
	if (surfaceCapabilities.currentExtent.width != UINT32_MAX)
	{
		return surfaceCapabilities.currentExtent;
	}
	else
	{
		RECT windowRect;
		::GetWindowRect(m_hMainWindowHandle, &windowRect);

		int width = windowRect.right - windowRect.left;
		int height = windowRect.bottom - windowRect.top;

		VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

		actualExtent.width = std::max(surfaceCapabilities.minImageExtent.width, std::min(surfaceCapabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(surfaceCapabilities.minImageExtent.height, std::min(surfaceCapabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

bool VulkanInstance::IsDeviceUsable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices = FindQueueFamilies(device);

	bool extensionsSupported = HasWantedDeviceExtensionSupport(device);

	bool swapChainAdequate = false;

	if (extensionsSupported)
	{
		SwapChainSupportDetails swapChainSupportDetails = QuerySwapChainSupport(device);
		swapChainAdequate = !swapChainSupportDetails.SurfaceFormats.empty() && !swapChainSupportDetails.PresentModes.empty();
	}

	return indices.IsComplete() && extensionsSupported && swapChainAdequate;
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

bool VulkanInstance::HasWantedDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	::vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(m_WantedDeviceExtensions.begin(), m_WantedDeviceExtensions.end());

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

SwapChainSupportDetails VulkanInstance::QuerySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;
	::vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_VkSurfaceKhr, &details.SurfaceCapabilities);

	uint32_t formatCount;
	::vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_VkSurfaceKhr, &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.SurfaceFormats.resize(formatCount);
		::vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_VkSurfaceKhr, &formatCount, details.SurfaceFormats.data());
	}

	uint32_t presentModeCount;
	::vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_VkSurfaceKhr, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.PresentModes.resize(presentModeCount);
		::vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_VkSurfaceKhr, &presentModeCount, details.PresentModes.data());
	}

	return details;
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
		VkBool32 hasPresentSupport = false;
		::vkGetPhysicalDeviceSurfaceSupportKHR(device, idx, m_VkSurfaceKhr, &hasPresentSupport);

		if (hasPresentSupport)
		{
			supportedIndicies.PresentFamily = idx;
		}

		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			supportedIndicies.GraphicsFamily = idx;
		}

		if (supportedIndicies.IsComplete()) 
			break;

		idx++;
	}

	return supportedIndicies;
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
