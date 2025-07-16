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
#include "utility/RenderObject.hpp"
#include "window/Window.hpp"

#include <vulkan/vulkan_core.h>

#include <cstddef>
#include <memory>
#include <string_view>
#include <vector>

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
    Window& window;

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
    std::vector<RenderObject> m_renderObjects;

    static constexpr VkClearColorValue CLEAR_COLOR{ 0.01f, 0.01f, 0.01f, 1.f };
    static constexpr std::string_view BASIC_VERT_SHADER_PATH{ "./shaders/basic.vert.spv" };
    static constexpr std::string_view BASIC_FRAG_SHADER_PATH{ "./shaders/basic.frag.spv" };
    
    [[nodiscard]] std::unique_ptr<VulkanPipeline> createPipeline();

    void loadRenderObjects();
    void recreateSwapchain();
    void recordCommandBuffers(std::size_t imageIndex);
    void renderObjects(VkCommandBuffer comandBuffer);
};

} // !rr

#endif // !RRENDERER_ENGINE_VULKAN_RENDERER_HPP
