#include "VulkanCommandPool.hpp"

#include "core/VulkanCommandBuffer.hpp"
#include "core/VulkanDevice.hpp"

#include "spdlog/spdlog.h"

#include <cstdint>
#include <memory>
#include <stdexcept>

namespace rr
{

VulkanCommandPool::VulkanCommandPool(VulkanDevice& device)
    : device(device)
    , m_commandPool(VK_NULL_HANDLE)
{
    QueueFamilyIndices indices{ device.findPhysicalQueueFamilies() };

    VkCommandPoolCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = indices.graphicsFamily.value()
    };

    if(vkCreateCommandPool(device.getHandle(), &createInfo, nullptr, &m_commandPool) != VK_SUCCESS)
    {
        spdlog::critical("Failure while creating command pool");
        throw std::runtime_error("Failed to create command pool");
    }

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
