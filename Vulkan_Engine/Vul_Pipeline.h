#pragma once
#include <string>
#include <vector>
#include "Vul_Device.h"

namespace vul {

	struct PipelineConfigInfo {
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo() = default;

		//VkViewport viewport;
		//VkRect2D scissor;

		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		std::vector<VkDynamicState>dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		uint32_t subpass = 0;
	};


	class Vul_Pipeline
	{
	public:
		Vul_Pipeline(VulDevice &device, const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo &cfg);
		~Vul_Pipeline();

		Vul_Pipeline(const Vul_Pipeline&) = delete;
		Vul_Pipeline& operator=(const Vul_Pipeline&) = delete;
		Vul_Pipeline() = default;

		static void defaultCfgInfo(PipelineConfigInfo& cfg);

		void bind(VkCommandBuffer cmdBuffer);

	private:
		static std::vector<char> readFile(const std::string& filePath);
		void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo &cfg);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* module);

		VulDevice& vulDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
};

} 