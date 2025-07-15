#include "VulkanPipelineLayout.hpp"
#include "exception/EngineException.hpp"
#include "exception/VulkanException.hpp"
#include "spdlog/spdlog.h"
#include <source_location>
#include <vulkan/vulkan_core.h>

namespace rr
{

VulkanPipelineLayout::VulkanPipelineLayout(VkDevice device)
    : device(device)
{
    VkPushConstantRange pushConstantRange{
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        .offset = 0,
        .size = sizeof(SimplePushConstantData)
    };

    VkPipelineLayoutCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 0,
        .pSetLayouts = nullptr,
        .pushConstantRangeCount = 1,
        .pPushConstantRanges = &pushConstantRange
    };

    if(vkCreatePipelineLayout(device, &createInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::CREATE_PIPELINE_LAYOUT);

    spdlog::info("Created pipeline layout successfully...");
}

VulkanPipelineLayout::~VulkanPipelineLayout()
{
    vkDestroyPipelineLayout(device,  m_pipelineLayout, nullptr);
}

} // !rr
