#pragma once

#include "PlaygroundCommon.h"

#define VK_USE_PLATFORM_WIN32_KHR

#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_win32.h>

#include "imgui_impl_win32.h"
#include "imgui_impl_vulkan.h"

#include "Vector2f.h"
#include "Vector3f.h"
#include "Matrix4f.h"

#include "MatrixTransforms.h"

struct UniformBufferObject
{
	Matrix4f Model;
	Matrix4f View;
	Matrix4f Projection;
};

struct QueueFamilyIndices
{
	std::optional<uint32_t> GraphicsFamily;
	std::optional<uint32_t> PresentFamily;

	bool IsComplete()
	{
		return GraphicsFamily.has_value() && PresentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR SurfaceCapabilities = {};

	std::vector<VkSurfaceFormatKHR> SurfaceFormats;
	std::vector<VkPresentModeKHR> PresentModes;
};

struct Vertex
{
	Vector2f Position;
	Vector3f Colour;

	static VkVertexInputBindingDescription GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription = 
		{
			0,
			sizeof(Vertex),
			VK_VERTEX_INPUT_RATE_VERTEX
		};

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, Position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, Colour);

		return attributeDescriptions;
	};
};

const int MAX_FRAMES_IN_FLIGHT = 2;

class VulkanInstance
{
public:
	VulkanInstance();
	~VulkanInstance();

	bool InitVulkan(HWND mainWindowHandle);
	bool IsInitialised();

	void DrawFrame(const float& deltaTime);

	void RecreateSwapChain();

private:
	void CleanupSwapChain();

	bool CreateInstance();
	bool CreateDebugMessenger();
	bool CreateSurface();

	bool CreatePhysicalDevice();
	bool CreateLogicalDevice();
	bool CreateSwapChain();

	bool CreateImageViews();
	bool CreateRenderPass();

	bool CreateDescriptorSetLayout();
	bool CreateGraphicsPipeline();
	bool CreateFramebuffers();

	bool CreateCommandPool();

	bool CreateBuffer(VkDeviceSize bufferSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags propertyFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	bool CreateVertexBuffer();
	bool CreateIndexBuffer();
	bool CreateUniformBuffers();

	bool CreateDescriptorPool();
	bool CreateDescriptorSets();

	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize bufferSize);

	void UpdateUniformBuffer(uint32_t currentImageIdx, const float& deltaTime);

	bool CreateCommandBuffers();
	bool CreateSyncObjects();

	bool InitDearImGui(HWND mainWindowHandle);

	VkSurfaceFormatKHR SelectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR SelectSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D SelectSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);

	bool IsDeviceUsable(VkPhysicalDevice device);

	bool HasWantedValidationLayerSupport();
	bool HasWantedDeviceExtensionSupport(VkPhysicalDevice device);

	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

private:
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags);

	VkShaderModule CreateShaderModule(const std::vector<char>& bytecode);

private:
	void InitialiseDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

	// Proxy functions to VK extension functions.
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

private:
	HWND m_hMainWindowHandle  = nullptr;

	bool m_bIsVulkanInitialised = false;

	size_t m_CurrentFrameIdx = 0;

	VkInstance m_VkInstance = VK_NULL_HANDLE;

	VkDevice m_VkDevice = VK_NULL_HANDLE;
	VkPhysicalDevice m_VkPhysicalDevice = VK_NULL_HANDLE;

	VkQueue m_VkGraphicsQueue = VK_NULL_HANDLE;
	VkQueue m_VkPresentQueue = VK_NULL_HANDLE;

	VkBuffer m_VkVertexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory m_VertexBufferMemory = VK_NULL_HANDLE;

	VkBuffer m_VkIndexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory m_VkIndexBufferMemory = VK_NULL_HANDLE;
	
	VkSurfaceKHR m_VkSurfaceKhr = VK_NULL_HANDLE;

	VkSwapchainKHR m_VkSwapChainKhr = VK_NULL_HANDLE;
	VkRenderPass m_VkRenderPass = VK_NULL_HANDLE;

	VkDescriptorSetLayout m_VkDescriptorSetLayout = VK_NULL_HANDLE;
	VkPipelineLayout m_VkPipelineLayout = VK_NULL_HANDLE;

	VkPipeline m_VkGraphicsPipeline = VK_NULL_HANDLE;
	VkCommandPool m_VkCommandPool = VK_NULL_HANDLE;

	VkDescriptorPool m_VkDescriptorPool = VK_NULL_HANDLE;

	VkFormat m_VkSwapChainFormat;
	VkExtent2D m_VkSwapChainExtent2D;

	VkDebugUtilsMessengerEXT m_VkDebugMessenger = nullptr;

private:
	std::vector<VkFence> m_InFlightFences;
	std::vector<VkFence> m_ImagesInFlight;

	std::vector<VkImage> m_SwapChainImages;
	std::vector<VkImageView> m_SwapChainImageViews;

	std::vector<VkFramebuffer> m_VkFramebuffers;
	std::vector<VkCommandBuffer> m_VkCommandBuffers;

	std::vector<VkBuffer> m_VkUniformBuffers;
	std::vector<VkDeviceMemory> m_VkUniformBuffersMemory;

	std::vector<VkDescriptorSet> m_VkDescriptorSets;

	std::vector<VkSemaphore> m_ImageAvailableSemaphores;
	std::vector<VkSemaphore> m_RenderCompleteSemaphores;

private:
	const std::vector<Vertex> m_Verticies = 
	{
		// Position:		// Colour:
		{ { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, } },
		{ {  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f, } },
		{ {  0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f, } },
		{ { -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f, } }
	};

	const std::vector<uint16_t> m_Indicies =
	{
		0, 1, 2, 2, 3, 0
	};

private:
	const std::vector<const char*> m_WantedValidationLayers = 
	{
		"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> m_WantedDeviceExtensions = 
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	const std::vector<const char*> m_WantedInstanceExtensions = 
	{
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