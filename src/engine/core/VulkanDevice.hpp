#ifndef RRENDERER_ENGINE_CORE_VULKAN_DEVICE_HPP
#define RRENDERER_ENGINE_CORE_VULKAN_DEVICE_HPP

#include "interfaces/IDevice.hpp"

#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <optional>
#include <set>
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

    [[nodiscard]] constexpr bool isComplete() const noexcept { return graphicsFamily.has_value() && presentFamily.has_value(); }

    [[nodiscard]] constexpr std::set<std::uint32_t> getUniqueFamilies() const noexcept { return { graphicsFamily.value_or(0), presentFamily.value_or(0) }; }
};

/*
 *  <code>VulkanDevice<\code> is a wrapper around <code>VkDevice<\code>, <code>VkPhysicalDevice<\code> and also
 *  manages the queues.
 *
 *  @author Felix Hommel
 *  @date 5/26/2025
*/ 
class VulkanDevice : public IDevice
{
public:
    VulkanDevice(VkInstance instance, VkSurfaceKHR surface);
    ~VulkanDevice() override;

    VulkanDevice(const VulkanDevice&) = delete;
    VulkanDevice(VulkanDevice&&) = delete;
    VulkanDevice& operator=(const VulkanDevice&) = delete;
    VulkanDevice& operator=(VulkanDevice&&) = delete;

    int createBuffer(std::size_t size, int usage) override;
    void destroyBuffer() override;
    void waitIdle() override;

private:
    VkInstance instance;
    VkSurfaceKHR surface;

    VkPhysicalDevice m_physicalDevice;
    VkPhysicalDeviceProperties m_physicalDeviceProperties;
    VkDevice m_device;
    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;

    const std::vector<const char*> deviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    void pickPhyscialDevice();
    void createLogicalDevice();

    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool checkDeviceExtensionsSupported(VkPhysicalDevice device);
    SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device);
};

} // !rr

#endif // !RRENDERER_ENGINE_CORE_VULKAN_DEVICE_HPP
