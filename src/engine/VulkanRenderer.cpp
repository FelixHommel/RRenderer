#include "VulkanRenderer.hpp"

#include "core/VulkanDebugMessenger.hpp"
#include "core/VulkanDevice.hpp"
#include "core/VulkanInstance.hpp"
#include "core/VulkanSurface.hpp"
#include "window/Window.hpp"

namespace rr
{

VulkanRenderer::VulkanRenderer(Window& window)
    : m_instance(std::make_unique<VulkanInstance>())
    , m_debugMessenger(std::make_unique<VulkanDebugMessenger>(m_instance->getHandle()))
    , m_surface(std::make_unique<VulkanSurface>(m_instance->getHandle(), window))
    , m_device(std::make_unique<VulkanDevice>(m_instance->getHandle(), m_surface->getHandle()))
    , m_swapchain(std::make_unique<VulkanSwapchain>(*m_device, m_surface->getHandle(), window.getExtent()))
{}

VulkanRenderer::~VulkanRenderer() = default;

void VulkanRenderer::init()
{}

void VulkanRenderer::drawFrame()
{}

void VulkanRenderer::shutdown()
{}

}
