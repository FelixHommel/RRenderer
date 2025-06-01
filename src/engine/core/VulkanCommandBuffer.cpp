#include "VulkanCommandBuffer.hpp"

#include "exception/EngineException.hpp"
#include "exception/VulkanException.hpp"
#include <source_location>
#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <memory>
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
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::ALLOCATE_COMMAND_BUFFERS);
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
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::ALLOCATE_COMMAND_BUFFERS);

    std::vector<std::unique_ptr<VulkanCommandBuffer>> wrapped;
    for(const auto& buffer : rawBuffers)
        wrapped.push_back(std::make_unique<VulkanCommandBuffer>(buffer));

    return wrapped;
}

} // !rr
