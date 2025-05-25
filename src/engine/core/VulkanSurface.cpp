#include "VulkanSurface.hpp"

#include "spdlog/spdlog.h"
#include "window/Window.hpp"

#include <vulkan/vulkan_core.h>

namespace rr
{

VulkanSurface::VulkanSurface(VkInstance instance, Window& window)
    : instance(instance)
    , m_surface(VK_NULL_HANDLE)
{
    window.createWindowSurface(instance, &m_surface);
    spdlog::info("Surface created successfully...");
}

VulkanSurface::~VulkanSurface()
{
    vkDestroySurfaceKHR(instance, m_surface, nullptr);
}

} // !rr
