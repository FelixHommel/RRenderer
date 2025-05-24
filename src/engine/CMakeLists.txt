set(NAME "Engine")

include (${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_library(${NAME}
    Renderer.cpp
    Renderer.hpp
    VulkanRenderer.cpp
    VulkanRenderer.hpp
    Shader.hpp
    window/Window.cpp
    window/Window.hpp
    core/VulkanInstance.cpp
    core/VulkanInstance.hpp
    core/VulkanDebugMessenger.cpp
    core/VulkanDebugMessenger.hpp
    core/interfaces/IDevice.hpp
    core/VulkanDevice.cpp
    core/VulkanDevice.hpp
    core/Swapchain.cpp
    core/Swapchain.hpp
    core/CommandBuffer.cpp
    core/CommandBuffer.hpp
)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${NAME} PRIVATE cxx_std_23)
target_link_libraries(${NAME}
    PRIVATE
        project_warnings
    PUBLIC
        Vulkan::Vulkan
        spdlog::spdlog
        glfw
)
