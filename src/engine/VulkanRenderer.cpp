#include "VulkanRenderer.hpp"

#include "GLFW/glfw3.h"
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
#include <cassert>
#include <utility>
#include <vulkan/vulkan_core.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <source_location>
#include <vector>

namespace rr
{

VulkanRenderer::VulkanRenderer(Window& window)
    : window(window)
    , m_debugMessenger(std::make_unique<VulkanDebugMessenger>(m_instance->getHandle()))
    , m_surface(std::make_unique<VulkanSurface>(m_instance->getHandle(), window))
    , m_device(std::make_unique<VulkanDevice>(m_instance->getHandle(), m_surface->getHandle()))
    , m_swapchain(std::make_unique<VulkanSwapchain>(*m_device, m_surface->getHandle(), window.getExtent()))
    , m_pipelineLayout(std::make_unique<VulkanPipelineLayout>(m_device->getHandle()))
    , m_pipeline(createPipeline())
    , m_commandPool(std::make_unique<VulkanCommandPool>(*m_device))
    , m_commandBuffers(m_commandPool->allocateCommandBuffer(m_swapchain->imageCount()))
{
    std::vector<Vertex> vertices{
        {.position = {0.f, -0.5f}, .color = {1.f, 0.f, 0.f}}, //NOLINT
        {.position = {0.5f, 0.5f}, .color = {0.f, 1.f, 0.f}}, //NOLINT
        {.position = {-0.5f, 0.5f}, .color = {0.f, 0.f, 1.f}} //NOLINT
    };
    m_model = std::make_unique<VulkanMesh>(*m_device, vertices);

    spdlog::info("allocated {} command buffers", m_commandBuffers.size());
}

void VulkanRenderer::render()
{
    std::uint32_t imageIndex{};
    auto result{ m_swapchain->acquireNextImage(&imageIndex) };

    if(result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        recreateSwapchain();

        return;
    }

    if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::IMAGE_ACQUISITION);

    recordCommandBuffers(imageIndex);
    result = m_swapchain->submitCommandBuffer(&m_commandBuffers[imageIndex]->getHandle(), &imageIndex);

    if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized())
    {
        window.resetWindowResized();
        recreateSwapchain();

        return;
    }

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
    assert(m_swapchain != nullptr && "Cannot create pipeline before swapchain");
    assert(m_pipelineLayout != nullptr && "cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    VulkanPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = m_swapchain->getRenderPassHandle();
    pipelineConfig.pipelineLayout = m_pipelineLayout->getHandle();
    return std::make_unique<VulkanPipeline>(m_device->getHandle(), pipelineConfig, BASIC_VERT_SHADER_PATH, BASIC_FRAG_SHADER_PATH);
}

void VulkanRenderer::recreateSwapchain()
{
    auto extent{ window.getExtent() };
    while(extent.height == 0 || extent.width == 0)
    {
        extent = window.getExtent();
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(m_device->getHandle());

    if(m_swapchain == nullptr)
    {
        m_swapchain = std::make_unique<VulkanSwapchain>(*m_device, m_surface->getHandle(), extent);
    }
    else
    {
        m_swapchain = std::make_unique<VulkanSwapchain>(*m_device, m_surface->getHandle(), extent, std::move(m_swapchain));

        if(m_swapchain->imageCount() != m_commandBuffers.size())
            m_commandBuffers = m_commandPool->allocateCommandBuffer(m_swapchain->imageCount());
    }

    m_pipeline = createPipeline();
}

void VulkanRenderer::recordCommandBuffers(std::size_t imageIndex)
{
    static int frame{ 30 }; //NOLINT
    frame = (++frame) % 100; //NOLINT

    VkCommandBufferBeginInfo beginInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
    };

    if(vkBeginCommandBuffer(m_commandBuffers.at(imageIndex)->getHandle(), &beginInfo) != VK_SUCCESS)
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::BEGIN_RECORD_COMMAND_BUFFER, imageIndex);

    std::array<VkClearValue, 2> clearValues{
        VkClearValue{ .color = CLEAR_COLOR },
        VkClearValue{ .depthStencil = { 1.f, 0 } }
    };
    VkRenderPassBeginInfo renderPassBeginInfo{
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = m_swapchain->getRenderPassHandle(),
        .framebuffer = m_swapchain->getFramebufferHandle(imageIndex),
        .renderArea = {
            .offset = { 0, 0 },
            .extent = m_swapchain->getExtent()
        },
        .clearValueCount = static_cast<std::uint32_t>(clearValues.size()),
        .pClearValues = clearValues.data()
    };

    vkCmdBeginRenderPass(m_commandBuffers[imageIndex]->getHandle(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{
        .x = 0,
        .y = 0,
        .width = static_cast<float>(m_swapchain->getExtent().width),
        .height = static_cast<float>(m_swapchain->getExtent().height),
        .minDepth = 0.f,
        .maxDepth = 1.f
    };
    vkCmdSetViewport(m_commandBuffers[imageIndex]->getHandle(), 0, 1, &viewport);

    VkRect2D scissor{
        { 0, 0 },
        m_swapchain->getExtent()
    };
    vkCmdSetScissor(m_commandBuffers[imageIndex]->getHandle(), 0, 1, &scissor);

    m_pipeline->bind(m_commandBuffers[imageIndex]->getHandle());
    m_model->bind(m_commandBuffers[imageIndex]->getHandle());

    for(int i{ 0 }; i < 4; ++i)
    {
        SimplePushConstantData pushData{
            .offset = { -0.5f + (frame * 0.02f), -0.4f + (i * 0.25f) }, //NOLINT
            .color = { 0.f, 0.f, 0.2f + (0.2f * i) } //NOLINT
        };

        vkCmdPushConstants(m_commandBuffers[imageIndex]->getHandle(), m_pipelineLayout->getHandle(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &pushData);
        m_model->draw(m_commandBuffers[imageIndex]->getHandle());
    }

    vkCmdEndRenderPass(m_commandBuffers[imageIndex]->getHandle());

    if(vkEndCommandBuffer(m_commandBuffers[imageIndex]->getHandle()) != VK_SUCCESS)
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::END_RECORD_COMMAND_BUFFER, imageIndex);
}

}
