#include "gtest/gtest.h"

#include "exception/EngineException.hpp"
#include "exception/VulkanException.hpp"

#include <source_location>
#include <stdexcept>

TEST(VulkanException, ConstructorNonIndexed)
{
    try
    {
        rr::throwWithLog<rr::VulkanException>(std::source_location::current(), rr::VulkanExceptionCause::CREATE_INSTANCE);
        FAIL() << "Expected VulkanException";
    }
    catch(const rr::VulkanException& ex)
    {
        EXPECT_EQ(ex.cause(), rr::VulkanExceptionCause::CREATE_INSTANCE);
    }
    catch(const std::runtime_error& ex)
    {
        FAIL() << "Expected VulkanException";
    }
}

TEST(VulkanException, ConstructorIndexed)
{
    try
    {
        rr::throwWithLog<rr::VulkanException>(std::source_location::current(), rr::VulkanExceptionCause::BEGIN_RECORD_COMMAND_BUFFER, 1);
        FAIL() << "Expected VulkanException";
    }
    catch(const rr::VulkanException& ex)
    {
        EXPECT_EQ(ex.cause(), rr::VulkanExceptionCause::BEGIN_RECORD_COMMAND_BUFFER);
    }
    catch(const std::runtime_error& ex)
    {
        FAIL() << "Expected VulkanException";
    }
}
