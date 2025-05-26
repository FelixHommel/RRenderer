#ifndef RRENDERER_ENGINE_CORE_INTERFACES_ICOMMAND_BUFFER_HPP
#define RRENDERER_ENGINE_CORE_INTERFACES_ICOMMAND_BUFFER_HPP

namespace rr
{

class ICommandBuffer
{
public:
    ICommandBuffer() = default;
    virtual ~ICommandBuffer() = default;

    ICommandBuffer(const ICommandBuffer&) = delete;
    ICommandBuffer(ICommandBuffer&&) = delete;
    ICommandBuffer& operator=(const ICommandBuffer&) = delete;
    ICommandBuffer& operator=(ICommandBuffer&&) = delete;
};

} // !rr

#endif // !RRENDERER_ENGINE_CORE_INTERFACES_ICOMMAND_BUFFER_HPP
