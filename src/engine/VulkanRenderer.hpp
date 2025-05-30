#ifndef RRENDERER_ENGINE_VULKAN_RENDERER_HPP
#define RRENDERER_ENGINE_VULKAN_RENDERER_HPP

#include "Renderer.hpp"
#include "core/VulkanCommandBuffer.hpp"
#include "core/VulkanCommandPool.hpp"
#include "core/VulkanDebugMessenger.hpp"
#include "core/VulkanDevice.hpp"
#include "core/VulkanInstance.hpp"
#include "core/VulkanPipeline.hpp"
#include "core/VulkanPipelineLayout.hpp"
#include "core/VulkanSurface.hpp"
#include "core/VulkanSwapchain.hpp"
#include "window/Window.hpp"

#include <memory>
#include <vulkan/vulkan_core.h>

namespace rr
{

/**
 * Vulkan implementation for the Renderer.
 *
 * @author Felix Hommel
 * @date 5/25/2025
*/
class VulkanRenderer : public Renderer
{
public:
    explicit VulkanRenderer(Window& window);
    ~VulkanRenderer() override = default;

    VulkanRenderer(const VulkanRenderer&) = delete;
    VulkanRenderer(VulkanRenderer&&) = delete;
    VulkanRenderer& operator=(const VulkanRenderer&) = delete;
    VulkanRenderer& operator=(VulkanRenderer&&) = delete;

    void render() override;
    void shutdown() override;

private:
    //NOTE: Order here matters in orer for the right order of dstructions to work and not interfere with vulkan objects
    std::unique_ptr<VulkanInstance> m_instance{ std::make_unique<VulkanInstance>() };
    std::unique_ptr<VulkanDebugMessenger> m_debugMessenger;
    std::unique_ptr<VulkanSurface> m_surface;
    std::unique_ptr<VulkanDevice> m_device;
    std::unique_ptr<VulkanSwapchain> m_swapchain;
    std::unique_ptr<VulkanPipelineLayout> m_pipelineLayout;
    std::unique_ptr<VulkanPipeline> m_pipeline;
    std::unique_ptr<VulkanCommandPool> m_commandPool;
    std::vector<std::unique_ptr<VulkanCommandBuffer>> m_commandBuffers;

    static constexpr VkClearColorValue m_clearColor{ 0.1f, 0.1f, 0.1f, 1.f };
    
    std::unique_ptr<VulkanPipeline> createPipeline();
    void recordCommandBuffers();
};

} // !rr

#endif // !RRENDERER_ENGINE_VULKAN_RENDERER_HPP
