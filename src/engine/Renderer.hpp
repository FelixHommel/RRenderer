#ifndef RRENDERER_ENGINE_RENDERER_HPP
#define RRENDERER_ENGINE_RENDERER_HPP

#include <cstdint>

namespace rr
{

enum class RendererType : std::uint8_t
{
    RRENDERER_VULKAN
};

class Renderer
{
public:
    Renderer() = default;
    virtual ~Renderer() = default;

    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) noexcept = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) noexcept = delete;

    virtual void init() = 0;
    virtual void drawFrame() = 0;
    virtual void shutdown() = 0;
};

} // !rr

#endif // !RRENDERER_ENGINE_RENDERER_HPP
