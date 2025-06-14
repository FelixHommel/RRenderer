#include "VulkanInstance.hpp"

#include "constants.hpp"

#include "GLFW/glfw3.h"
#include "exception/EngineException.hpp"
#include "exception/VulkanException.hpp"
#include "spdlog/spdlog.h"
#include "utility/StringHash.hpp"
#include <source_location>
#include <unordered_set>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <cstring>
#include <functional>
#include <span>
#include <string>
#include <vector>

namespace rr
{

namespace
{

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT /*messageType*/,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* /*pUserData*/)
{
    switch(messageSeverity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
            spdlog::error("validation layer: {}", pCallbackData->pMessage);
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
            spdlog::warn("validation layer: {}", pCallbackData->pMessage);
            break;
        }
        default: {
            spdlog::info("validation layer: {}", pCallbackData->pMessage);
            break;
        }
    }

    return VK_FALSE;
}

}

VulkanInstance::VulkanInstance()
{
    if(useValidationLayers && !checkValidationLayerSupport())
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::VALIDATION_LAYERS_UNAVAILABLE);

    VkApplicationInfo appInfo{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "RRenderer Application",
        .pEngineName = "RRenderer",
        .engineVersion = VK_MAKE_VERSION(0, 0, 1),
        .apiVersion = VK_API_VERSION_1_0
    };

    auto extensions{ getRequiredExtensions() };
    VkInstanceCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = static_cast<std::uint32_t>(extensions.size()),
        .ppEnabledExtensionNames = extensions.data()
    };

    if(useValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<std::uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = debugCallback,
            .pUserData = nullptr
        };

        createInfo.pNext = &debugCreateInfo;
    }

    if(vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::CREATE_INSTANCE);

    hasGLFWRequiredInstanceExtensions();
    spdlog::info("Instance created successfully...");
}

VulkanInstance::~VulkanInstance()
{
    vkDestroyInstance(m_instance, nullptr);
}

/**
 *  Check if all validation layers are available to use.
 *
 *  @return true if all requested validation layers are available, otherwise false
*/
bool VulkanInstance::checkValidationLayerSupport()
{
    std::uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for(const char* layerName : validationLayers)
    {
        bool layerFound{false};
        for(const auto& layerProp : availableLayers)
        {
            if(std::strcmp(layerName, layerProp.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if(!layerFound)
            return false;
    }

    return true;
}

/**
 *  Collect all required extensions by any parts of the renderer.
 *
 *  @return std::vector of required extension name strings
*/
std::vector<const char*> VulkanInstance::getRequiredExtensions()
{
    std::uint32_t glfwExtensionCount{ 0 };
    auto* glfwExtensions{ glfwGetRequiredInstanceExtensions(&glfwExtensionCount) };
    std::span<const char*> extensionSpan(glfwExtensions, glfwExtensionCount);

    std::vector<const char*> extensions(extensionSpan.begin(), extensionSpan.end());
    if(useValidationLayers)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return extensions;
}

/**
 *  Check if the instance supports all the extensions that are requried by GLFW.
 */
void VulkanInstance::hasGLFWRequiredInstanceExtensions()
{
    std::uint32_t extensionCount{0};
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    spdlog::info("available extensions: {}", extensionCount);
    std::unordered_set<std::string, StringHash, std::equal_to<>> available;
    for(const auto& extension : extensions)
    {
        spdlog::info("\t{}", extension.extensionName);
        available.emplace(extension.extensionName);
    }

    spdlog::info("required extensions:");
    auto requiredExtensions{ getRequiredExtensions() };
    for(const auto& required : requiredExtensions)
    {
        spdlog::info("\t{}", required);

        if(!available.contains(required))
            throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::GLFW_EXTENSIONS_MISSING);
    }
}

} // !rr
