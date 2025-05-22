cmake_minimum_required(VERSION 3.30)
project(
    RRenderer
    DESCRIPTION "A Renderer using Vulkan written in C++"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/DefaultProjectSettings.cmake)

add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

include(cmake/CompileShaders.cmake)

enable_testing()
add_subdirectory(deps)
add_subdirectory(src)
add_subdirectory(test)
