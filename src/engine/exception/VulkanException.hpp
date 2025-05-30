#ifndef RRENDERER_ENGINE_EXCEPTION_VULKAN_EXCEPTION_HPP
#define RRENDERER_ENGINE_EXCEPTION_VULKAN_EXCEPTION_HPP

#include "exception/EngineException.hpp"

#include <cstddef>
#include <string>

namespace rr
{

enum class VulkanExceptionCause
{
    CREATE_INSTANCE,
    IMAGE_ACQUISITION,
    SUBMIT_COMMAND_BUFFER,
    BEGIN_RECORD_COMMAND_BUFFER,
    END_RECORD_COMMAND_BUFFER,
    ALLOCATE_COMMAND_BUFFERS,
    QUEUE_FAMILY_INDEX_IS_EMPTY,
    CREATE_COMMAND_POOL,
    CREATE_DEBUG_MESSENGER,
    FIND_SUPPORTED_FORMAT,
    CREATE_IMAGE,
    ALLOCATE_MEMORY,
    BIND_IMAGE_MEMORY,
    NO_PHYSICAL_DEVICE_FOUND,
    NO_SUITABLE_DEVICE_FOUND,
    CREATE_DEVICE,
    NO_SUITABLE_MEMORY_TYPE_FOUND,
};

class VulkanException : public EngineException
{
public:
    VulkanException(VulkanExceptionCause cause)
        : EngineException("Vulkan error occurd during " + causeToString(cause))
        , m_cause(cause)
    {}

    VulkanException(VulkanExceptionCause cause, std::size_t index)
        : EngineException("Vulkan error occurd during " + causeToString(cause, index))
        , m_cause(cause)
    {}

    [[nodiscard]] VulkanExceptionCause cause() const { return m_cause; }

private:
    VulkanExceptionCause m_cause;

    std::string causeToString(VulkanExceptionCause cause)
    {
        switch(cause)
        {
            case VulkanExceptionCause::CREATE_INSTANCE: return "creation of a VkInstance";
            case VulkanExceptionCause::SUBMIT_COMMAND_BUFFER: return "submission of a VkCommandBuffer";
            case VulkanExceptionCause::IMAGE_ACQUISITION: return "the acquisition of the next swapchain image";
            case VulkanExceptionCause::BEGIN_RECORD_COMMAND_BUFFER: return "begin recording of VkCommandBuffer #";
            case VulkanExceptionCause::END_RECORD_COMMAND_BUFFER: return "ending recording of VkCommandBuffer #";
            case VulkanExceptionCause::ALLOCATE_COMMAND_BUFFERS: return "allocating VkCommandBuffers";
            case VulkanExceptionCause::QUEUE_FAMILY_INDEX_IS_EMPTY: return "trying to get the index of a non-existing VkQueue";
            case VulkanExceptionCause::CREATE_COMMAND_POOL: return "creation of a VkCommandPool";
            case VulkanExceptionCause::CREATE_DEBUG_MESSENGER: return "creation of the VkDebugMessenger";
            case VulkanExceptionCause::FIND_SUPPORTED_FORMAT: return "search for a supported VkFormat";
            case VulkanExceptionCause::CREATE_IMAGE: return "creation of an VkImage";
            case VulkanExceptionCause::ALLOCATE_MEMORY: return "allocating memory";
            case VulkanExceptionCause::BIND_IMAGE_MEMORY: return "binding image memory";
            case VulkanExceptionCause::NO_PHYSICAL_DEVICE_FOUND: return "search for vulkan compatible VkPhysicalDevice";
            case VulkanExceptionCause::NO_SUITABLE_DEVICE_FOUND: return "search for suitable VkPhysicalDevice";
            case VulkanExceptionCause::CREATE_DEVICE: return "creation of a VkDevice";
            case VulkanExceptionCause::NO_SUITABLE_MEMORY_TYPE_FOUND: return "search for a suitable memory type";
            default: return "unknown events";
        }
    }

    std::string causeToString(VulkanExceptionCause cause, std::size_t index)
    {
        return causeToString(cause) + std::to_string(index);
    }
};

} // !rr

#endif // !RRENDERER_ENGINE_EXCEPTION_VULKAN_EXCEPTION_HPP
