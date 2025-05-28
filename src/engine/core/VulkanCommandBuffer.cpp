#include "VulkanCommandBuffer.hpp"

#include "spdlog/spdlog.h"
#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>

namespace rr
{

/**
 *  Construct a single CommandBuffer
 */
VulkanCommandBuffer::VulkanCommandBuffer(VkDevice device, VkCommandPool commandPool, VkCommandBufferLevel level)
    : m_commandBuffer(VK_NULL_HANDLE)
{
    VkCommandBufferAllocateInfo allocInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = commandPool,
        .level = level,
        .commandBufferCount = 1
    };

    if(vkAllocateCommandBuffers(device, &allocInfo, &m_commandBuffer) != VK_SUCCESS)
    {
        spdlog::critical("Failure while creating command buffers");
        throw std::runtime_error("Failed to create command buffers");
    }
}

/**
 *  Helper constructor for the create function
 */
VulkanCommandBuffer::VulkanCommandBuffer(VkCommandBuffer commandBuffer)
    : m_commandBuffer(commandBuffer)
{}

/**
 * Factory method to create <code>count<\code> of <code>VulkanCommandBuffer<\code>
 *
 * @param device - VkDevice from where the buffers are located on
 * @param commandPool - VkCommandPool where the buffers are allocated in
 * @param cound - amount of buffers to allocate
 * @param level - level of the buffers. Default: VK_COMMAND_BUFFER_LEVEL_PRIMARY
 * @return vector with the allocated VulkanCommandBuffers
*/
std::vector<std::unique_ptr<VulkanCommandBuffer>> VulkanCommandBuffer::create(VkDevice device, VkCommandPool commandPool, std::uint32_t count, VkCommandBufferLevel level)
{
    std::vector<VkCommandBuffer> rawBuffers(count);

    VkCommandBufferAllocateInfo allocInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = commandPool,
        .level = level,
        .commandBufferCount = count
    };

    if(vkAllocateCommandBuffers(device, &allocInfo, rawBuffers.data()) != VK_SUCCESS)
    {
        spdlog::critical("Failure while creating command buffers");
        throw std::runtime_error("Failed to create command buffers");
    }

    std::vector<std::unique_ptr<VulkanCommandBuffer>> wrapped;
    for(const auto& buffer : rawBuffers)
        wrapped.push_back(std::make_unique<VulkanCommandBuffer>(buffer));

    return wrapped;
}

} // !rr
