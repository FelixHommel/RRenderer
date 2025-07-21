#include "VulkanPipeline.hpp"

#include "VulkanMesh.hpp"
#include "exception/EngineException.hpp"
#include "exception/FileIOException.hpp"
#include "exception/VulkanException.hpp"
#include "spdlog/spdlog.h"
#include <source_location>
#include <vulkan/vulkan_core.h>

#include <array>
#include <cassert>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ios>
#include <vector>

namespace rr
{

/// \brief Create a new \ref VulkanPipeline
///
/// \param device the `VkDevice` where the Pipeline is running on
/// \param configInfo `PipelineConfigInfo&` which provides configuration of the `VulkanPipeline`
/// \param vertFilepath `std::filesystem::path&` to the vertex shader (.spv compiled)
/// \param fragFilepath `std::filesystem::path&` to the vertex shader (.spv compiled)
///
/// \throws \ref VulkanException if there was an error during pipeline creation
VulkanPipeline::VulkanPipeline(VkDevice device, const PipelineConfigInfo& configInfo, const std::filesystem::path& vertFilepath, const std::filesystem::path& fragFilepath)
    : device(device)
{
    assert(configInfo.pipelineLayout != VK_NULL_HANDLE && "Cannot create graphics pipeline: no pipeline layout provided");
    assert(configInfo.renderPass != VK_NULL_HANDLE && "Cannot create graphics pipeline: no renderPass provided");

    createShaderModule(readFile(vertFilepath), &m_vertShaderModule);
    createShaderModule(readFile(fragFilepath), &m_fragShaderModule);

    const std::array<VkPipelineShaderStageCreateInfo,2 > shaderStages{
        {
            {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .flags = 0,
                .stage = VK_SHADER_STAGE_VERTEX_BIT,
                .module = m_vertShaderModule,
                .pName = "main"
            },
            {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .flags = 0,
                .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
                .module = m_fragShaderModule,
                .pName = "main"
            }
        }
    };

    const auto bindingDescriptions = Vertex::getBindingDescriptions();
    const auto attributeDescriptions = Vertex::getAttributeDescriptions();
    const VkPipelineVertexInputStateCreateInfo vertexInputInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = static_cast<std::uint32_t>(bindingDescriptions.size()),
        .pVertexBindingDescriptions = bindingDescriptions.data(),
        .vertexAttributeDescriptionCount = static_cast<std::uint32_t>(attributeDescriptions.size()),
        .pVertexAttributeDescriptions = attributeDescriptions.data()
    };

    const VkGraphicsPipelineCreateInfo pipelineInfo{
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = 2,
        .pStages = shaderStages.data(),
        .pVertexInputState = &vertexInputInfo,
        .pInputAssemblyState = &configInfo.inputAssemblyInfo,
        .pViewportState = &configInfo.viewportInfo,
        .pRasterizationState = &configInfo.rasterizationInfo,
        .pMultisampleState = &configInfo.multisampleInfo,
        .pDepthStencilState = &configInfo.depthStencilInfo,
        .pColorBlendState = &configInfo.colorBlendInfo,
        .pDynamicState = &configInfo.dynamicStateInfo,
        .layout = configInfo.pipelineLayout,
        .renderPass = configInfo.renderPass,
        .subpass = configInfo.subpass,
        .basePipelineHandle = VK_NULL_HANDLE,
        .basePipelineIndex = -1
    };

    if(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline) != VK_SUCCESS)
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::CREATE_GRAPHICS_PIPELINE);

    spdlog::info("Created graphics pipeline successfully...");
}

VulkanPipeline::~VulkanPipeline()
{
    vkDestroyShaderModule(device, m_vertShaderModule, nullptr);
    vkDestroyShaderModule(device, m_fragShaderModule, nullptr);

    vkDestroyPipeline(device, m_pipeline, nullptr);
}

/// \brief Provide a default configuration for a `VkPipeline`
///
/// \param[out] configInfo `PipelineConfigInfo&` where the configuration will be stored
void VulkanPipeline::defaultPipelineConfigInfo(PipelineConfigInfo& configInfo)
{
    configInfo.inputAssemblyInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE
    };
    configInfo.viewportInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .pViewports = nullptr,
        .scissorCount = 1,
        .pScissors = nullptr
    };
    configInfo.rasterizationInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_NONE,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 0.f,
        .depthBiasClamp = 0.f,
        .lineWidth = 1.f
    };
    configInfo.multisampleInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE,
        .minSampleShading = 1.f,
        .pSampleMask = nullptr,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable = VK_FALSE
    };
    configInfo.colorBlendAttachment = {
        .blendEnable = VK_FALSE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp = VK_BLEND_OP_ADD,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };
    configInfo.colorBlendInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments = &configInfo.colorBlendAttachment,
        .blendConstants = { 0.f, 0.f, 0.f, 0.f }
    };
    configInfo.depthStencilInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .depthTestEnable = VK_TRUE,
        .depthWriteEnable = VK_TRUE,
        .depthCompareOp = VK_COMPARE_OP_LESS,
        .depthBoundsTestEnable = VK_FALSE,
        .stencilTestEnable = VK_FALSE,
        .front = {},
        .back = {},
        .minDepthBounds = 0.f,
        .maxDepthBounds = 1.f
    };
    configInfo.dynamicStateEnables = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    configInfo.dynamicStateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = static_cast<std::uint32_t>(configInfo.dynamicStateEnables.size()),
        .pDynamicStates = configInfo.dynamicStateEnables.data()
    };
}

/// \brief Bind the Pipeline for use in \p cmdBuffer
///
/// \param cmdBuffer the `VkCommandBuffer` that the Pipeline will be bound to
void VulkanPipeline::bind(VkCommandBuffer cmdBuffer) const
{
    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
}

/// \brief Create a shader module from \p code
///
/// \param code the source code of the shader
/// \param[out] shaderModule the 'VkShaderModule' handle where the created shader will be saved
///
/// \throws \ref VulkanException if there was an error while creating the module
void VulkanPipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) const
{
    const VkShaderModuleCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = code.size(),
        .pCode = reinterpret_cast<const std::uint32_t*>(code.data())
    };

    if(vkCreateShaderModule(device, &createInfo, nullptr, shaderModule) != VK_SUCCESS)
        throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::CREATE_SHADER_MODULE);
}

/// \brief Read a file and return it's content
///
/// \param filepath `std::filesystem::path&` to the file
///
/// \returns std::vector containing the file's content as `char`
/// \throws \ref FileIOException if there was an error while reading the file
std::vector<char> VulkanPipeline::readFile(const std::filesystem::path& filepath)
{
    std::ifstream file(filepath, std::ios::ate | std::ios::binary);

    if(!file.is_open())
        throwWithLog<FileIOException>(std::source_location::current(), filepath);

    const std::streamsize fileSize{ file.tellg() };
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

} // !rr
