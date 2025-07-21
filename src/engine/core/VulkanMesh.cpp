#include "VulkanMesh.hpp"

#include "core/VulkanDevice.hpp"

#include <array>
#include <cstddef>
#include <cstring>
#include <vulkan/vulkan_core.h>

#include <cassert>
#include <cstdint>
#include <vector>

namespace rr
{

/// \brief Provide the Binding description for a \ref Vertex
///
/// \return std::vector of `VkVertexInputBindingDescription`
std::vector<VkVertexInputBindingDescription> Vertex::getBindingDescriptions()
{
    std::vector<VkVertexInputBindingDescription> bindingDescriptions{
            {
                .binding = 0,
                .stride = sizeof(Vertex),
                .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
            }
    };

    return bindingDescriptions;
}

/// \brief Provide the Attribute descriptions of a \ref Vertex
///
/// \return std::vector of `VkVertexInputAttributeDescription`
std::vector<VkVertexInputAttributeDescription> Vertex::getAttributeDescriptions()
{
    // NOTE: For each attribute in the vertex buffer add an entry to the attribute description
    std::vector<VkVertexInputAttributeDescription> attributeDescription{
            {
                .location = 0,
                .binding = 0,
                .format = VK_FORMAT_R32G32_SFLOAT,
                .offset = offsetof(Vertex, position)
            },
            {
                .location = 1,
                .binding = 0,
                .format = VK_FORMAT_R32G32B32_SFLOAT,
                .offset = offsetof(Vertex, color)
            }
    };

    return attributeDescription;
}

/// \brief Construct a new \ref VulkanMesh
VulkanMesh::VulkanMesh(VulkanDevice& device, const std::vector<Vertex>& vertices)
    : device(device)
    , m_vertexCount(static_cast<std::uint32_t>(vertices.size()))
{
    assert(m_vertexCount >= 3 && "At least 3 vertices are needed to create a vertex buffer");

    VkDeviceSize bufferSize{ sizeof(vertices[0]) * m_vertexCount };

    device.createBuffer(
        bufferSize,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        m_vertexBuffer,
        m_vertexBufferMemory);

    void* data{ nullptr };
    vkMapMemory(device.getHandle(), m_vertexBufferMemory, 0, bufferSize, 0, &data);
    std::memcpy(data, vertices.data(), bufferSize);
    vkUnmapMemory(device.getHandle(), m_vertexBufferMemory);
}

VulkanMesh::~VulkanMesh()
{
    vkDestroyBuffer(device.getHandle(), m_vertexBuffer, nullptr);
    vkFreeMemory(device.getHandle(), m_vertexBufferMemory, nullptr);
}

/// \brief Bind the mesh to a command buffer
///
/// \param cmdBuffer `VkCommandBuffer` to bind the mesh to
void VulkanMesh::bind(VkCommandBuffer cmdBuffer) const
{
    const std::array<VkBuffer, 1> buffers{m_vertexBuffer};
    constexpr std::array<VkDeviceSize, 1> offsets{ 0 };

    vkCmdBindVertexBuffers(cmdBuffer, 0, 1, buffers.data(), offsets.data());
}

/// \brief Draw the mesh to a command buffer
///
/// \param cmdBuffer `VkCommandBuffer` where the mesh is drawn to
void VulkanMesh::draw(VkCommandBuffer cmdBuffer) const
{
    vkCmdDraw(cmdBuffer, m_vertexCount, 1, 0, 0);
}

}; // !rr
