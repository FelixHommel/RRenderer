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
#include "glm/ext/vector_float3.hpp"
#include "utility/RenderObject.hpp"
#include "window/Window.hpp"

#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"
#include <vulkan/vulkan_core.h>

#include <array>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <numbers>
#include <source_location>
#include <thread>
#include <utility>
#include <vector>

namespace rr
{

/// \brief Construct a new \ref VulkanRenderer based on \p window
///
/// \param window `Window&`
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
	loadRenderObjects();

	spdlog::info("allocated {} command buffers", m_commandBuffers.size());
}

/// \brief Update the frame
///
/// \throws \ref VulkanException if there are any errors during the rendering process
void VulkanRenderer::render()
{
	// TODO: find better way to limit frames
	constexpr int FPS{ 33 };
	std::this_thread::sleep_for(std::chrono::milliseconds(FPS));

	std::uint32_t imageIndex{};
	auto result{ m_swapchain->acquireNextImage(&imageIndex) };

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		recreateSwapchain();

		return;
	}

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		throwWithLog<VulkanException>(
			std::source_location::current(), VulkanExceptionCause::IMAGE_ACQUISITION);

	recordCommandBuffers(imageIndex);
	result = m_swapchain->submitCommandBuffer(&m_commandBuffers[imageIndex]->getHandleRef(), &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized())
	{
		window.resetWindowResized();
		recreateSwapchain();

		return;
	}

	if (result != VK_SUCCESS)
		throwWithLog<VulkanException>(
			std::source_location::current(), VulkanExceptionCause::SUBMIT_COMMAND_BUFFER);
}

/// \brief Shutdown the Renderer
void VulkanRenderer::shutdown()
{
	if (m_device)
		vkDeviceWaitIdle(m_device->getHandle());
}

/// \brief Load all models that are used in the Renderer
void VulkanRenderer::loadRenderObjects()
{
	const std::vector<Vertex> vertices{
		{ .position = { 0.f, -0.5f }, .color = { 1.f, 0.f, 0.f } }, //NOLINT
		{ .position = { 0.5f, 0.5f }, .color = { 0.f, 1.f, 0.f } }, //NOLINT
		{ .position = { -0.5f, 0.5f }, .color = { 0.f, 0.f, 1.f } } //NOLINT
	};
	const auto mesh = std::make_shared<VulkanMesh>(*m_device, vertices);

	std::vector<glm::vec3> colors{
		{ 1.f, 0.7f, 0.73f }, //NOLINT
		{ 1.f, 0.87f, 0.73f }, //NOLINT
		{ 1.f, 1.f, 0.73f }, //NOLINT
		{ 0.73f, 1.f, 0.8f }, //NOLINT
		{ 0.73f, 0.88f, 1.f } //NOLINT
	};

	for (auto& color : colors)
		color = glm::pow(color, glm::vec3(2.2f)); //NOLINT

	constexpr std::size_t triCount{ 40 };
	for (std::size_t i{ 0 }; i < triCount; ++i)
	{
		RenderObject triangle{ mesh,
							   colors.at(i % colors.size()),
							   { .translation = {},
								 .scale = { glm::vec2(0.5f) + i * 0.025f },
								 .rotation = i * std::numbers::pi_v<float> * 0.025f } }; //NOLINT

		m_renderObjects.push_back(std::move(triangle));
	}
}

/// \brief Create a new \ref VulkanPipeline
///
/// It is returned as a std::unique_ptr. \ref VulkanPipeline depends on the render passes
/// and the pipeline layout.
///
/// \return std::unique_ptr<VulkanPipeline>, the newly created Pipeline
std::unique_ptr<VulkanPipeline> VulkanRenderer::createPipeline() const
{
	assert(m_swapchain != nullptr && "Cannot create pipeline before swapchain");
	assert(m_pipelineLayout != nullptr && "cannot create pipeline before pipeline layout");

	PipelineConfigInfo pipelineConfig{};
	VulkanPipeline::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.renderPass = m_swapchain->getRenderPassHandle();
	pipelineConfig.pipelineLayout = m_pipelineLayout->getHandle();

	return std::make_unique<VulkanPipeline>(
		m_device->getHandle(), pipelineConfig, BASIC_VERT_SHADER_PATH, BASIC_FRAG_SHADER_PATH);
}

