#ifndef RRENDERER_ENGINE_WINDOW_WINDOW_HPP
#define RRENDERER_ENGINE_WINDOW_WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <string>

namespace rr
{

/// \brief Window to which can be drawn
///
/// The Window class opens a GLFW window. The Window is bound to GLFW meaning if the window is created, GLFW
/// is initialized and if the window is destroyed GLFW is terminated.
///
/// \author Felix Hommel
/// \date 7/18/2025
class Window
{
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) noexcept = delete;
    Window& operator=(Window&&) noexcept = delete;

    [[nodiscard]] GLFWwindow* getWindowHandle() const { return m_window; }
    [[nodiscard]] int shouldClose() const { return glfwWindowShouldClose(m_window); }
    [[nodiscard]] VkExtent2D getExtent() const { return { static_cast<std::uint32_t>(m_width), static_cast<std::uint32_t>(m_height) }; }
    [[nodiscard]] bool wasWindowResized() const { return m_framebufferResized; }

    void resetWindowResized() { m_framebufferResized = false; }

    void createWindowSurface(VkInstance& instance, VkSurfaceKHR* surface) const;

private:
    // TODO: Convert to a std::unique_ptr<GLFWwindow>
    GLFWwindow* m_window;

    int m_width;
    int m_height;
    std::string m_title;
    bool m_framebufferResized{ false };

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};

} // !rr

#endif // !RRENDERER_ENGINE_WINDOW_WINDOW_HPP
