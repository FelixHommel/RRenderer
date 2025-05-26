#include "VulkanRenderer.hpp"

#include "core/VulkanCommandPool.hpp"
#include "core/VulkanDebugMessenger.hpp"
#include "core/VulkanDevice.hpp"
#include "core/VulkanInstance.hpp"
#include "core/VulkanPipelineLayout.hpp"
#include "core/VulkanSurface.hpp"
#include "window/Window.hpp"

#include "spdlog/spdlog.h"

#include <array>
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
    , m_commandPool(std::make_unique<VulkanCommandPool>(*m_device))
    , m_commandBuffers(m_commandPool->allocateCommandBuffer(m_swapchain->imageCount()))
{
    recordCommandBuffers();
}

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

void VulkanRenderer::recordCommandBuffers()
{
    for(std::size_t i{0}; i < m_commandBuffers.size(); ++i)
    {
        VkCommandBufferBeginInfo beginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
        };

        if(vkBeginCommandBuffer(m_commandBuffers.at(i)->getHandle(), &beginInfo) != VK_SUCCESS)
        {
            spdlog::critical("Failure while beginning recording of command buffer #{}", i);
            throw std::runtime_error("Failed to begin recording of command m_commandBuffers");
        }

        std::array<VkClearValue, 2> clearValues{
            VkClearValue{ .color = { 0.1f, 0.1f, 0.1f, 1.f } },
            VkClearValue{ .depthStencil = { 1.f, 0 } }
        };
        VkRenderPassBeginInfo renderPassBeginInfo{
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass = m_swapchain->getRenderPassHandle(),
            .framebuffer = m_swapchain->getFramebufferHandle(i),
            .renderArea = {
                .offset = { 0, 0 },
                .extent = m_swapchain->getExtent()
            },
            .clearValueCount = static_cast<std::uint32_t>(clearValues.size()),
            .pClearValues = clearValues.data()
        };

        vkCmdBeginRenderPass(m_commandBuffers[i]->getHandle(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        m_pipeline->bind(m_commandBuffers[i]->getHandle());
        vkCmdDraw(m_commandBuffers[i]->getHandle(), 3, 1, 0, 0);

        vkCmdEndRenderPass(m_commandBuffers[i]->getHandle());

        if(vkEndCommandBuffer(m_commandBuffers[i]->getHandle()) != VK_SUCCESS)
        {
            spdlog::critical("Failure while recording command buffer #{}", i);
            throw std::runtime_error("Failed to record command m_commandBuffers");
        }
    }

    spdlog::info("Command buffers recorded successfully");
}

}
