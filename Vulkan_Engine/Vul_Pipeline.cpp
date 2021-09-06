#include "Vul_Pipeline.h"
#include <fstream>
#include <stdexcept>

#include <iostream>

namespace vul {
	Vul_Pipeline::Vul_Pipeline(VulDevice& device, 
		const std::string& vertFilePath, 
		const std::string& fragFilePath,
		const PipelineConfigInfo cfg) : vulDevice(device)
	{
		createGraphicsPipeline(vertFilePath, fragFilePath, cfg);
	}

	void Vul_Pipeline::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo cfg)
	{
		auto vertCode = readFile(vertFilePath);
		auto fragCode = readFile(fragFilePath);

		

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

		PipelineConfigInfo cfgInfo{};
		return cfgInfo;
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