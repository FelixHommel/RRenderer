#ifndef RRENDERER_ENGINE_GPU_INTERFACE_DEVICE_HPP
#define RRENDERER_ENGINE_GPU_INTERFACE_DEVICE_HPP

#include <cstddef>

namespace rr
{

class IDevice
{
public:
    IDevice() = default;
    virtual ~IDevice() = default;

    IDevice(const IDevice&) = delete;
    IDevice(IDevice&&) noexcept = delete;
    IDevice& operator=(const IDevice&) = delete;
    IDevice& operator=(IDevice&&) noexcept = delete;

    virtual int createBuffer(std::size_t, int) = 0;
    virtual void destroyBuffer() = 0;
    virtual void waitIdle() = 0;
};

} // !rr

#endif // !RRENDERER_ENGINE_GPU_INTERFACE_DEVICE_HPP
