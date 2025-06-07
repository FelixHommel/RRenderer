#ifndef RRENDERER_ENGINE_CORE_VULKAN_INSTANCE_HPP
#define RRENDERER_ENGINE_CORE_VULKAN_INSTANCE_HPP

#include <vector>
#include <vulkan/vulkan_core.h>

namespace rr
{

class VulkanInstance
{
public:
    VulkanInstance();
    ~VulkanInstance();

    VulkanInstance(const VulkanInstance&) = delete;
    VulkanInstance(VulkanInstance&&) = delete;
    VulkanInstance& operator=(const VulkanInstance&) = delete;
    VulkanInstance& operator=(VulkanInstance&&) = delete;

    [[nodiscard]] VkInstance getHandle() const { return m_instance; }

private:
    VkInstance m_instance{ VK_NULL_HANDLE };

    static bool checkValidationLayerSupport();
    static std::vector<const char*> getRequiredExtensions();
    static void hasGLFWRequiredInstanceExtensions();
};

} // !rr

#endif // !RRENDERER_ENGINE_CORE_VULKAN_INSTANCE_HPP
