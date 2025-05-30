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
    CREATE_COMMAND_POOL
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
            case VulkanExceptionCause::CREATE_INSTANCE: return "creation of an instance";
            case VulkanExceptionCause::SUBMIT_COMMAND_BUFFER: return "command buffer submission";
            case VulkanExceptionCause::IMAGE_ACQUISITION: return "the acquisition of the next image";
            case VulkanExceptionCause::BEGIN_RECORD_COMMAND_BUFFER: return "begin recording of command buffer #";
            case VulkanExceptionCause::END_RECORD_COMMAND_BUFFER: return "ending of recording command buffer #";
            case VulkanExceptionCause::ALLOCATE_COMMAND_BUFFERS: return "allocating command buffers";
            case VulkanExceptionCause::QUEUE_FAMILY_INDEX_IS_EMPTY: return "trying to get the index of a non-existing queue";
            case VulkanExceptionCause::CREATE_COMMAND_POOL: return "creation of a command pool";
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
