#ifndef RRENDERER_ENGINE_CORE_INTERFACES_ICOMMAND_POOL_HPP
#define RRENDERER_ENGINE_CORE_INTERFACES_ICOMMAND_POOL_HPP

namespace rr
{

class ICommandPool
{
public:
    ICommandPool() = default;
    virtual ~ICommandPool() = default;

    ICommandPool(const ICommandPool&) = delete;
    ICommandPool(ICommandPool&&) = delete;
    ICommandPool& operator=(const ICommandPool&) = delete;
    ICommandPool& operator=(ICommandPool&&) = delete;
};

} // !rr

#endif // !RRENDERER_ENGINE_CORE_INTERFACES_ICOMMAND_POOL_HPP
