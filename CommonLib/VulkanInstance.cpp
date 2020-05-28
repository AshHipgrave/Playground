#include "VulkanInstance.h"

VulkanInstance::VulkanInstance()
{
}

VulkanInstance::~VulkanInstance()
{
	if (m_VkInstance)
	{
		::vkDeviceWaitIdle(m_VkDevice);

		CleanupSwapChain();

		::vkDestroyDescriptorSetLayout(m_VkDevice, m_VkDescriptorSetLayout, nullptr);

		::vkDestroyBuffer(m_VkDevice, m_VkIndexBuffer, nullptr);
		::vkFreeMemory(m_VkDevice, m_VkIndexBufferMemory, nullptr);

		::vkDestroyBuffer(m_VkDevice, m_VkVertexBuffer, nullptr);
		::vkFreeMemory(m_VkDevice, m_VertexBufferMemory, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			::vkDestroySemaphore(m_VkDevice, m_RenderCompleteSemaphores[i], nullptr);
			::vkDestroySemaphore(m_VkDevice, m_ImageAvailableSemaphores[i], nullptr);

			::vkDestroyFence(m_VkDevice, m_InFlightFences[i], nullptr);
		}

		::vkDestroyCommandPool(m_VkDevice, m_VkCommandPool, nullptr);

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

	if (!CreateRenderPass())
		return false;

	if (!CreateDescriptorSetLayout())
		return false;

	if (!CreateGraphicsPipeline())
		return false;

	if (!CreateFramebuffers())
		return false;

	if (!CreateCommandPool())
		return false;

	if (!CreateVertexBuffer())
		return false;

	if (!CreateIndexBuffer())
		return false;

	if (!CreateUniformBuffers())
		return false;

	if (!CreateDescriptorPool())
		return false;

	if (!CreateDescriptorSets())
		return false;

	if (!CreateCommandBuffers())
		return false;

	if (!CreateSyncObjects())
		return false;

	if (!InitDearImGui(mainWindowHandle))
		return false;

	m_bIsVulkanInitialised = true;

	return true;
}

bool VulkanInstance::IsInitialised()
{
	return m_bIsVulkanInitialised;
}

void VulkanInstance::DrawFrame(const float& deltaTime)
{
	::vkWaitForFences(m_VkDevice, 1, &m_InFlightFences[m_CurrentFrameIdx], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = ::vkAcquireNextImageKHR(m_VkDevice, m_VkSwapChainKhr, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrameIdx], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS)
	{
		throw new std::exception("Failed to obtain SwapChain image!");
	}

	if (m_ImagesInFlight[imageIndex] != VK_NULL_HANDLE)
	{
		::vkWaitForFences(m_VkDevice, 1, &m_ImagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}

	m_ImagesInFlight[imageIndex] = m_InFlightFences[m_CurrentFrameIdx];

	UpdateUniformBuffer(imageIndex, deltaTime);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrameIdx] };
	VkPipelineStageFlags waitStageFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStageFlags;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_VkCommandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = { m_RenderCompleteSemaphores[m_CurrentFrameIdx] };

	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	::vkResetFences(m_VkDevice, 1, &m_InFlightFences[m_CurrentFrameIdx]);

	if (::vkQueueSubmit(m_VkGraphicsQueue, 1, &submitInfo, m_InFlightFences[m_CurrentFrameIdx]) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to submit draw command!");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { m_VkSwapChainKhr };

	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	result = ::vkQueuePresentKHR(m_VkPresentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
	{
		RecreateSwapChain();
	}
	else if (result != VK_SUCCESS)
	{
		throw std::exception("Failed to present swap image!");
	}

	::vkDeviceWaitIdle(m_VkDevice);
}

void VulkanInstance::RecreateSwapChain()
{
	if (!m_bIsVulkanInitialised)
		return;

	::vkDeviceWaitIdle(m_VkDevice);

	CleanupSwapChain();

	CreateSwapChain();
	CreateImageViews();

	CreateRenderPass();
	CreateGraphicsPipeline();

	CreateFramebuffers();
	CreateUniformBuffers();

	CreateDescriptorPool();
	CreateDescriptorSets();

	CreateCommandBuffers();
}

void VulkanInstance::CleanupSwapChain()
{
	for (size_t i = 0; i < m_SwapChainImages.size(); i++)
	{
		::vkDestroyBuffer(m_VkDevice, m_VkUniformBuffers[i], nullptr);
		::vkFreeMemory(m_VkDevice, m_VkUniformBuffersMemory[i], nullptr);
	}

	::vkDestroyDescriptorPool(m_VkDevice, m_VkDescriptorPool, nullptr);

	for (size_t i = 0; i < m_VkFramebuffers.size(); i++)
	{
		::vkDestroyFramebuffer(m_VkDevice, m_VkFramebuffers[i], nullptr);
	}

	::vkFreeCommandBuffers(m_VkDevice, m_VkCommandPool, static_cast<uint32_t>(m_VkCommandBuffers.size()), m_VkCommandBuffers.data());

	::vkDestroyPipeline(m_VkDevice, m_VkGraphicsPipeline, nullptr);
	::vkDestroyPipelineLayout(m_VkDevice, m_VkPipelineLayout, nullptr);

	::vkDestroyRenderPass(m_VkDevice, m_VkRenderPass, nullptr);

	for (auto imageView : m_SwapChainImageViews)
	{
		::vkDestroyImageView(m_VkDevice, imageView, nullptr);
	}

	::vkDestroySwapchainKHR(m_VkDevice, m_VkSwapChainKhr, nullptr);
}

bool VulkanInstance::CreateInstance()
{
	if (m_bEnableValidationLayers && !HasWantedValidationLayerSupport())
	{
		::OutputDebugString(L"Error: Validation layers are enabled but the required layers aren't available!");
		return false;
	}

	VkApplicationInfo appInfo =
	{
		VK_STRUCTURE_TYPE_APPLICATION_INFO,
		nullptr,
		"PlaygroundGameApp",
		VK_MAKE_VERSION(1, 0, 0),
		"No Engine",
		VK_MAKE_VERSION(1, 0, 0),
		VK_API_VERSION_1_2
	};

	VkInstanceCreateInfo instanceCreateInfo =
	{
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		nullptr,
		0,
		&appInfo,
		0,
		nullptr,
		static_cast<uint32_t>(m_WantedInstanceExtensions.size()),
		m_WantedInstanceExtensions.data()
	};

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

	if (m_bEnableValidationLayers)
	{
		instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_WantedValidationLayers.size());
		instanceCreateInfo.ppEnabledLayerNames = m_WantedValidationLayers.data();

		InitialiseDebugMessengerCreateInfo(debugCreateInfo);
		instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}

	if (::vkCreateInstance(&instanceCreateInfo, nullptr, &m_VkInstance) != VK_SUCCESS)
	{
		::OutputDebugString(L"Error: Failed to create Vulkan instance!");
		return false;
	}

	return true;
}

bool VulkanInstance::CreateSurface()
{
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo =
	{
		VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
		nullptr,
		0,
		::GetModuleHandle(nullptr),
		m_hMainWindowHandle
	};

	if (::vkCreateWin32SurfaceKHR(m_VkInstance, &surfaceCreateInfo, nullptr, &m_VkSurfaceKhr) != VK_SUCCESS)
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
	std::set<uint32_t> uniqueQueueFamilies = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

	std::vector<float> queuePriorities = { 1.0f };
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo =
		{
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			nullptr,
			0,
			queueFamily,
			1,
			&queuePriorities[0]
		};

		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkDeviceCreateInfo deviceCreateInfo =
	{
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		nullptr,
		0,
		static_cast<uint32_t>(queueCreateInfos.size()),
		queueCreateInfos.data(),
		0,
		nullptr,
		static_cast<uint32_t>(m_WantedDeviceExtensions.size()),
		m_WantedDeviceExtensions.data(),
		nullptr
	};

	if (m_bEnableValidationLayers)
	{
		deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_WantedValidationLayers.size());
		deviceCreateInfo.ppEnabledLayerNames = m_WantedValidationLayers.data();
	}

	if (::vkCreateDevice(m_VkPhysicalDevice, &deviceCreateInfo, nullptr, &m_VkDevice) != VK_SUCCESS)
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
		imageViewCreateInfo.subresourceRange.layerCount = 1;

		VkResult result = ::vkCreateImageView(m_VkDevice, &imageViewCreateInfo, nullptr, &m_SwapChainImageViews[i]);

		if (result != VK_SUCCESS)
		{
			::OutputDebugString(L"Failed to create image views!");
			return false;
		}
	}

	return true;
}

