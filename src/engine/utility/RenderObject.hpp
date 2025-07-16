#ifndef RRENDERER_ENGINE_UTILITY_RENDER_OBJECT_HPP
#define RRENDERER_ENGINE_UTILITY_RENDER_OBJECT_HPP

#include "core/VulkanMesh.hpp"

#include "glm/ext/matrix_float2x2.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"

#include <cstdint>
#include <memory>

namespace rr
{

struct Transform2DComponent
{
    glm::vec2 translation{};
    glm::vec2 scale{ 1.f, 1.f };
    float rotation;

    [[nodiscard]] glm::mat2 mat2() const
    {
        const float sine = glm::sin(rotation);
        const float cosine = glm::cos(rotation);
        // NOTE: GLM matricies are initialized in columns, and NOT rows
        glm::mat2 rotMat{ { cosine, sine }, { -sine, cosine } };
        glm::mat2 scaleMat{ { scale.x, 0.f }, { 0.f, scale.y } };

        return rotMat * scaleMat;
    }
};

class RenderObject
{
    public:
        using id_t = std::uint32_t;

        RenderObject(std::shared_ptr<VulkanMesh> mesh, const glm::vec3& color, const Transform2DComponent& transform);
        ~RenderObject() = default;

        RenderObject(const RenderObject&) = delete;
        RenderObject(RenderObject&&) = default;
        RenderObject& operator=(const RenderObject&) = delete;
        RenderObject& operator=(RenderObject&&) = default;

        [[nodiscard]] id_t getId() const { return m_id; }
        [[nodiscard]] glm::vec2 getTranslation() const { return m_transform2D.translation; }
        [[nodiscard]] glm::vec3 getColor() const { return m_color; }
        [[nodiscard]] float getRotation() const { return m_transform2D.rotation; }
        [[nodiscard]] glm::mat2 getTransformMatrix()
        {
            updateTransformMatrix();
            return m_transformMatrixCache;
        }

        void setRotation(float rot) { m_transform2D.rotation = rot; m_transformIsDirty = true; }

        void render(VkCommandBuffer commandBuffer) const;

    private:
        static id_t idGenerator;
        id_t m_id;

        std::shared_ptr<VulkanMesh> m_mesh;
        glm::vec3 m_color;
        Transform2DComponent m_transform2D;

        glm::mat2 m_transformMatrixCache{};
        bool m_transformIsDirty{ true };

        void updateTransformMatrix();
};

} //!rr

#endif //!RRENDERER_ENGINE_UTILITY_RENDER_OBJECT_HPP
