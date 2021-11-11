#include "app.h"
#include <stdexcept>


namespace vul {

	app::app() {
		createPipelineLayout();
		createPipeline();
		createCmdBuffers();
	}

	app::~app() {
		vkDestroyPipelineLayout(vulDevice.device(), pipelineLayout, nullptr);
	}
	
	void vul::app::run(){
		while (!vulWindow.shouldClose()) {
			glfwPollEvents();
		}
	}
	void app::createPipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(vulDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create pipline");
		}
	}
	void app::createPipeline()
	{
		auto pipelineCfg = Vul_Pipeline::defaultCfgInfo(vulSwapChain.width(), vulSwapChain.height());
		pipelineCfg.renderPass = vulSwapChain.getRenderPass();
		pipelineCfg.pipelineLayout = pipelineLayout;
		vulPipeline = std::make_unique<Vul_Pipeline>(vulDevice, "simple_shader.vert.spv", "simple_shader.frag.spv", pipelineCfg);
	}

	void app::createCmdBuffers()
	{
	}
	void app::draw()
	{
	}
}