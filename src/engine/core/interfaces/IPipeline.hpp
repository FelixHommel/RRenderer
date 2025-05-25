#ifndef RRENDERER_ENGINE_CORE_INTERFACES_IPIPELINE_HPP
#define RRENDERER_ENGINE_CORE_INTERFACES_IPIPELINE_HPP

#include "core/CommandBuffer.hpp"

namespace rr
{

class IPipeline
{
public:
    IPipeline() = default;
    virtual ~IPipeline() = default;

    IPipeline(const IPipeline&) = delete;
    IPipeline(IPipeline&&) = delete;
    IPipeline& operator=(const IPipeline&) = delete;
    IPipeline& operator=(IPipeline&&) = delete;

    virtual void bind(CommandBuffer& cmdBuffer) = 0;
};

} // !rr

#endif // !RRENDERER_ENGINE_CORE_INTERFACES_IPIPELINE_HPP
