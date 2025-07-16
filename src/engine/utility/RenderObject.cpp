#include "RenderObject.hpp"

namespace rr
{

id_t RenderObject::idGenerator{ 0 };

RenderObject::RenderObject(std::shared_ptr<VulkanMesh> mesh, const glm::vec3& color, const Transform2DComponent& transform)
    : m_id(idGenerator++)
    , m_mesh{ mesh }
    , m_color{ color }
    , m_transform2D{ transform }
{}

void RenderObject::render(VkCommandBuffer commandBuffer) const
{
    m_mesh->bind(commandBuffer);
    m_mesh->draw(commandBuffer);
}

void RenderObject::updateTransformMatrix()
{
    if(!m_transformIsDirty)
        return;

    m_transformMatrixCache = m_transform2D.mat2();
    m_transformIsDirty = false;
}

}
