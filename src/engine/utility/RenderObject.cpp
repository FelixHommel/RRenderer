#include "RenderObject.hpp"

#include "core/VulkanMesh.hpp"

#include "glm/ext/vector_float3.hpp"
#include <vulkan/vulkan_core.h>

#include <memory>

namespace rr
{

/// \brief Construct a new RenderObject
///
/// \param mesh std::shared_ptr of a \ref VulkanMesh
/// \param color glm::vec3& to represent the color of the object
/// \param transform \ref Transform2DComponent& initialized with the transform of the object
RenderObject::RenderObject(
	std::shared_ptr<VulkanMesh> mesh, const glm::vec3& color, const Transform2DComponent& transform)
	: m_id(idGenerator++)
	, m_mesh{ mesh }
	, m_color{ color }
	, m_transform2D{ transform }
{}

/// \brief Render the \ref RenderObject
///
/// \param commandBuffer VkCommandBuffer
void RenderObject::render(VkCommandBuffer commandBuffer) const
{
	m_mesh->bind(commandBuffer);
	m_mesh->draw(commandBuffer);
}

/// \brief Update the Transform matrix if needed
///
/// Check if the transform matrix is outdated because of a change in transformations and update it if needed.
void RenderObject::updateTransformMatrix()
{
	if (!m_transformIsDirty)
		return;

	m_transformMatrixCache = m_transform2D.mat2();
	m_transformIsDirty = false;
}

} // namespace rr
