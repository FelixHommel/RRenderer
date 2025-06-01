#include "VulkanCommandPool.hpp"

#include "core/VulkanCommandBuffer.hpp"
#include "core/VulkanDevice.hpp"

#include "exception/EngineException.hpp"
#include "exception/VulkanException.hpp"
#include "spdlog/spdlog.h"
#include <source_location>
#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <memory>
#include <vector>

namespace rr
{

VulkanCommandPool::VulkanCommandPool(VulkanDevice& device)
    : device(device)
{
    QueueFamilyIndices indices{ device.findPhysicalQueueFamilies() };
    if(!indices.graphicsFamily.has_value())
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::QUEUE_FAMILY_INDEX_IS_EMPTY);

    VkCommandPoolCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = indices.graphicsFamily.value()
    };

    if(vkCreateCommandPool(device.getHandle(), &createInfo, nullptr, &m_commandPool) != VK_SUCCESS)
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::CREATE_COMMAND_POOL);

    spdlog::info("Command pool created successfully...");
}

VulkanCommandPool::~VulkanCommandPool()
{
    vkDestroyCommandPool(device.getHandle(), m_commandPool, nullptr);
}

std::unique_ptr<VulkanCommandBuffer> VulkanCommandPool::allocateCommandBuffer()
{
    return std::make_unique<VulkanCommandBuffer>(device.getHandle(), m_commandPool);
}

std::vector<std::unique_ptr<VulkanCommandBuffer>> VulkanCommandPool::allocateCommandBuffer(std::uint32_t count)
{
    return VulkanCommandBuffer::create(device.getHandle(), m_commandPool, count);
}

} // !rr
