#include "VulkanRenderer.hpp"
#include "window/Window.hpp"

#include <string>

static constexpr int WINDOW_WIDTH{700};
static constexpr int WINDOW_HEIGHT{700};
static constexpr std::string WINDOW_TITLE{"title"};

int main()
{
    rr::Window w{WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE};
    rr::VulkanRenderer r{w};

    while(w.shouldClose() == 0)
    {
        glfwPollEvents();
    }

    return 0;
}
