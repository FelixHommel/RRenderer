#ifndef RRENDERER_ENGINE_CORE_VULKAN_DEVICE_HPP
#define RRENDERER_ENGINE_CORE_VULKAN_DEVICE_HPP

#include "exception/VulkanException.hpp"
#include "exception/EngineException.hpp"

#include <vulkan/vulkan_core.h>

#include <array>
#include <cstdint>
#include <optional>
#include <set>
#include <source_location>
#include <vector>

namespace rr
{

struct SwapchainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices
{
    std::optional<std::uint32_t> graphicsFamily;
    std::optional<std::uint32_t> presentFamily;

    [[nodiscard]] constexpr bool isComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
    [[nodiscard]] constexpr bool areSameQueue() const
    {
        if(graphicsFamily.has_value() && presentFamily.has_value())
            return graphicsFamily.value() == presentFamily.value();
        
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::QUEUE_FAMILY_INDEX_IS_EMPTY);

    }

    [[nodiscard]] std::set<std::uint32_t> getUniqueFamilies() const { return { graphicsFamily.value_or(0), presentFamily.value_or(0) }; }
    [[nodiscard]] constexpr std::array<std::uint32_t, 2> toAray() const
    {
        if(graphicsFamily.has_value() && presentFamily.has_value())
            return { graphicsFamily.value(), presentFamily.value() };

        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::QUEUE_FAMILY_INDEX_IS_EMPTY);
    }
};

/*
 *  <code>VulkanDevice<\code> is a wrapper around <code>VkDevice<\code>, <code>VkPhysicalDevice<\code> and also
 *  manages the queues.
 *
 *  @author Felix Hommel
 *  @date 5/26/2025
*/ 
class VulkanDevice
{
public:
    VulkanDevice(VkInstance instance, VkSurfaceKHR surface);
    ~VulkanDevice();

    VulkanDevice(const VulkanDevice&) = delete;
    VulkanDevice(VulkanDevice&&) = delete;
    VulkanDevice& operator=(const VulkanDevice&) = delete;
    VulkanDevice& operator=(VulkanDevice&&) = delete;

    [[nodiscard]] VkDevice getHandle() const { return m_device; }
    [[nodiscard]] VkQueue getGraphicsQueueHandle() const { return m_graphicsQueue; }
    [[nodiscard]] VkQueue getPresentQueueHandle() const { return m_presentQueue; }

    [[nodiscard]] SwapchainSupportDetails getSwapchainSupport() const { return querySwapchainSupport(m_physicalDevice); }
    [[nodiscard]] QueueFamilyIndices findPhysicalQueueFamilies() const { return findQueueFamilies(m_physicalDevice); }
    [[nodiscard]] VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    void createImageWithInfo(const VkImageCreateInfo& createInfo, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

private:
    VkInstance instance;
    VkSurfaceKHR surface;

    VkPhysicalDevice m_physicalDevice{ VK_NULL_HANDLE };
    VkPhysicalDeviceProperties m_physicalDeviceProperties{};
    VkDevice m_device{ VK_NULL_HANDLE };
    VkQueue m_graphicsQueue{ VK_NULL_HANDLE };
    VkQueue m_presentQueue{ VK_NULL_HANDLE };

    const std::vector<const char*> deviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    void pickPhyscialDevice();
    void createLogicalDevice();

    bool isDeviceSuitable(VkPhysicalDevice device) const;
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
    bool checkDeviceExtensionsSupported(VkPhysicalDevice device) const;
    SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device) const;
    std::uint32_t findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

} // !rr

#endif // !RRENDERER_ENGINE_CORE_VULKAN_DEVICE_HPP
