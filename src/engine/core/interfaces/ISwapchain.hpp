#ifndef RRENDERER_ENGINE_CORE_INTERFACES_ISWAPCHAIN_HPP
#define RRENDERER_ENGINE_CORE_INTERFACES_ISWAPCHAIN_HPP

#include <cstdint>

namespace rr
{

class ISwapchain
{
public:
    ISwapchain() = default;
    virtual ~ISwapchain() = default;

    ISwapchain(const ISwapchain&) = delete;
    ISwapchain(ISwapchain&&) = delete;
    ISwapchain& operator=(const ISwapchain&) = delete;
    ISwapchain& operator=(ISwapchain&&) = delete;

    virtual void resize(std::uint32_t width, std::uint32_t height) = 0;
    virtual void present() = 0;
};

} // !rr

#endif // !RRENDERER_ENGINE_CORE_INTERFACES_ISWAPCHAIN_HPP
