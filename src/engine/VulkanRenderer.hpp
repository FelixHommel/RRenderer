#ifndef RRENDERER_ENGINE_VULKAN_RENDERER_HPP
#define RRENDERER_ENGINE_VULKAN_RENDERER_HPP

#include "Renderer.hpp"
#include "core/Swapchain.hpp"
#include "core/VulkanDebugMessenger.hpp"
#include "core/VulkanDevice.hpp"
#include "core/VulkanInstance.hpp"
#include "window/Window.hpp"

#include <memory>

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
    VulkanRenderer(Window& window);
    ~VulkanRenderer() override;

    VulkanRenderer(const VulkanRenderer&) = delete;
    VulkanRenderer(VulkanRenderer&&) = delete;
    VulkanRenderer& operator=(const VulkanRenderer&) = delete;
    VulkanRenderer& operator=(VulkanRenderer&&) = delete;

    // NOTE: probably remove init()
    void init() override;
    void drawFrame() override;
    void shutdown() override;

private:
    //NOTE: Order here matters in orer for the right order of dstructions to work and not interfere with vulkan objects
    std::unique_ptr<VulkanInstance> m_instance;
    std::unique_ptr<VulkanDebugMessenger> m_debugMessenger;
    // surface
    std::unique_ptr<VulkanDevice> m_device;
    std::unique_ptr<Swapchain> m_swapchain;
    // commandPool
    // commandBuffer
};

} // !rr

#endif // !RRENDERER_ENGINE_VULKAN_RENDERER_HPP
