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
    END_RECORD_COMMAND_BUFFER
};

std::string toString(VulkanExceptionCause cause)
{
    switch(cause)
    {
        case VulkanExceptionCause::CREATE_INSTANCE: return "instance creation";
        case VulkanExceptionCause::SUBMIT_COMMAND_BUFFER: return "command buffer submission";
        case VulkanExceptionCause::IMAGE_ACQUISITION: return "the acquisition of the next image";
        case VulkanExceptionCause::BEGIN_RECORD_COMMAND_BUFFER: return "begin recording command buffer #";
        case VulkanExceptionCause::END_RECORD_COMMAND_BUFFER: return "end recording command buffer #";
        default: return " unknown events";
    }
}

std::string toString(VulkanExceptionCause cause, std::size_t index)
{
    return toString(cause) + std::to_string(index);
}

class VulkanException : public EngineException
{
public:
    VulkanException(VulkanExceptionCause cause)
        : EngineException("Vulkan error occurd during " + toString(cause))
        , m_cause(cause)
    {}

    VulkanException(VulkanExceptionCause cause, std::size_t index)
        : EngineException("Vulkan error occurd during " + toString(cause, index))
        , m_cause(cause)
    {}

    [[nodiscard]] VulkanExceptionCause cause() const { return m_cause; }

private:
    VulkanExceptionCause m_cause;
};

} // !rr

#endif // !RRENDERER_ENGINE_EXCEPTION_VULKAN_EXCEPTION_HPP
