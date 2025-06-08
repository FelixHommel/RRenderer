#include "VulkanRenderer.hpp"

#include "core/VulkanCommandPool.hpp"
#include "core/VulkanDebugMessenger.hpp"
#include "core/VulkanDevice.hpp"
#include "core/VulkanInstance.hpp"
#include "core/VulkanMesh.hpp"
#include "core/VulkanPipeline.hpp"
#include "core/VulkanPipelineLayout.hpp"
#include "core/VulkanSurface.hpp"
#include "core/VulkanSwapchain.hpp"
#include "exception/EngineException.hpp"
#include "exception/VulkanException.hpp"
#include "window/Window.hpp"

#include "spdlog/spdlog.h"
#include <vulkan/vulkan_core.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <source_location>

namespace rr
{

VulkanRenderer::VulkanRenderer(Window& window)
    :  m_debugMessenger(std::make_unique<VulkanDebugMessenger>(m_instance->getHandle()))
    , m_surface(std::make_unique<VulkanSurface>(m_instance->getHandle(), window))
    , m_device(std::make_unique<VulkanDevice>(m_instance->getHandle(), m_surface->getHandle()))
    , m_swapchain(std::make_unique<VulkanSwapchain>(*m_device, m_surface->getHandle(), window.getExtent()))
    , m_pipelineLayout(std::make_unique<VulkanPipelineLayout>(m_device->getHandle()))
    , m_pipeline(createPipeline())
    , m_commandPool(std::make_unique<VulkanCommandPool>(*m_device))
    , m_commandBuffers(m_commandPool->allocateCommandBuffer(m_swapchain->imageCount()))
{
    std::vector<Vertex> vertices{{ 
        {0.f, -0.5f}, {1.f, 0.f, 0.f}},
        {{0.5f, 0.5f}, {0.f, 1.f, 0.f}},
        {{-0.5f, 0.5f}, {0.f, 0.f, 1.f}}
    };
    m_model = std::make_unique<VulkanMesh>(*m_device, vertices);

    spdlog::info("allocated {} command buffers", m_commandBuffers.size());
    recordCommandBuffers();
}

void VulkanRenderer::render()
{
    std::uint32_t imageIndex{};
    auto result{ m_swapchain->acquireNextImage(&imageIndex) };

    if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::IMAGE_ACQUISITION);

    result = m_swapchain->submitCommandBuffer(&m_commandBuffers[imageIndex]->getHandle(), &imageIndex);

    if(result != VK_SUCCESS)
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::SUBMIT_COMMAND_BUFFER);
}

void VulkanRenderer::shutdown()
{
    if(m_device)
        vkDeviceWaitIdle(m_device->getHandle());
}

std::unique_ptr<VulkanPipeline> VulkanRenderer::createPipeline()
{
    auto config = VulkanPipeline::defaultPipelineConfigInfo(m_swapchain->getExtent());
    config.renderPass = m_swapchain->getRenderPassHandle();
    config.pipelineLayout = m_pipelineLayout->getHandle();
    return std::make_unique<VulkanPipeline>(m_device->getHandle(), config, "./shaders/basic.vert.spv", "./shaders/basic.frag.spv");
}

void VulkanRenderer::recordCommandBuffers()
{
    VkCommandBufferBeginInfo beginInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
    };

    for(std::size_t i{0}; i < m_commandBuffers.size(); ++i)
    {
        if(vkBeginCommandBuffer(m_commandBuffers.at(i)->getHandle(), &beginInfo) != VK_SUCCESS)
            throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::BEGIN_RECORD_COMMAND_BUFFER, i);

        std::array<VkClearValue, 2> clearValues{
            VkClearValue{ .color = m_clearColor },
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
        m_model->bind(m_commandBuffers[i]->getHandle());
        m_model->draw(m_commandBuffers[i]->getHandle());

        vkCmdEndRenderPass(m_commandBuffers[i]->getHandle());

        if(vkEndCommandBuffer(m_commandBuffers[i]->getHandle()) != VK_SUCCESS)
            throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::END_RECORD_COMMAND_BUFFER, i);
    }

    spdlog::info("Command buffers recorded successfully");
}

}
