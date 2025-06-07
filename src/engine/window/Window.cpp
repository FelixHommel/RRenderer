#include "Window.hpp"

#include "exception/EngineException.hpp"
#include "exception/GLFWException.hpp"

#include "GLFW/glfw3.h"
#include <spdlog/spdlog.h>
#include <vulkan/vulkan_core.h>

#include <source_location>
#include <string>

namespace rr
{

Window::Window(int width, int height, const std::string& title)
    : m_window(nullptr)
    , m_width(width)
    , m_height(height)
    , m_title(title)
{
    if(glfwInit() == 0)
        throwWithLog<GLFWException>(std::source_location::current(), GLFWExceptionCause::GLFW_INIT_FAILED);

    spdlog::info("GLFW initialized successfully...");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if(m_window == nullptr)
        throwWithLog<GLFWException>(std::source_location::current(), GLFWExceptionCause::WINDOW_CREATION_FAILED);

    spdlog::info("Window({}, {}) created successfully...", m_width, m_height);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
}

void Window::createWindowSurface(VkInstance& instance, VkSurfaceKHR* surface)
{
    if(glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS)
        throwWithLog<GLFWException>(std::source_location::current(), GLFWExceptionCause::SURFACE_CREATION_FAILED);
}

} // !rr
