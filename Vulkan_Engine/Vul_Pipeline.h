#pragma once
#include <string>
#include <vector>

namespace vul {
	class Vul_Pipeline
	{
	public:
		Vul_Pipeline(const std::string& vertFilePath, const std::string& fragFilePath); 

	private:
		static std::vector<char> readFile(const std::string& filePath);
		void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath);

	};

} 