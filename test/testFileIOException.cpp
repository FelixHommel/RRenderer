#include "exception/FileIOException.hpp"
#include "gtest/gtest.h"
#include <filesystem>

TEST(FileIOException, constructor)
{
    std::filesystem::path p("non-existing-file-that-cant-be-found.nothing");

    try
    {
        rr::throwWithLog<rr::FileIOException>(std::source_location::current(), std::string(p));
        FAIL() << "Expected VulkanException";
    }
    catch(const rr::FileIOException& ex)
    {
        EXPECT_EQ(std::string(p), ex.getPath());
    }
    catch(const std::runtime_error& ex)
    {
        FAIL() << "Expected VulkanException";
    }
}
