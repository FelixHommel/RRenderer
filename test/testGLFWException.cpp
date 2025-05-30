#include "gtest/gtest.h"

#include "exception/EngineException.hpp"
#include "exception/GLFWException.hpp"

#include <source_location>
#include <stdexcept>

TEST(GLFWException, Constructor)
{
    try
    {
        rr::throwWithLog<rr::GLFWException>(std::source_location::current(), rr::GLFWExceptionCause::GLFW_INIT_FAILED);
        FAIL() << "Expected GLFWException";
    }
    catch(const rr::GLFWException& ex)
    {
        EXPECT_EQ(ex.cause(), rr::GLFWExceptionCause::GLFW_INIT_FAILED);
    }
    catch(const std::runtime_error& ex)
    {
        FAIL() << "Expected GLFWException";
    }
}
