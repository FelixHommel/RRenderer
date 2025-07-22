#ifndef RRENDERER_ENGINE_EXCEPTIONS_GLFW_EXCEPTION_HPP
#define RRENDERER_ENGINE_EXCEPTIONS_GLFW_EXCEPTION_HPP

#include "exception/EngineException.hpp"

#include <cstdint>
#include <string>

namespace rr
{

/// \brief Enum which contains various reasons why GLFW may throw an exception
///
/// \author Felix Hommel
/// \date 7/19/2025
enum class GLFWExceptionCause : std::uint8_t
{
	GLFW_INIT_FAILED,
	WINDOW_CREATION_FAILED,
	SURFACE_CREATION_FAILED
};

/// \brief If a GLFW operation fails or doesn't behave as expected a GLFWException can be thrown
///
/// \author Felix Hommel
/// \date 7/19/2025
class GLFWException : public EngineException
{
public:
	explicit GLFWException(GLFWExceptionCause cause)
		: EngineException("Vulkan error occurd during " + causeToString(cause))
		, m_cause(cause)
	{}

	[[nodiscard]] GLFWExceptionCause cause() const { return m_cause; }

private:
	GLFWExceptionCause m_cause;

	static std::string causeToString(GLFWExceptionCause cause)
	{
		switch (cause)
		{
			using enum GLFWExceptionCause;

			case GLFW_INIT_FAILED:
				return "initialization of GLFW";
			case WINDOW_CREATION_FAILED:
				return "creation of  a GLFW window";
			case SURFACE_CREATION_FAILED:
				return "creation of a window surface";
			default:
				return "unknown events";
		}
	}
};

} // namespace rr

#endif // !RRENDERER_ENGINE_EXCEPTIONS_GLFW_EXCEPTION_HPP
