set(TEST_NAME "tests")

include (GoogleTest)
include (${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_executable(${TEST_NAME} testVulkanException.cpp testFileIOException.cpp testGLFWException.cpp)

target_compile_features(${TEST_NAME} PRIVATE cxx_std_20)
target_link_libraries(${TEST_NAME}
    PRIVATE
        project_warnings
        Engine
    PUBLIC
        GTest::gtest_main
)

# Setup for coverage
set(GCC_COVERAGE_COMPILER_FLAGS "-fprofile-arcs -ftest-coverage")
set(GCC_COVERAGE_LINK_FLAGS "--coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

gtest_discover_tests(${TEST_NAME})
