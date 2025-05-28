#include "VulkanRenderer.hpp"
#include "window/Window.hpp"

#include <iostream>
#include <memory>
#include <string>

static constexpr int WINDOW_WIDTH{700};
static constexpr int WINDOW_HEIGHT{700};
static constexpr std::string WINDOW_TITLE{"title"};

int main()
{
    std::unique_ptr<rr::Window> w{ std::make_unique<rr::Window>(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE) };
    std::unique_ptr<rr::VulkanRenderer> r{ std::make_unique<rr::VulkanRenderer>(*w) };

    while(w->shouldClose() == 0)
    {
        glfwPollEvents();
        r->render();
    }
    r->shutdown();
    r.reset();

    w.reset();
    glfwTerminate();

    return 0;
}
