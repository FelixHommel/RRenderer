#ifndef RRENDERER_ENGINE_CORE_VULKAN_COMMAND_POOL_HPP
#define RRENDERER_ENGINE_CORE_VULKAN_COMMAND_POOL_HPP

#include "core/VulkanCommandBuffer.hpp"
#include "core/VulkanDevice.hpp"

#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <memory>
#include <vector>

namespace rr
{

class VulkanCommandPool
{
public:
    explicit VulkanCommandPool(VulkanDevice& device);
    ~VulkanCommandPool();

    VulkanCommandPool(const VulkanCommandPool&) = delete;
    VulkanCommandPool(VulkanCommandPool&&) = delete;
    VulkanCommandPool& operator=(const VulkanCommandPool&) = delete;
    VulkanCommandPool& operator=(VulkanCommandPool&&) = delete;

    [[nodiscard]] VkCommandPool getHandle() const { return m_commandPool; }

    std::unique_ptr<VulkanCommandBuffer> allocateCommandBuffer();
    std::vector<std::unique_ptr<VulkanCommandBuffer>> allocateCommandBuffer(std::uint32_t count);

private:
    VulkanDevice& device;

    VkCommandPool m_commandPool{ VK_NULL_HANDLE };
};

} // !rr

#endif // !RRENDERER_ENGINE_CORE_VULKAN_COMMAND_POOL_HPP