bool VulkanInstance::CreateRenderPass()
{
	VkAttachmentDescription attachmentDescription = {};
	attachmentDescription.format = m_VkSwapChainFormat;
	attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;

	attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

	attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference attachmentReference = {};
	attachmentReference.attachment = 0;
	attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDescription = {};
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &attachmentReference;

	VkSubpassDependency subpassDependency = {};
	subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependency.dstSubpass = 0;
	subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.srcAccessMask = 0;
	subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = 1;
	renderPassCreateInfo.pAttachments = &attachmentDescription;
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpassDescription;
	renderPassCreateInfo.dependencyCount = 1;
	renderPassCreateInfo.pDependencies = &subpassDependency;

	VkResult result = ::vkCreateRenderPass(m_VkDevice, &renderPassCreateInfo, nullptr, &m_VkRenderPass);

	if (result != VK_SUCCESS)
	{
		::OutputDebugString(L"Failed to create RenderPass");
		return false;
	}

	return true;
}

bool VulkanInstance::CreateDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;

	if (::vkCreateDescriptorSetLayout(m_VkDevice, &layoutInfo, nullptr, &m_VkDescriptorSetLayout) != VK_SUCCESS)
	{
		::OutputDebugString(L"Failed to create descriptor set layout");
		return false;
	}

	return true;
}

