#include <iostream>
#include <vulkan\vulkan.hpp>

int main()
{
    uint32_t extensionCount = 0;
    ::vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);
    ::vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "Available Extensions:" << std::endl;

    for (const auto& extension : extensions)
    {
        std::cout << "\t" << extension.extensionName << std::endl;
    }

    uint32_t layerCount = 0;
    ::vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> layers(layerCount);
    ::vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

    std::cout << "\nAvailable Layers:" << std::endl;

    for (const auto& layer : layers)
    {
        std::cout << "\t" << layer.layerName << std::endl;
    }
}
