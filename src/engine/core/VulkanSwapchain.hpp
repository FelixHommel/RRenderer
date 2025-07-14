#ifndef RRENDERER_ENGINE_CORE_SWAPCHAIN_HPP
#define RRENDERER_ENGINE_CORE_SWAPCHAIN_HPP

#include "core/VulkanDevice.hpp"

#include <memory>
#include <vulkan/vulkan_core.h>

#include <cstddef>
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
class VulkanSwapchain
{
public:
    VulkanSwapchain(VulkanDevice& device, VkSurfaceKHR surface, VkExtent2D windowExtent);
    VulkanSwapchain(VulkanDevice& device, VkSurfaceKHR surface, VkExtent2D windowExtent, std::shared_ptr<VulkanSwapchain> previous);
    ~VulkanSwapchain();

    VulkanSwapchain(const VulkanSwapchain&) = delete;
    VulkanSwapchain(VulkanSwapchain&&) = delete;
    VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;
    VulkanSwapchain& operator=(VulkanSwapchain&&) = delete;

    static constexpr std::uint32_t MAX_FRAMES_IN_FLIGHT{ 2 };

    [[nodiscard]] std::size_t imageCount() const { return m_swapchainImages.size(); }
    [[nodiscard]] VkExtent2D getExtent() const { return m_swapchainImageExtent; }

    /** Presentation utility */
    [[nodiscard]] VkResult acquireNextImage(std::uint32_t* imageIndex);
    [[nodiscard]] VkResult submitCommandBuffer(const VkCommandBuffer* commandBuffer, const std::uint32_t* imageIndex);

    /** Raw handle access */
    [[nodiscard]] VkSwapchainKHR getHandle() const { return m_swapchain; }
    [[nodiscard]] VkRenderPass getRenderPassHandle() const { return m_renderPass; }
    [[nodiscard]] VkFramebuffer getFramebufferHandle(std::size_t index) const;

private:
    /** External objects */
    VulkanDevice& device;
    VkSurfaceKHR surface;
    VkExtent2D windowExtent;

    /** Core */
    VkSwapchainKHR m_swapchain{ VK_NULL_HANDLE };
    std::vector<VkFramebuffer> m_swapchainFramebuffers{ VK_NULL_HANDLE };
    VkRenderPass m_renderPass{ VK_NULL_HANDLE };
    std::shared_ptr<VulkanSwapchain> m_oldSwapchain{ nullptr };

    /** Images */
    VkFormat m_swapchainImageFormat{};
    VkExtent2D m_swapchainImageExtent{};
    std::vector<VkImage> m_swapchainImages;
    std::vector<VkImageView> m_swapchainImageViews;
    std::vector<VkImage> m_depthImages;
    std::vector<VkDeviceMemory> m_depthImagesMemory;
    std::vector<VkImageView> m_depthImageViews;
    std::size_t m_currentFrame{ 0 };
    std::uint32_t m_lastImageIndex{};

    /** Sync */
    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence> m_inFlightFences;
    std::vector<VkFence> m_imagesInFlight;

    void createVulkanSwapchain();

    /** Setup functions */
    void createSwapchain();
    void createImageViews();
    void createRenderPass();
    void createDepthResources();
    void createFramebuffers();
    void createSyncObjects();

    /** Supporting methods */
    [[nodiscard]] VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
    [[nodiscard]] VkFormat findDepthFormat() const;

    /** Supporting functions */
    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
};

} // !rr

#endif // !RRENDERER_ENGINE_CORE_SWAPCHAIN_HPP
