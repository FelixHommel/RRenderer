#ifndef RRENDERER_ENGINE_RENDERER_HPP
#define RRENDERER_ENGINE_RENDERER_HPP

#include <cstdint>

namespace rr
{

/// \enum RendererType
///
/// \brief Enum listing all available Renderer implementations
enum class RendererType : std::uint8_t
{
	RRENDERER_VULKAN
};

/// \brief An abstract base class for specific renderer implementations
///
/// The \ref Renderer interface consists of \ref Renderer::render() to draw images to the screen and
/// \ref Renderer::shutdown() to cleanly exit the Renderer early
///
/// \author Felix Hommel
/// \date 7/18/2025
class Renderer
{
public:
	Renderer() = default;
	virtual ~Renderer() = default;

	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) noexcept = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&) noexcept = delete;

	/// \brief Draw an image to the screen
	virtual void render() = 0;

	/// \brief Shutdown the Renderer
	///
	/// This method can be used to exit the Renderer before the App using it closes
	virtual void shutdown() = 0;
};

} // namespace rr

#endif // !RRENDERER_ENGINE_RENDERER_HPP
