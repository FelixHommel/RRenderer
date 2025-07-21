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

/// \brief Construct a new \ref VulkanCommandPool
///
/// \param device VulkanDevice& where the command pool is on
///
/// \throws \ref VulkanException if \p device has no queue of the graphics family
VulkanCommandPool::VulkanCommandPool(VulkanDevice& device)
    : device(device)
{
    auto [graphicsFamily,_]{ device.findPhysicalQueueFamilies() };
    if(!graphicsFamily.has_value())
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::QUEUE_FAMILY_INDEX_IS_EMPTY);

    const VkCommandPoolCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = graphicsFamily.value()
    };

    if(vkCreateCommandPool(device.getHandle(), &createInfo, nullptr, &m_commandPool) != VK_SUCCESS)
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::CREATE_COMMAND_POOL);

    spdlog::info("Command pool created successfully...");
}

VulkanCommandPool::~VulkanCommandPool()
{
    vkDestroyCommandPool(device.getHandle(), m_commandPool, nullptr);
}

/// \brief Allocate a single \ref VulkanCommandBuffer
///
/// \return std::unique_ptr<VulkanCommandBuffer> the newly allocated buffer
/// \sa VulkanCommandBuffer
std::unique_ptr<VulkanCommandBuffer> VulkanCommandPool::allocateCommandBuffer()
{
    return std::make_unique<VulkanCommandBuffer>(device.getHandle(), m_commandPool);
}

/// \brief Allocate \p count of \ref VulkanCommandBuffer
///
/// \return std::vector of std::unique_ptr<VulkanCommandBuffer>
/// \sa VulkanCommandBuffer
std::vector<std::unique_ptr<VulkanCommandBuffer>> VulkanCommandPool::allocateCommandBuffer(const std::uint32_t count) const
{
    return VulkanCommandBuffer::create(device.getHandle(), m_commandPool, count);
}

} // !rr
