#ifndef RRENDERER_ENGINE_EXCEPTION_VULKAN_EXCEPTION_HPP
#define RRENDERER_ENGINE_EXCEPTION_VULKAN_EXCEPTION_HPP

#include "exception/EngineException.hpp"

#include <cstddef>
#include <cstdint>
#include <format>
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
    VALIDATION_LAYERS_UNAVAILABLE
};

class VulkanException : public EngineException
{
public:
    explicit VulkanException(VulkanExceptionCause cause)
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
            using enum VulkanExceptionCause;

            case CREATE_INSTANCE: return "creation of a VkInstance";
            case SUBMIT_COMMAND_BUFFER: return "submission of a VkCommandBuffer";
            case IMAGE_ACQUISITION: return "the acquisition of the next swapchain image";
            case BEGIN_RECORD_COMMAND_BUFFER: return "begin recording of VkCommandBuffer #";
            case END_RECORD_COMMAND_BUFFER: return "ending recording of VkCommandBuffer #";
            case ALLOCATE_COMMAND_BUFFERS: return "allocating VkCommandBuffers";
            case QUEUE_FAMILY_INDEX_IS_EMPTY: return "trying to get the index of a non-existing VkQueue";
            case CREATE_COMMAND_POOL: return "creation of a VkCommandPool";
            case CREATE_DEBUG_MESSENGER: return "creation of the VkDebugMessenger";
            case FIND_SUPPORTED_FORMAT: return "search for a supported VkFormat";
            case CREATE_IMAGE: return "creation of an VkImage";
            case ALLOCATE_MEMORY: return "allocating memory";
            case BIND_IMAGE_MEMORY: return "binding image memory";
            case NO_PHYSICAL_DEVICE_FOUND: return "search for vulkan compatible VkPhysicalDevice";
            case NO_SUITABLE_DEVICE_FOUND: return "search for suitable VkPhysicalDevice";
            case CREATE_DEVICE: return "creation of a VkDevice";
            case NO_SUITABLE_MEMORY_TYPE_FOUND: return "search for a suitable memory type";
            case GLFW_EXTENSIONS_MISSING: return "verifying availability of extensions required by GLFW";
            case CREATE_GRAPHICS_PIPELINE: return "creation of VkPipeline";
            case CREATE_SHADER_MODULE: return "creation of VkShaderModule";
            case CREATE_PIPELINE_LAYOUT: return "creation of VkPipelineLayout";
            case QUEUE_SUBMIT_GRAPHICS: return "submiting graphics queue";
            case CREATE_SWAPCHAIN_KHR: return "creation of VkSwapchain";
            case CREATE_IMAGE_VIEW: return "creation of VkImageView #";
            case CREATE_RENDER_PASS: return "creation of VkRenderPass";
            case CREATE_FRAMEBUFFER: return "creation of framebuffer #";
            case CREATE_SEMAPHORE: return "creation of VkSemaphore #";
            case CREATE_IN_FLIGHT_SYNC_OBJECT: return "creation of in flight VkFence or VkSemaphore #";
            case VALIDATION_LAYERS_UNAVAILABLE: return "checking for validation layer availability";
            default: return "unknown events";
        }
    }

    static std::string causeToString(VulkanExceptionCause cause, std::size_t index)
    {
        return std::format("{}{}",causeToString(cause), index);
    }
};

} // !rr

#endif // !RRENDERER_ENGINE_EXCEPTION_VULKAN_EXCEPTION_HPP
