#ifndef RRENDERER_ENGINE_CORE_VULKAN_COMMAND_BUFFER_HPP
#define RRENDERER_ENGINE_CORE_VULKAN_COMMAND_BUFFER_HPP

#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <memory>
#include <vector>

namespace rr
{

/// \brief \ref VulkanCommandBuffer is a wrapper around VkCommandBuffer
///
/// \ref VulkanCommandBuffer is managing a VkCommandBuffer handle provides access to it and is capable of creating
/// new VulkanCommandBuffers.
///
/// \author Felix Hommel
/// \date 5/26/2025
class VulkanCommandBuffer
{
public:
    VulkanCommandBuffer(VkDevice device, VkCommandPool commandPool, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    explicit VulkanCommandBuffer(VkDevice device, VkCommandPool commandPool, VkCommandBuffer commandBuffer);
    ~VulkanCommandBuffer();

    VulkanCommandBuffer(const VulkanCommandBuffer&) = delete;
    VulkanCommandBuffer& operator=(const VulkanCommandBuffer&) = delete;
    VulkanCommandBuffer(VulkanCommandBuffer&&) = delete;
    VulkanCommandBuffer& operator=(VulkanCommandBuffer&&) = delete;

    static std::vector<std::unique_ptr<VulkanCommandBuffer>> create(VkDevice device, VkCommandPool commandPool, std::uint32_t count, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    [[nodiscard]] VkCommandBuffer& getHandleRef() { return m_commandBuffer; }

private:
    VkDevice device;
    VkCommandPool commandPool;

    VkCommandBuffer m_commandBuffer;
};

} // !rr

#endif // !RRENDERER_ENGINE_CORE_VULKAN_COMMAND_BUFFER_HPP