bool VulkanInstance::CreateGraphicsPipeline()
{
	auto vertexShaderBytecode = ReadFileAsBinary("vert.spv");
	auto fragmentShaderBytecode = ReadFileAsBinary("frag.spv");

	VkShaderModule vertexShaderModule = CreateShaderModule(vertexShaderBytecode);
	VkShaderModule fragmentShaderModule = CreateShaderModule(fragmentShaderBytecode);

	VkPipelineShaderStageCreateInfo vertexShaderStageCreateInfo = {};
	vertexShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertexShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertexShaderStageCreateInfo.module = vertexShaderModule;
	vertexShaderStageCreateInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragmentShaderStageCreateInfo = {};
	fragmentShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragmentShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragmentShaderStageCreateInfo.module = fragmentShaderModule;
	fragmentShaderStageCreateInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStageCreateInfo, fragmentShaderStageCreateInfo };

	auto bindingDescription = Vertex::GetBindingDescription();
	auto attributeDescription = Vertex::GetAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo  vertexInputStageCreateInfo = {};
	vertexInputStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputStageCreateInfo.vertexBindingDescriptionCount = 1;
	vertexInputStageCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescription.size());
	vertexInputStageCreateInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputStageCreateInfo.pVertexAttributeDescriptions = attributeDescription.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStageCreateInfo = {};
	inputAssemblyStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyStageCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyStageCreateInfo.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)m_VkSwapChainExtent2D.width;
	viewport.height = (float)m_VkSwapChainExtent2D.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissorRect = {};
	scissorRect.offset = { 0, 0 };
	scissorRect.extent = m_VkSwapChainExtent2D;

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
	viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.pViewports = &viewport;
	viewportStateCreateInfo.scissorCount = 1;
	viewportStateCreateInfo.pScissors = &scissorRect;

	VkPipelineRasterizationStateCreateInfo rasterizerStageCreateInfo = {};
	rasterizerStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizerStageCreateInfo.depthClampEnable = VK_FALSE;
	rasterizerStageCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizerStageCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizerStageCreateInfo.lineWidth = 1.0f;
	rasterizerStageCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizerStageCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizerStageCreateInfo.depthBiasEnable = VK_FALSE;
	rasterizerStageCreateInfo.depthBiasConstantFactor = 0.0f;
	rasterizerStageCreateInfo.depthBiasClamp = 0.0f;
	rasterizerStageCreateInfo.depthBiasSlopeFactor = 0.0f;

	VkPipelineMultisampleStateCreateInfo multisamplingStateCreateInfo = {};
	multisamplingStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisamplingStateCreateInfo.sampleShadingEnable = VK_FALSE;
	multisamplingStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisamplingStateCreateInfo.minSampleShading = 1.0f;
	multisamplingStateCreateInfo.pSampleMask = nullptr;
	multisamplingStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
	multisamplingStateCreateInfo.alphaToOneEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colourBlendAttachmentStateCreateInfo = {};
	colourBlendAttachmentStateCreateInfo.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colourBlendAttachmentStateCreateInfo.blendEnable = VK_FALSE;
	colourBlendAttachmentStateCreateInfo.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	colourBlendAttachmentStateCreateInfo.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	colourBlendAttachmentStateCreateInfo.colorBlendOp = VK_BLEND_OP_ADD;
	colourBlendAttachmentStateCreateInfo.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colourBlendAttachmentStateCreateInfo.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colourBlendAttachmentStateCreateInfo.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colourBlendStateCreateInfo = {};
	colourBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colourBlendStateCreateInfo.logicOpEnable = VK_FALSE;
	colourBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
	colourBlendStateCreateInfo.attachmentCount = 1;
	colourBlendStateCreateInfo.pAttachments = &colourBlendAttachmentStateCreateInfo;
	colourBlendStateCreateInfo.blendConstants[0] = 0.0f;
	colourBlendStateCreateInfo.blendConstants[1] = 0.0f;
	colourBlendStateCreateInfo.blendConstants[2] = 0.0f;
	colourBlendStateCreateInfo.blendConstants[3] = 0.0f;

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &m_VkDescriptorSetLayout;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
	pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

	VkResult result = ::vkCreatePipelineLayout(m_VkDevice, &pipelineLayoutCreateInfo, nullptr, &m_VkPipelineLayout);

	if (result != VK_SUCCESS)
	{
		::OutputDebugString(L"Failed to create pipeline layout!");
		return false;
	}

	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphicsPipelineCreateInfo.stageCount = 2;
	graphicsPipelineCreateInfo.pStages = shaderStages;
	graphicsPipelineCreateInfo.pVertexInputState = &vertexInputStageCreateInfo;
	graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyStageCreateInfo;
	graphicsPipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
	graphicsPipelineCreateInfo.pRasterizationState = &rasterizerStageCreateInfo;
	graphicsPipelineCreateInfo.pMultisampleState = &multisamplingStateCreateInfo;
	graphicsPipelineCreateInfo.pDepthStencilState = nullptr;
	graphicsPipelineCreateInfo.pColorBlendState = &colourBlendStateCreateInfo;
	graphicsPipelineCreateInfo.pDynamicState = nullptr;
	graphicsPipelineCreateInfo.layout = m_VkPipelineLayout;
	graphicsPipelineCreateInfo.renderPass = m_VkRenderPass;
	graphicsPipelineCreateInfo.subpass = 0;
	graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	graphicsPipelineCreateInfo.basePipelineIndex = -1;

	result = ::vkCreateGraphicsPipelines(m_VkDevice, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr, &m_VkGraphicsPipeline);

	if (result != VK_SUCCESS)
	{
		::OutputDebugString(L"Failed to create Graphics Pipeline!");
		return false;
	}

	::vkDestroyShaderModule(m_VkDevice, fragmentShaderModule, nullptr);
	::vkDestroyShaderModule(m_VkDevice, vertexShaderModule, nullptr);

	return true;
}

