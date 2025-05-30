#ifndef RRENDERER_ENGINE_CORE_VULKAN_SURFACE_HPP
#define RRENDERER_ENGINE_CORE_VULKAN_SURFACE_HPP

#include "window/Window.hpp"

#include <vulkan/vulkan_core.h>

namespace rr
{

/**
 *  <code>VulkanSurface<\code> is a wrapper of <code>VkSurfaceKHR<\code>
 *
 *  @author Felix Hommel
 *  @date 5/26/2025
*/
class VulkanSurface
{
public:
    VulkanSurface(VkInstance instance, Window& window);
    ~VulkanSurface();

    VulkanSurface(const VulkanSurface&) = delete;
    VulkanSurface(VulkanSurface&&) = delete;
    VulkanSurface& operator=(const VulkanSurface&) = delete;
    VulkanSurface& operator=(VulkanSurface&&) = delete;

    [[nodiscard]] VkSurfaceKHR getHandle() const { return m_surface; }

private:
    VkInstance instance;

    VkSurfaceKHR m_surface{ VK_NULL_HANDLE };
};

} // !rr

#endif // !RRENDERER_ENGINE_CORE_VULKAN_SURFACE_HPP
