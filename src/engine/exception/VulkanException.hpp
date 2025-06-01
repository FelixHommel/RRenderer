#ifndef RRENDERER_ENGINE_EXCEPTION_VULKAN_EXCEPTION_HPP
#define RRENDERER_ENGINE_EXCEPTION_VULKAN_EXCEPTION_HPP

#include "exception/EngineException.hpp"

#include <cstddef>
#include <cstdint>
#include <string>

namespace rr
{

enum class VulkanExceptionCause : std::uint8_t
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
    GLFW_EXTENSIONS_MISSING,
    CREATE_GRAPHICS_PIPELINE,
    CREATE_SHADER_MODULE,
    CREATE_PIPELINE_LAYOUT,
    QUEUE_SUBMIT_GRAPHICS,
    CREATE_SWAPCHAIN_KHR,
    CREATE_IMAGE_VIEW,
    CREATE_RENDER_PASS,
    CREATE_FRAMEBUFFER,
    CREATE_SEMAPHORE,
    CREATE_IN_FLIGHT_SYNC_OBJECT,
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

    static std::string causeToString(VulkanExceptionCause cause)
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
            case VulkanExceptionCause::GLFW_EXTENSIONS_MISSING: return "verifying availability of extensions required by GLFW";
            case VulkanExceptionCause::CREATE_GRAPHICS_PIPELINE: return "creation of VkPipeline";
            case VulkanExceptionCause::CREATE_SHADER_MODULE: return "creation of VkShaderModule";
            case VulkanExceptionCause::CREATE_PIPELINE_LAYOUT: return "creation of VkPipelineLayout";
            case VulkanExceptionCause::QUEUE_SUBMIT_GRAPHICS: return "submiting graphics queue";
            case VulkanExceptionCause::CREATE_SWAPCHAIN_KHR: return "creation of VkSwapchain";
            case VulkanExceptionCause::CREATE_IMAGE_VIEW: return "creation of VkImageView #";
            case VulkanExceptionCause::CREATE_RENDER_PASS: return "creation of VkRenderPass";
            case VulkanExceptionCause::CREATE_FRAMEBUFFER: return "creation of framebuffer #";
            case VulkanExceptionCause::CREATE_SEMAPHORE: return "creation of VkSemaphore #";
            case VulkanExceptionCause::CREATE_IN_FLIGHT_SYNC_OBJECT: return "creation of in flight VkFence or VkSemaphore #";
            default: return "unknown events";
        }
    }

    static std::string causeToString(VulkanExceptionCause cause, std::size_t index)
    {
        return causeToString(cause) + std::to_string(index);
    }
};

} // !rr

#endif // !RRENDERER_ENGINE_EXCEPTION_VULKAN_EXCEPTION_HPP
