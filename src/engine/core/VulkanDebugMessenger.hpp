#ifndef RRENDERER_ENGINE_CORE_VULKAN_DEBUG_MESSENGER_HPP
#define RRENDERER_ENGINE_CORE_VULKAN_DEBUG_MESSENGER_HPP

#include <vulkan/vulkan_core.h>

namespace rr
{

/**
 *  <code>VulkanDebugMessenger<\code> is used to collect and display relevant debug information. It wrapps
 *  <code>VkDebugUtilsMessengerEXT<\code>.
 *
 *  @author Felix Hommel
 *  @date 5/25/2025
*/
class VulkanDebugMessenger
{
public:
    explicit VulkanDebugMessenger(VkInstance instance);
    ~VulkanDebugMessenger();

    VulkanDebugMessenger(const VulkanDebugMessenger&) = delete;
    VulkanDebugMessenger(VulkanDebugMessenger&&) = delete;
    VulkanDebugMessenger& operator=(const VulkanDebugMessenger&) = delete;
    VulkanDebugMessenger& operator=(VulkanDebugMessenger&&) = delete;

private:
    VkInstance instance;
    VkDebugUtilsMessengerEXT m_debugMessenger{ VK_NULL_HANDLE };
};

} // !rr

#endif // !RRENDERER_ENGINE_CORE_VULKAN_DEBUG_MESSENGER_HPP
