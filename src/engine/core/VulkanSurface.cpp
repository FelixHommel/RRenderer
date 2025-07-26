#include "VulkanSurface.hpp"

#include "spdlog/spdlog.h"
#include "window/Window.hpp"

#include <vulkan/vulkan_core.h>

namespace rr
{

/// \brief Create a new \ref VulkanSurface
///
/// \param instance a `VkInstance`
/// \param window `Window&` needed to interface with the surface
VulkanSurface::VulkanSurface(VkInstance instance, Window& window)
	: instance(instance)
{
	window.createWindowSurface(instance, &m_surface);
	spdlog::info("Surface created successfully...");
}

VulkanSurface::~VulkanSurface()
{
	vkDestroySurfaceKHR(instance, m_surface, nullptr);
}

} // namespace rr