bool VulkanInstance::CreateFramebuffers()
{
	m_VkFramebuffers.resize(m_SwapChainImageViews.size());

	VkResult result = VK_NOT_READY;

	for (size_t i = 0; i < m_SwapChainImageViews.size(); i++)
	{
		VkImageView attachments[] = {
			m_SwapChainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferCreateInfo = {};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.renderPass = m_VkRenderPass;
		framebufferCreateInfo.attachmentCount = 1;
		framebufferCreateInfo.pAttachments = attachments;
		framebufferCreateInfo.width = m_VkSwapChainExtent2D.width;
		framebufferCreateInfo.height = m_VkSwapChainExtent2D.height;
		framebufferCreateInfo.layers = 1;

		result = ::vkCreateFramebuffer(m_VkDevice, &framebufferCreateInfo, nullptr, &m_VkFramebuffers[i]);

		if (result != VK_SUCCESS)
		{
			::OutputDebugString(L"Failed to create SwapChain Framebuffer!");
			return false;
		}
	}

	return true;
}

bool VulkanInstance::CreateCommandPool()
{
	QueueFamilyIndices queueFamilyInidices = FindQueueFamilies(m_VkPhysicalDevice);

	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.queueFamilyIndex = queueFamilyInidices.GraphicsFamily.value();
	commandPoolCreateInfo.flags = 0;

	VkResult result = ::vkCreateCommandPool(m_VkDevice, &commandPoolCreateInfo, nullptr, &m_VkCommandPool);

	if (result != VK_SUCCESS)
	{
		::OutputDebugString(L"Failed to create CommandPool!");
		return false;
	}

	return true;
}

bool VulkanInstance::CreateBuffer(VkDeviceSize bufferSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags propertyFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = bufferSize;
	bufferCreateInfo.usage = usageFlags;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (::vkCreateBuffer(m_VkDevice, &bufferCreateInfo, nullptr, &buffer) != VK_SUCCESS)
	{
		::OutputDebugString(L"Failed to create buffer!");
		return false;
	}

	VkMemoryRequirements memoryRequirements = {};
	::vkGetBufferMemoryRequirements(m_VkDevice, buffer, &memoryRequirements);

	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, propertyFlags);

	if (::vkAllocateMemory(m_VkDevice, &memoryAllocateInfo, nullptr, &bufferMemory) != VK_SUCCESS)
	{
		::OutputDebugString(L"Failed to allocate buffer memory!");
		return false;
	}

	if (::vkBindBufferMemory(m_VkDevice, buffer, bufferMemory, 0) != VK_SUCCESS)
	{
		::OutputDebugString(L"Failed to bind buffer memory!");
		return false;
	}

	return true;
}

