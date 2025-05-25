#include "VulkanRenderer.hpp"

#include "core/VulkanDebugMessenger.hpp"
#include "core/VulkanInstance.hpp"
#include "core/VulkanSurface.hpp"
#include "window/Window.hpp"

namespace rr
{

VulkanRenderer::VulkanRenderer(Window& window)
    : m_instance(std::make_unique<VulkanInstance>())
    , m_debugMessenger(std::make_unique<VulkanDebugMessenger>(m_instance->getHandle()))
    , m_surface(std::make_unique<VulkanSurface>(m_instance->getHandle(), window))
{}

VulkanRenderer::~VulkanRenderer() = default;

void VulkanRenderer::init()
{}

void VulkanRenderer::drawFrame()
{}

void VulkanRenderer::shutdown()
{}

}
