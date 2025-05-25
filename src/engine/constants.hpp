#ifndef RRENDERER_ENGINE_CONSTANTS_HPP
#define RRENDERER_ENGINE_CONSTANTS_HPP

#include <vector>
namespace rr
{

#ifdef RRDEBUG
    static constexpr bool useValidationLayers{true};
#else
    static constexpr bool useValidationLayers{false};
#endif

const std::vector<const char*> validationLayers{
    "VK_LAYER_KHRONOS_validation"
};

}

#endif // !RRENDERER_ENGINE_CONSTANTS_HPP
