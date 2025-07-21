#ifndef RRENDERER_ENGINE_CORE_VULKAN_PIPELINE_LAYOUT_HPP
#define RRENDERER_ENGINE_CORE_VULKAN_PIPELINE_LAYOUT_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/ext/matrix_float2x2.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

#include <vulkan/vulkan_core.h>

namespace rr
{

constexpr std::size_t ALIGNMENT_OF_GLM_VEC3{ 16 }; ///< Alignment of a glm::vec3

/// \brief Simple struct containing everything that is being used as push constants
///
/// \author Felix Hommel
/// \date 7/20/2025
struct SimplePushConstantData
{
    glm::mat2 transform{ 1.f };
    glm::vec2 offset;
    alignas(ALIGNMENT_OF_GLM_VEC3) glm::vec3 color;
};

/// \brief \ref VulkanPipelineLayout is a wrapper around `VkPipelineLayout`
///
/// It is used to tell the `VulkanPipeline` about descriptor set layouts and push constant ranges.
///
/// \author Felix Hommel
/// \date 7/20/2025
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