bool VulkanInstance::CreateVertexBuffer()
{
	VkDeviceSize bufferSize = sizeof(m_Verticies[0]) * m_Verticies.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* vertexData = nullptr;

	if (::vkMapMemory(m_VkDevice, stagingBufferMemory, 0, bufferSize, 0, &vertexData) != VK_SUCCESS)
	{
		::OutputDebugString(L"Failed to map vertex buffer memory!");
		return false;
	}

	::memcpy(vertexData, m_Verticies.data(), (size_t)bufferSize);
	::vkUnmapMemory(m_VkDevice, stagingBufferMemory);

	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VkVertexBuffer, m_VertexBufferMemory);

	CopyBuffer(stagingBuffer, m_VkVertexBuffer, bufferSize);

	::vkDestroyBuffer(m_VkDevice, stagingBuffer, nullptr);
	::vkFreeMemory(m_VkDevice, stagingBufferMemory, nullptr);

	return true;
}

bool VulkanInstance::CreateIndexBuffer()
{
	VkDeviceSize bufferSize = sizeof(m_Indicies[0]) * m_Indicies.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	
	if (::vkMapMemory(m_VkDevice, stagingBufferMemory, 0, bufferSize, 0, &data) != VK_SUCCESS)
	{
		::OutputDebugString(L"Failed to map index buffer memory!");
		return false;
	}

	::memcpy(data, m_Indicies.data(), (size_t)bufferSize);
	::vkUnmapMemory(m_VkDevice, stagingBufferMemory);

	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VkIndexBuffer, m_VkIndexBufferMemory);
	
	CopyBuffer(stagingBuffer, m_VkIndexBuffer, bufferSize);

	::vkDestroyBuffer(m_VkDevice, stagingBuffer, nullptr);
	::vkFreeMemory(m_VkDevice, stagingBufferMemory, nullptr);

	return true;
}

