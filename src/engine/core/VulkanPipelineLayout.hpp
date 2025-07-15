#ifndef RRENDERER_ENGINE_CORE_VULKAN_PIPELINE_LAYOUT_HPP
#define RRENDERER_ENGINE_CORE_VULKAN_PIPELINE_LAYOUT_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

#include <vulkan/vulkan_core.h>

namespace rr
{

constexpr std::size_t ALIGN_OF_VEC3{ 16 };

struct SimplePushConstantData
{
    glm::vec2 offset;
    alignas(ALIGN_OF_VEC3) glm::vec3 color;
};

class VulkanPipelineLayout
{
public:
    explicit VulkanPipelineLayout(VkDevice device);
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
