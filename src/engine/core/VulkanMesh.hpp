#ifndef RRENDERER_ENGINE_CORE_VULKAN_MESH_HPP
#define RRENDERER_ENGINE_CORE_VULKAN_MESH_HPP

#include "core/VulkanDevice.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <vector>

namespace rr
{

/// \brief A \ref Vertex is a simple point
///
/// It consists of a position and color.
///
/// \author Felix Hommel
/// \date 7/20/2025
struct Vertex
{
    glm::vec2 position;
    glm::vec3 color;

    static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
};

/// \brief A \ref VulkanMesh is a collection of \ref Vertex that makes some sort of shape
///
/// They are allocated on the `VkDevice` and provide drawing and binding functionalities.
///
/// \author Felix Hommel
/// \date 7/20/2025
class VulkanMesh
{
public:
    VulkanMesh(VulkanDevice& device, const std::vector<Vertex>& vertices);
    ~VulkanMesh();

    VulkanMesh(const VulkanMesh&) = delete;
    VulkanMesh& operator=(const VulkanMesh&) = delete;
    VulkanMesh(VulkanMesh&&) = delete;
    VulkanMesh& operator=(VulkanMesh&&) = delete;

    void bind(VkCommandBuffer cmdBuffer) const;
    void draw(VkCommandBuffer cmdBuffer) const;

private:
    VulkanDevice& device;

    VkBuffer m_vertexBuffer{ VK_NULL_HANDLE };
    VkDeviceMemory m_vertexBufferMemory{ VK_NULL_HANDLE };
    std::uint32_t m_vertexCount;
};

} // !rr

#endif // !RRENDERER_ENGINE_CORE_VULKAN_MESH_HPP
