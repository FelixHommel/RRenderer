set(TEST_NAME "tests")

include (GoogleTest)
include (${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_executable(${TEST_NAME} testVulkanException.cpp)

target_compile_features(${TEST_NAME} PRIVATE cxx_std_20)
target_link_libraries(${TEST_NAME}
    PRIVATE
        project_warnings
        Engine
    PUBLIC
        GTest::gtest_main
)

gtest_discover_tests(${TEST_NAME})
