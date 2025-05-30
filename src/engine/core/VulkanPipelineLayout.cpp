#include "VulkanPipelineLayout.hpp"
#include "spdlog/spdlog.h"
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace rr
{

VulkanPipelineLayout::VulkanPipelineLayout(VkDevice device)
    : device(device)
{
    VkPipelineLayoutCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 0,
        .pSetLayouts = nullptr,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr
    };

    if(vkCreatePipelineLayout(device, &createInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
    {
        spdlog::critical("Failure while creaing pipeline layout");
        throw std::runtime_error("Failed to create pipeline layout");
    }

    spdlog::info("Created pipeline layout successfully...");
}

VulkanPipelineLayout::~VulkanPipelineLayout()
{
    vkDestroyPipelineLayout(device,  m_pipelineLayout, nullptr);
}

} // !rr