/// \brief Recreate the \ref VulkanSwapchain in the case that it is outdated
void VulkanRenderer::recreateSwapchain()
{
	auto extent{ window.getExtent() };
	while (extent.height == 0 || extent.width == 0)
	{
		extent = window.getExtent();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(m_device->getHandle());

	if (m_swapchain == nullptr)
	{
		m_swapchain = std::make_unique<VulkanSwapchain>(*m_device, m_surface->getHandle(), extent);
	}
	else
	{
		m_swapchain = std::make_unique<VulkanSwapchain>(
			*m_device, m_surface->getHandle(), extent, std::move(m_swapchain));

		if (m_swapchain->imageCount() != m_commandBuffers.size())
			m_commandBuffers = m_commandPool->allocateCommandBuffer(m_swapchain->imageCount());
	}

	m_pipeline = createPipeline();
}

/// \brief Record a Command buffer
///
/// \param imageIndex std::size_t denoting which image is being recorded to
void VulkanRenderer::recordCommandBuffers(std::size_t imageIndex)
{
	const VkCommandBufferBeginInfo beginInfo{ .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };

	if (vkBeginCommandBuffer(m_commandBuffers.at(imageIndex)->getHandleRef(), &beginInfo) != VK_SUCCESS)
		throwWithLog<VulkanException>(
			std::source_location::current(), VulkanExceptionCause::BEGIN_RECORD_COMMAND_BUFFER, imageIndex);

	constexpr std::array<VkClearValue, 2> clearValues{ VkClearValue{ .color = CLEAR_COLOR },
													   VkClearValue{ .depthStencil = { 1.f, 0 } } };
	const VkRenderPassBeginInfo renderPassBeginInfo{
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.renderPass = m_swapchain->getRenderPassHandle(),
		.framebuffer = m_swapchain->getFramebufferHandle(imageIndex),
		.renderArea = { .offset = { 0, 0 }, .extent = m_swapchain->getExtent() },
		.clearValueCount = static_cast<std::uint32_t>(clearValues.size()),
		.pClearValues = clearValues.data()
	};

	vkCmdBeginRenderPass(
		m_commandBuffers[imageIndex]->getHandleRef(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	const VkViewport viewport{ .x = 0,
							   .y = 0,
							   .width = static_cast<float>(m_swapchain->getExtent().width),
							   .height = static_cast<float>(m_swapchain->getExtent().height),
							   .minDepth = 0.f,
							   .maxDepth = 1.f };
	vkCmdSetViewport(m_commandBuffers[imageIndex]->getHandleRef(), 0, 1, &viewport);

	const VkRect2D scissor{ { 0, 0 }, m_swapchain->getExtent() };
	vkCmdSetScissor(m_commandBuffers[imageIndex]->getHandleRef(), 0, 1, &scissor);

	renderObjects(m_commandBuffers[imageIndex]->getHandleRef());

	vkCmdEndRenderPass(m_commandBuffers[imageIndex]->getHandleRef());

	if (vkEndCommandBuffer(m_commandBuffers[imageIndex]->getHandleRef()) != VK_SUCCESS)
		throwWithLog<VulkanException>(
			std::source_location::current(), VulkanExceptionCause::END_RECORD_COMMAND_BUFFER, imageIndex);
}

/// \brief Render all \ref RenderObjects
///
/// \param commandBuffer VkCommandBuffer to which the \ref RenderObject are recorded to
void VulkanRenderer::renderObjects(VkCommandBuffer commandBuffer)
{
	int i{ 0 };
	for (auto& obj : m_renderObjects)
	{
		++i;
		obj.setRotation(
			glm::mod<float>(obj.getRotation() + 0.001f * i, 2.f * std::numbers::pi_v<float>)); //NOLINT
	}

	m_pipeline->bind(commandBuffer);
	for (auto& obj : m_renderObjects)
	{
		SimplePushConstantData pushData{ .transform = obj.getTransformMatrix(),
										 .offset = obj.getTranslation(),
										 .color = obj.getColor() };

		vkCmdPushConstants(
			commandBuffer,
			m_pipelineLayout->getHandle(),
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(SimplePushConstantData),
			&pushData);

		obj.render(commandBuffer);
	}
}

} // namespace rr
