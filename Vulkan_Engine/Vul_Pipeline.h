#pragma once
#include <string>
#include <vector>
#include "Vul_Device.h"

namespace vul {

	struct PipelineConfigInfo {

	};

	class Vul_Pipeline
	{
	public:
		Vul_Pipeline(VulDevice &device, const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo cfg);
		~Vul_Pipeline() {}

		Vul_Pipeline(const Vul_Pipeline&) = delete;
		void operator=(const Vul_Pipeline&) = delete;

		static PipelineConfigInfo defaultCfgInfo(uint32_t width, uint32_t height);

	private:
		static std::vector<char> readFile(const std::string& filePath);
		void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo cfg);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* module);

		VulDevice& vulDevice;
		VkPipeline grpahicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
};

} 