bool VulkanInstance::CreateUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	m_VkUniformBuffers.resize(m_SwapChainImages.size());
	m_VkUniformBuffersMemory.resize(m_SwapChainImages.size());

	for (size_t i = 0; i < m_SwapChainImages.size(); i++)
	{
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_VkUniformBuffers[i], m_VkUniformBuffersMemory[i]);
	}

	return true;
}

bool VulkanInstance::CreateDescriptorPool()
{
	VkDescriptorPoolSize poolSize = {};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = static_cast<uint32_t>(m_SwapChainImages.size());

	VkDescriptorPoolCreateInfo createDescriptorPoolInfo = {};
	createDescriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	createDescriptorPoolInfo.poolSizeCount = 1;
	createDescriptorPoolInfo.pPoolSizes = &poolSize;
	createDescriptorPoolInfo.maxSets = static_cast<uint32_t>(m_SwapChainImages.size());

	if (::vkCreateDescriptorPool(m_VkDevice, &createDescriptorPoolInfo, nullptr, &m_VkDescriptorPool) != VK_SUCCESS)
	{
		::OutputDebugString(L"Failed to create descriptor pool!");
		return false;
	}

	return true;
}

bool VulkanInstance::CreateDescriptorSets()
{
	std::vector<VkDescriptorSetLayout> layouts(m_SwapChainImages.size(), m_VkDescriptorSetLayout);

	VkDescriptorSetAllocateInfo descriptorAllocateInfo = {};
	descriptorAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorAllocateInfo.descriptorPool = m_VkDescriptorPool;
	descriptorAllocateInfo.descriptorSetCount = static_cast<uint32_t>(m_SwapChainImages.size());
	descriptorAllocateInfo.pSetLayouts = layouts.data();

	m_VkDescriptorSets.resize(m_SwapChainImages.size());

	if (::vkAllocateDescriptorSets(m_VkDevice, &descriptorAllocateInfo, m_VkDescriptorSets.data()) != VK_SUCCESS)
	{
		::OutputDebugString(L"");
		return false;
	}

	for (size_t i = 0; i < m_SwapChainImages.size(); i++)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = m_VkUniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_VkDescriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr;
		descriptorWrite.pTexelBufferView = nullptr;

		::vkUpdateDescriptorSets(m_VkDevice, 1, &descriptorWrite, 0, nullptr);
	}

	return true;
}

void VulkanInstance::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize bufferSize)
{
	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandPool = m_VkCommandPool;
	commandBufferAllocateInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	::vkAllocateCommandBuffers(m_VkDevice, &commandBufferAllocateInfo, &commandBuffer);

	VkCommandBufferBeginInfo commandBufferBeginInfo = {};
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	::vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);

	VkBufferCopy bufferCopy = {};
	bufferCopy.srcOffset = 0;
	bufferCopy.dstOffset = 0;
	bufferCopy.size = bufferSize;

	::vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &bufferCopy);
	::vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	::vkQueueSubmit(m_VkGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	::vkQueueWaitIdle(m_VkGraphicsQueue);

	::vkFreeCommandBuffers(m_VkDevice, m_VkCommandPool, 1, &commandBuffer);
}

