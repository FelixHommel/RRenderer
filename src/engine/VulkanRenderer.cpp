#include "VulkanRenderer.hpp"

#include "core/VulkanDebugMessenger.hpp"
#include "core/VulkanDevice.hpp"
#include "core/VulkanInstance.hpp"
#include "core/VulkanPipelineLayout.hpp"
#include "core/VulkanSurface.hpp"
#include "window/Window.hpp"

#include <memory>

namespace rr
{

VulkanRenderer::VulkanRenderer(Window& window)
    : m_instance(std::make_unique<VulkanInstance>())
    , m_debugMessenger(std::make_unique<VulkanDebugMessenger>(m_instance->getHandle()))
    , m_surface(std::make_unique<VulkanSurface>(m_instance->getHandle(), window))
    , m_device(std::make_unique<VulkanDevice>(m_instance->getHandle(), m_surface->getHandle()))
    , m_swapchain(std::make_unique<VulkanSwapchain>(*m_device, m_surface->getHandle(), window.getExtent()))
    , m_pipelineLayout(std::make_unique<VulkanPipelineLayout>(m_device->getHandle()))
    , m_pipeline(createPipeline())
{}

VulkanRenderer::~VulkanRenderer() = default;

void VulkanRenderer::init()
{}

void VulkanRenderer::drawFrame()
{}

void VulkanRenderer::shutdown()
{}

std::unique_ptr<VulkanPipeline> VulkanRenderer::createPipeline()
{
    auto config = VulkanPipeline::defaultPipelineConfigInfo(m_swapchain->getExtent());
    config.renderPass = m_swapchain->getRenderPassHandle();
    config.pipelineLayout = m_pipelineLayout->getHandle();
    return std::make_unique<VulkanPipeline>(m_device->getHandle(), config, "./shaders/basic.vert.spv", "./shaders/basic.frag.spv");
}

}
