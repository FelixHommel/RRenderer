#include "Window.hpp"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan_core.h>

#include <stdexcept>
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
        throw std::runtime_error("ERROR: Failed to initialize GLFW");

    spdlog::info("GLFW initialized successfully...");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if(m_window == nullptr)
        throw std::runtime_error("ERROR: Failed to create a GLFW window");

    spdlog::info("Window({}, {}) created successfully...", m_width, m_height);
}

Window::~Window()
{
    spdlog::info("Shutting down...");
    glfwDestroyWindow(m_window);
}

void Window::createWindowSurface(VkInstance& instance, VkSurfaceKHR* surface)
{
    if(glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS)
        throw std::runtime_error("GLFW failed to create window surface");
}

} // !rr
