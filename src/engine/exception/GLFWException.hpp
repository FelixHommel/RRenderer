#ifndef RRENDERER_ENGINE_EXCEPTIONS_GLFW_EXCEPTION_HPP
#define RRENDERER_ENGINE_EXCEPTIONS_GLFW_EXCEPTION_HPP

#include "exception/EngineException.hpp"

#include <cstdint>
#include <string>

namespace rr
{

enum class GLFWExceptionCause : std::uint8_t
{
    GLFW_INIT_FAILED,
    WINDOW_CREATION_FAILED,
    SURFACE_CREATION_FAILED
};

class GLFWException : public EngineException
{
public:
    GLFWException(GLFWExceptionCause cause)
        : EngineException("Vulkan error occurd during " + causeToString(cause))
        , m_cause(cause)
    {}

    [[nodiscard]] GLFWExceptionCause cause() const { return m_cause; }

private:
    GLFWExceptionCause m_cause;

    static std::string causeToString(GLFWExceptionCause cause)
    {
        switch(cause)
        {
            case GLFWExceptionCause::GLFW_INIT_FAILED: return "initialization of GLFW";
            case GLFWExceptionCause::WINDOW_CREATION_FAILED: return "creation of  a GLFW window";
            case GLFWExceptionCause::SURFACE_CREATION_FAILED: return "creation of a window surface";
            default: return "unknown events";
        }
    }
};

} // !rr

#endif // !RRENDERER_ENGINE_EXCEPTIONS_GLFW_EXCEPTION_HPP
