#ifndef RRENDERER_ENGINE_WINDOW_WINDOW_HPP
#define RRENDERER_ENGINE_WINDOW_WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdint>
#include <string>

namespace rr
{

/**
 *  The Window class opens a GLFW window. The Window is bound to GLFW meaning if the window is created, GLFW
 *  is initialized and if the window is destroyed GLFW is terminated.
*/
class Window
{
public:
    /**
     *  minimal window constructor
     *
     *  @param width - width of the window
     *  @param height - height of the window
     *  @param title - title of the window
     *  @throws std::runtime_error if anything goes wrong while creation
    */
    Window(int width, int height, const std::string& title);
    ~Window();

    Window(const Window&) = delete;
    Window(Window&&) noexcept = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) noexcept = delete;

    [[nodiscard]] int shouldClose() const { return glfwWindowShouldClose(m_window); }
    [[nodiscard]] VkExtent2D getExtent() const { return { static_cast<std::uint32_t>(m_width), static_cast<std::uint32_t>(m_height) }; }

    [[nodiscard]] GLFWwindow* getWindowHandle() const { return m_window; }

    void createWindowSurface(VkInstance& instance, VkSurfaceKHR* surface);

private:
    GLFWwindow* m_window;

    int m_width;
    int m_height;
    std::string m_title;
};

} // !rr

#endif // !RRENDERER_ENGINE_WINDOW_WINDOW_HPP
