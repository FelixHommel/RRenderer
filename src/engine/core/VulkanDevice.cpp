#include "VulkanDevice.hpp"

#include "constants.hpp"

#include "exception/EngineException.hpp"
#include "exception/VulkanException.hpp"
#include "spdlog/spdlog.h"
#include <source_location>
#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <cstring>
#include <set>
#include <string>
#include <vector>

namespace rr
{

VulkanDevice::VulkanDevice(VkInstance instance, VkSurfaceKHR surface)
    : instance(instance)
    , surface(surface)
{
    pickPhyscialDevice();
    createLogicalDevice();
}

VulkanDevice::~VulkanDevice()
{
    vkDestroyDevice(m_device, nullptr);
}

VkFormat VulkanDevice::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
    for(const auto format : candidates)
    {
        VkFormatProperties properties;
        vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &properties);

        if(tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features ||
            tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::FIND_SUPPORTED_FORMAT);
}

void VulkanDevice::createImageWithInfo(const VkImageCreateInfo& createInfo, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
    if(vkCreateImage(m_device, &createInfo, nullptr, &image) != VK_SUCCESS)
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::CREATE_IMAGE);

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(m_device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memRequirements.size,
        .memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties)
    };

    if(vkAllocateMemory(m_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::ALLOCATE_MEMORY);

    if(vkBindImageMemory(m_device, image, imageMemory, 0) != VK_SUCCESS)
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::BIND_IMAGE_MEMORY);
}

void VulkanDevice::pickPhyscialDevice()
{
    std::uint32_t deviceCount{0};
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if(deviceCount == 0)
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::NO_PHYSICAL_DEVICE_FOUND);

    spdlog::info("Found {} vulkan compatible device(s)", deviceCount);

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for(const auto& dev : devices)
    {
        if(isDeviceSuitable(dev))
        {
            m_physicalDevice = dev;
            break;
        }
    }

    if(m_physicalDevice == VK_NULL_HANDLE)
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::NO_SUITABLE_DEVICE_FOUND);

    vkGetPhysicalDeviceProperties(m_physicalDevice, &m_physicalDeviceProperties);
    spdlog::info("physical device: {}\n\tID: {}\n\tvendorID: {}\n\tdeviceType: {}",
                 m_physicalDeviceProperties.deviceName,
                 m_physicalDeviceProperties.deviceID,
                 m_physicalDeviceProperties.vendorID,
                 static_cast<int>(m_physicalDeviceProperties.deviceType));
}

void VulkanDevice::createLogicalDevice()
{
    QueueFamilyIndices indices{ findQueueFamilies(m_physicalDevice) };

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<std::uint32_t> uniqueQueueFamilies{ indices.getUniqueFamilies() };

    float queuePriority{ 1.f };
    for(auto queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = queueFamily,
            .queueCount = 1,
            .pQueuePriorities = &queuePriority
        };

        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{
        .samplerAnisotropy = VK_TRUE
    };

    VkDeviceCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = static_cast<std::uint32_t>(queueCreateInfos.size()),
        .pQueueCreateInfos = queueCreateInfos.data(),
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = static_cast<std::uint32_t>(deviceExtensions.size()),
        .ppEnabledExtensionNames = deviceExtensions.data(),
        .pEnabledFeatures = &deviceFeatures
    };

    if(useValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<std::uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }

    if(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::CREATE_DEVICE);

    if(indices.graphicsFamily.has_value() && indices.presentFamily.has_value())
    {
        vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
        vkGetDeviceQueue(m_device, indices.presentFamily.value(), 0, &m_presentQueue);
    } else
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::QUEUE_FAMILY_INDEX_IS_EMPTY);

    spdlog::info("Logical device created successfully...");
}

bool VulkanDevice::isDeviceSuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices{ findQueueFamilies(device) };
    bool extensionsSupported{ checkDeviceExtensionsSupported(device) };
    bool swapchainSuitable{ false };

    if(extensionsSupported)
    {
        SwapchainSupportDetails swapchainSupport{ querySwapchainSupport(device) };
        swapchainSuitable = !swapchainSupport.formats.empty() && !swapchainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapchainSuitable && static_cast<bool>(supportedFeatures.samplerAnisotropy);
}

QueueFamilyIndices VulkanDevice::findQueueFamilies(VkPhysicalDevice device) const
{
    QueueFamilyIndices indices;

    std::uint32_t queueFamilyCount{0};
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i{0};
    for(const auto& queueFamily : queueFamilies)
    {
        if(queueFamily.queueCount > 0 && static_cast<bool>(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT))
            indices.graphicsFamily.emplace(i);

        VkBool32 presentSupport{ VK_FALSE };
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if(queueFamily.queueCount > 0 && static_cast<bool>(presentSupport))
            indices.presentFamily.emplace(i);

        if(indices.isComplete())
            break;

        ++i;
    }

    return indices;
}

bool VulkanDevice::checkDeviceExtensionsSupported(VkPhysicalDevice device) const
{
    std::uint32_t extensionCount{0};
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for(const auto& extension : availableExtensions)
        requiredExtensions.erase(extension.extensionName);

    return requiredExtensions.empty();
}

SwapchainSupportDetails VulkanDevice::querySwapchainSupport(VkPhysicalDevice device) const
{
    SwapchainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    std::uint32_t formatCount{0};
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if(formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    std::uint32_t presentModeCount{0};
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if(presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

std::uint32_t VulkanDevice::findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

    for(std::uint32_t i{0}; i < memProperties.memoryTypeCount; ++i)
    {
        if(static_cast<bool>((typeFilter & (1 << i))) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) //NOLINT
            return i;
    }

    throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::NO_SUITABLE_MEMORY_TYPE_FOUND);
}

}
