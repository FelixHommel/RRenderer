set(NAME "Engine")

include (${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_library(${NAME}
    Renderer.cpp
    Renderer.hpp
    VulkanRenderer.cpp
    VulkanRenderer.hpp
    utility/StringHash.hpp
    window/Window.cpp
    window/Window.hpp
    exception/EngineException.hpp
    exception/VulkanException.hpp
    core/VulkanInstance.cpp
    core/VulkanInstance.hpp
    core/VulkanDebugMessenger.cpp
    core/VulkanDebugMessenger.hpp
    core/VulkanSurface.cpp
    core/VulkanSurface.hpp
    core/VulkanDevice.cpp
    core/VulkanDevice.hpp
    core/VulkanSwapchain.cpp
    core/VulkanSwapchain.hpp
    core/VulkanPipelineLayout.cpp
    core/VulkanPipelineLayout.hpp
    core/VulkanPipeline.cpp
    core/VulkanPipeline.hpp
    core/VulkanCommandPool.cpp
    core/VulkanCommandPool.hpp
    core/VulkanCommandBuffer.cpp
    core/VulkanCommandBuffer.hpp
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
