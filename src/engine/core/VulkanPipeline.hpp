#ifndef RRENDERER_ENGINE_CORE_VULKAN_PIPELINE_HPP
#define RRENDERER_ENGINE_CORE_VULKAN_PIPELINE_HPP

#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <filesystem>
#include <vector>

namespace rr
{

struct PipelineConfigInfo
{
    PipelineConfigInfo() = default;
    ~PipelineConfigInfo() = default;

    PipelineConfigInfo(PipelineConfigInfo&&) = delete;
    PipelineConfigInfo& operator=(PipelineConfigInfo&&) = delete;
    PipelineConfigInfo(const PipelineConfigInfo&) = delete;
    PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

    VkPipelineViewportStateCreateInfo viewportInfo{};
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
    VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
    VkPipelineMultisampleStateCreateInfo multisampleInfo{};
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
    VkPipelineLayout pipelineLayout{ nullptr };
    VkRenderPass renderPass{ nullptr };
    std::uint32_t subpass{ 0 };
};

class VulkanPipeline
{
public:
    VulkanPipeline(VkDevice device, const PipelineConfigInfo& configInfo, const std::filesystem::path& vertFilepath, const std::filesystem::path& fragFilepath);
    ~VulkanPipeline();

    VulkanPipeline(const VulkanPipeline&) = delete;
    VulkanPipeline(VulkanPipeline&&) = delete;
    VulkanPipeline& operator=(const VulkanPipeline&) = delete;
    VulkanPipeline& operator=(VulkanPipeline&&) = delete;

    static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

    void bind(VkCommandBuffer cmdBuffer);

private:
    VkDevice device;

    VkPipeline m_pipeline{ VK_NULL_HANDLE };
    VkShaderModule m_vertShaderModule{ VK_NULL_HANDLE };
    VkShaderModule m_fragShaderModule{ VK_NULL_HANDLE };

    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

    // TODO: move to dedicated file io class
    static std::vector<char> readFile(const std::filesystem::path& filepath);
};

} // !rr

#endif // !RRENDERER_ENGINE_CORE_VULKAN_PIPELINE_HPP
