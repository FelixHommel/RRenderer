#ifndef RRENDERER_ENGINE_CONSTANTS_HPP
#define RRENDERER_ENGINE_CONSTANTS_HPP

namespace rr
{

#ifdef RRDEBUG
    static constexpr bool useValidationLayers{true};
#else
    static constexpr bool useValidationLayers{false};
#endif

}

#endif // !RRENDERER_ENGINE_CONSTANTS_HPP
