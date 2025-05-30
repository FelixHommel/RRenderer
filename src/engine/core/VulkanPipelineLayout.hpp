#ifndef RRENDERER_ENGINE_CORE_VULKAN_PIPELINE_LAYOUT_HPP
#define RRENDERER_ENGINE_CORE_VULKAN_PIPELINE_LAYOUT_HPP

#include <vulkan/vulkan_core.h>

namespace rr
{

class VulkanPipelineLayout
{
public:
    VulkanPipelineLayout(VkDevice device);
    ~VulkanPipelineLayout();

    VulkanPipelineLayout(const VulkanPipelineLayout&) = delete;
    VulkanPipelineLayout(VulkanPipelineLayout&&) = delete;
    VulkanPipelineLayout& operator=(const VulkanPipelineLayout&) = delete;
    VulkanPipelineLayout& operator=(VulkanPipelineLayout&&) = delete;

    [[nodiscard]] VkPipelineLayout getHandle() const { return m_pipelineLayout; }

private:
    VkDevice device;

    VkPipelineLayout m_pipelineLayout{ VK_NULL_HANDLE };
};

} // !rr

#endif // !RRENDERER_ENGINE_CORE_VULKAN_PIPELINE_LAYOUT_HPP