void VulkanInstance::UpdateUniformBuffer(uint32_t currentImageIdx, const float& deltaTime)
{
	UniformBufferObject uniformBufferObj = {};

	uniformBufferObj.Model = MathLib::Rotate(Matrix4f::Identity(), 0, Vector3f(0.0f, 0.0f, 1.0f));
	
	uniformBufferObj.View = Matrix4f::Identity(); //MathLib::LookAtRH(Vector3f(5.0f, 5.0f, 5.0f), Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f));

	uniformBufferObj.Projection = Matrix4f::Identity(); //MathLib::PerspectiveFoVRH(MathLib::DegreesToRadians(45.0f), m_VkSwapChainExtent2D.width / (float)m_VkSwapChainExtent2D.height, 0.1f, 10.0f);

	//uniformBufferObj.Projection[1][1] *= -1; //TODO: Is this needed?

	void* data;
	::vkMapMemory(m_VkDevice, m_VkUniformBuffersMemory[currentImageIdx], 0, sizeof(uniformBufferObj), 0, &data);

	::memcpy(data, &uniformBufferObj, sizeof(uniformBufferObj));
	::vkUnmapMemory(m_VkDevice, m_VkUniformBuffersMemory[currentImageIdx]);
}

bool VulkanInstance::CreateCommandBuffers()
{
	m_VkCommandBuffers.resize(m_VkFramebuffers.size());

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = m_VkCommandPool;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = (uint32_t)m_VkCommandBuffers.size();

	VkResult result = ::vkAllocateCommandBuffers(m_VkDevice, &commandBufferAllocateInfo, m_VkCommandBuffers.data());

	if (result != VK_SUCCESS)
	{
		::OutputDebugString(L"Failed to allocate Command Buffers!");
		return false;
	}

	for (size_t i = 0; i < m_VkCommandBuffers.size(); i++)
	{
		VkCommandBufferBeginInfo commandBufferBeginInfo = {};
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.flags = 0;
		commandBufferBeginInfo.pInheritanceInfo = nullptr;

		result = ::vkBeginCommandBuffer(m_VkCommandBuffers[i], &commandBufferBeginInfo);

		if (result != VK_SUCCESS)
		{
			::OutputDebugString(L"Failed to begin recording command buffer!");
			return false;
		}

		VkClearValue clearColour = { 0.0f, 0.0f, 0.0f, 1.0f };

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = m_VkRenderPass;
		renderPassBeginInfo.framebuffer = m_VkFramebuffers[i];
		renderPassBeginInfo.renderArea.offset = { 0, 0 };
		renderPassBeginInfo.renderArea.extent = m_VkSwapChainExtent2D;
		renderPassBeginInfo.clearValueCount = 1;
		renderPassBeginInfo.pClearValues = &clearColour;

		::vkCmdBeginRenderPass(m_VkCommandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		::vkCmdBindPipeline(m_VkCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_VkGraphicsPipeline);

		VkBuffer vertexBuffers[] = { m_VkVertexBuffer };
		VkDeviceSize offsets[] = { 0 };

		::vkCmdBindVertexBuffers(m_VkCommandBuffers[i], 0, 1, vertexBuffers, offsets);

		::vkCmdBindIndexBuffer(m_VkCommandBuffers[i], m_VkIndexBuffer, 0, VK_INDEX_TYPE_UINT16);

		//::vkCmdDraw(m_VkCommandBuffers[i], static_cast<uint32_t>(m_Verticies.size()), 1, 0, 0);

		::vkCmdBindDescriptorSets(m_VkCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_VkPipelineLayout, 0, 1, &m_VkDescriptorSets[i], 0, nullptr);
		::vkCmdDrawIndexed(m_VkCommandBuffers[i], static_cast<uint32_t>(m_Indicies.size()), 1, 0, 0, 0);

		::vkCmdEndRenderPass(m_VkCommandBuffers[i]);

		result = ::vkEndCommandBuffer(m_VkCommandBuffers[i]);

		if (result != VK_SUCCESS)
		{
			::OutputDebugString(L"Failed to record Command Buffer!");
			return false;
		}
	}

	return true;
}

bool VulkanInstance::CreateSyncObjects()
{
	m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	m_ImagesInFlight.resize(m_SwapChainImages.size(), VK_NULL_HANDLE);

	m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	m_RenderCompleteSemaphores.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (::vkCreateSemaphore(m_VkDevice, &semaphoreCreateInfo, nullptr, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
			::vkCreateSemaphore(m_VkDevice, &semaphoreCreateInfo, nullptr, &m_RenderCompleteSemaphores[i]) != VK_SUCCESS ||
			::vkCreateFence(m_VkDevice, &fenceCreateInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS)
		{
			::OutputDebugString(L"Failed to create sync objects!");
			return false;
		}
	}

	return true;
}

bool VulkanInstance::InitDearImGui(HWND mainWindowHandle)
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	QueueFamilyIndices queueFamilyInidices = FindQueueFamilies(m_VkPhysicalDevice);

	/*
	VkDescriptorPool imGuiDescriptorPool = VK_NULL_HANDLE;

	VkDescriptorPoolSize descriptorPoolSizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};

	VkDescriptorPoolCreateInfo createDescriptorPoolInfo = {};
	createDescriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	createDescriptorPoolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	createDescriptorPoolInfo.maxSets = 1000 * IM_ARRAYSIZE(descriptorPoolSizes);
	createDescriptorPoolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(descriptorPoolSizes);
	createDescriptorPoolInfo.pPoolSizes = descriptorPoolSizes;

	VkResult result = ::vkCreateDescriptorPool(m_VkDevice, &createDescriptorPoolInfo, nullptr, &imGuiDescriptorPool);

	if (result != VK_SUCCESS)
	{
		::OutputDebugString(L"Failed to create ImGUI Descriptor Pool");
		return false;
	}
	*/

	ImGui_ImplVulkan_InitInfo initDearImGuiVulkan = {};
	initDearImGuiVulkan.Instance = m_VkInstance;
	initDearImGuiVulkan.PhysicalDevice = m_VkPhysicalDevice;
	initDearImGuiVulkan.Device = m_VkDevice;
	initDearImGuiVulkan.QueueFamily = queueFamilyInidices.GraphicsFamily.value();
	initDearImGuiVulkan.Queue = m_VkGraphicsQueue;
	initDearImGuiVulkan.PipelineCache = VK_NULL_HANDLE;
	initDearImGuiVulkan.DescriptorPool = m_VkDescriptorPool; //imGuiDescriptorPool;
	initDearImGuiVulkan.MinImageCount = static_cast<uint32_t>(m_SwapChainImages.size());
	initDearImGuiVulkan.ImageCount = static_cast<uint32_t>(m_SwapChainImages.size());;
	initDearImGuiVulkan.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

	if (::ImGui_ImplVulkan_Init(&initDearImGuiVulkan, m_VkRenderPass) == false)
	{
		::OutputDebugString(L"Failed to init Dear ImGui VK Impl");
		return false;
	}

	if (::ImGui_ImplWin32_Init(&mainWindowHandle) == false)
	{
		::OutputDebugString(L"Failed to init Dear ImGui Win32 Impl");
		return false;
	}

	return true;
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
	if (surfaceCapabilities.currentExtent.width != UINT32_MAX && surfaceCapabilities.currentExtent.width > 0 && surfaceCapabilities.currentExtent.height > 0)
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

uint32_t VulkanInstance::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags)
{
	VkPhysicalDeviceMemoryProperties memoryProperties = {};
	::vkGetPhysicalDeviceMemoryProperties(m_VkPhysicalDevice, &memoryProperties);

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags))
		{
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type!");
}

VkShaderModule VulkanInstance::CreateShaderModule(const std::vector<char>& bytecode)
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

	createInfo.codeSize = bytecode.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(bytecode.data());

	VkShaderModule shaderModule = nullptr;

	VkResult result = ::vkCreateShaderModule(m_VkDevice, &createInfo, nullptr, &shaderModule);

	if (result != VK_SUCCESS)
	{
		::OutputDebugString(L"Failed to create shader module!");
		return nullptr;
	}

	return shaderModule;
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

void VulkanInstance::InitialiseDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& debugUtilsMessengerCreateInfo)
{
	debugUtilsMessengerCreateInfo = {};
	debugUtilsMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

	debugUtilsMessengerCreateInfo.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	debugUtilsMessengerCreateInfo.messageType =
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	debugUtilsMessengerCreateInfo.pfnUserCallback = DebugCallback;
	debugUtilsMessengerCreateInfo.pUserData = nullptr;
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
