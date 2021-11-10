#include "Vul_Pipeline.h"
#include <fstream>
#include <stdexcept>
#include <cassert>
#include <iostream>

namespace vul {
	Vul_Pipeline::Vul_Pipeline(VulDevice& device, 
		const std::string& vertFilePath, 
		const std::string& fragFilePath,
		const PipelineConfigInfo cfg) : vulDevice(device)
	{
		createGraphicsPipeline(vertFilePath, fragFilePath, cfg);
	}

	Vul_Pipeline::~Vul_Pipeline() {
		vkDestroyShaderModule(vulDevice.device(), vertShaderModule, nullptr);
		vkDestroyShaderModule(vulDevice.device(), fragShaderModule, nullptr);
		vkDestroyPipeline(vulDevice.device(), graphicsPipeline,nullptr); 

	}

	void Vul_Pipeline::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo cfg)
	{
		assert(cfg.pipelineLayout != VK_NULL_HANDLE);
		assert(cfg.renderPass != VK_NULL_HANDLE);
		auto vertCode = readFile(vertFilePath);
		auto fragCode = readFile(fragFilePath);

		createShaderModule(vertCode, &vertShaderModule);
		createShaderModule(fragCode, &fragShaderModule);

		VkPipelineShaderStageCreateInfo shaderStages[2];

		shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[0].module = vertShaderModule;
		shaderStages[0].pName = "main";
		shaderStages[0].flags = 0;
		shaderStages[0].pNext = nullptr;
		shaderStages[0].pSpecializationInfo = nullptr;

		shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStages[1].module = fragShaderModule;
		shaderStages[1].pName = "main";
		shaderStages[1].flags = 0;
		shaderStages[1].pNext = nullptr;
		shaderStages[1].pSpecializationInfo = nullptr;

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr;
		vertexInputInfo.pVertexBindingDescriptions = nullptr;

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &cfg.inputAssemblyInfo;
		pipelineInfo.pViewportState = &cfg.viewportInfo;
		pipelineInfo.pRasterizationState = &cfg.rasterizationInfo;
		pipelineInfo.pMultisampleState = &cfg.multisampleInfo;
		pipelineInfo.pColorBlendState = &cfg.colorBlendInfo;
		pipelineInfo.pDynamicState = nullptr;  // Optional
		pipelineInfo.pDepthStencilState = &cfg.depthStencilInfo;

		pipelineInfo.layout = cfg.pipelineLayout;
		pipelineInfo.renderPass = cfg.renderPass;
		pipelineInfo.subpass = cfg.subpass;

		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;  // Optional
		pipelineInfo.basePipelineIndex = -1;               // Optional

		if (vkCreateGraphicsPipelines(
			vulDevice.device(),
			VK_NULL_HANDLE,
			1,
			&pipelineInfo,
			nullptr,
			&graphicsPipeline) != VK_SUCCESS) {
			throw std::runtime_error("failed to create graphics pipeline!");
		}

		vkDestroyShaderModule(vulDevice.device(), fragShaderModule, nullptr);
		vkDestroyShaderModule(vulDevice.device(), vertShaderModule, nullptr);
		fragShaderModule = VK_NULL_HANDLE;
		vertShaderModule = VK_NULL_HANDLE;
	}

	void Vul_Pipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* module)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule ( vulDevice.device(), &createInfo,nullptr,module ) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create shader module");
		}
	}

	PipelineConfigInfo Vul_Pipeline::defaultCfgInfo(uint32_t width, uint32_t height)
	{

		PipelineConfigInfo cfg{};
		cfg.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		cfg.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		cfg.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		cfg.viewport.x = 0.0f;
		cfg.viewport.y = 0.0f;
		cfg.viewport.width = static_cast<float>(width);
		cfg.viewport.height = static_cast<float>(height);
		cfg.viewport.minDepth = 0.0f;
		cfg.viewport.maxDepth = 1.0f;

		cfg.scissor.offset = { 0,0 };
		cfg.scissor.extent = { width,height};

		cfg.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		cfg.viewportInfo.viewportCount = 1;
		cfg.viewportInfo.pViewports = &cfg.viewport;
		cfg.viewportInfo.scissorCount = 1;
		cfg.viewportInfo.pScissors = &cfg.scissor;

		cfg.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		cfg.rasterizationInfo.depthClampEnable = VK_FALSE;
		cfg.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		cfg.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		cfg.rasterizationInfo.lineWidth = 1.0f;
		cfg.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		cfg.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		cfg.rasterizationInfo.depthBiasEnable = VK_FALSE;
		cfg.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
		cfg.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
		cfg.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

		cfg.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		cfg.multisampleInfo.sampleShadingEnable = VK_FALSE;
		cfg.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		cfg.multisampleInfo.minSampleShading = 1.0f;           // Optional
		cfg.multisampleInfo.pSampleMask = nullptr;             // Optional
		cfg.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
		cfg.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

		cfg.colorBlendAttachment.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT;
		cfg.colorBlendAttachment.blendEnable = VK_FALSE;
		cfg.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		cfg.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		cfg.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
		cfg.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		cfg.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		cfg.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

		cfg.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		cfg.colorBlendInfo.logicOpEnable = VK_FALSE;
		cfg.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
		cfg.colorBlendInfo.attachmentCount = 1;
		cfg.colorBlendInfo.pAttachments = &cfg.colorBlendAttachment;
		cfg.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
		cfg.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
		cfg.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
		cfg.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

		cfg.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		cfg.depthStencilInfo.depthTestEnable = VK_TRUE;
		cfg.depthStencilInfo.depthWriteEnable = VK_TRUE;
		cfg.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		cfg.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		cfg.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
		cfg.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
		cfg.depthStencilInfo.stencilTestEnable = VK_FALSE;
		cfg.depthStencilInfo.front = {};  // Optional
		cfg.depthStencilInfo.back = {};   // Optional



		return cfg;
	}

	std::vector<char> Vul_Pipeline::readFile(const std::string& filePath)
	{
		std::ifstream file{ filePath, std::ios::ate | std::ios::binary};

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file " + filePath);
		}

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}
}