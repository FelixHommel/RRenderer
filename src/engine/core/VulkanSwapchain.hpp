#ifndef RRENDERER_ENGINE_GPU_SWAPCHAIN_HPP
#define RRENDERER_ENGINE_GPU_SWAPCHAIN_HPP

#include "core/VulkanDevice.hpp"
#include "interfaces/ISwapchain.hpp"

#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <vector>

namespace rr
{

/**
 *  <code>VulkanSwapchain<\code> is a wrapper around <code>VkSwapchainKHR<\code> and all supporting resources
 *  like render passes, depth resources and sync objects.
 *
 *  @author Felix Hommel
 *  @date 5/26/2025
*/
class VulkanSwapchain : public ISwapchain
{
public:
    VulkanSwapchain(VulkanDevice& device, VkSurfaceKHR surface, VkExtent2D windowExtent);
    ~VulkanSwapchain() override;

    VulkanSwapchain(const VulkanSwapchain&) = delete;
    VulkanSwapchain(VulkanSwapchain&&) = delete;
    VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;
    VulkanSwapchain& operator=(VulkanSwapchain&&) = delete;

    void resize(std::uint32_t width, std::uint32_t height) override;
    void present() override;

    [[nodiscard]] std::size_t imageCount() const { return m_swapchainImages.size(); }

private:
    /** External objects */
    VulkanDevice& device;
    VkSurfaceKHR surface;
    VkExtent2D windowExtent;

    /** Core */
    VkSwapchainKHR m_swapchain;
    std::vector<VkFramebuffer> m_swapchainFramebuffers;
    VkRenderPass m_renderPass;

    /** Images */
    VkFormat m_swapchainImageFormat;
    VkExtent2D m_swapchainImageExtent;
    std::vector<VkImage> m_swapchainImages;
    std::vector<VkImageView> m_swapchainImageViews;
    std::vector<VkImage> m_depthImages;
    std::vector<VkDeviceMemory> m_depthImagesMemory;
    std::vector<VkImageView> m_depthImageViews;

    /** Sync */
    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence> m_inFlightFences;
    std::vector<VkFence> m_imagesInFlight;

    /** local constants */
    static constexpr std::uint32_t MAX_FRAMES_IN_FLIGHT{ 2 };

    /** Setup functions */
    void createSwapchain();
    void createImageViews();
    void createRenderPass();
    void createDepthResources();
    void createFramebuffers();
    void createSyncObjects();

    /** Supporting methods */
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkFormat findDepthFormat();

    /** Supporting functions */
    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
};

} // !rr

#endif // !RRENDERER_ENGINE_GPU_SWAPCHAIN_HPP
