#include "app.h"
#include <stdexcept>
#include <array>

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
			draw();
		}

		vkDeviceWaitIdle(vulDevice.device());
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
		PipelineConfigInfo pipelineConfig{};
		Vul_Pipeline::defaultCfgInfo(
			pipelineConfig,
			vulSwapChain.width(),
			vulSwapChain.height());
		pipelineConfig.renderPass = vulSwapChain.getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		vulPipeline = std::make_unique<Vul_Pipeline>(
			vulDevice,
			"simple_shader.vert.spv",
			"simple_shader.frag.spv",
			pipelineConfig);
	}

	void app::createCmdBuffers()
	{
		cmdBuffers.resize(vulSwapChain.imageCount());
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = vulDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(cmdBuffers.size());

		if (vkAllocateCommandBuffers(vulDevice.device(), &allocInfo, cmdBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("Could not create command buffer");
		}

		for (int i = 0; i < cmdBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			if (vkBeginCommandBuffer(cmdBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("Could not record command buffer");
			}
			

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = vulSwapChain.getRenderPass();
			renderPassInfo.framebuffer = vulSwapChain.getFrameBuffer(i);
			renderPassInfo.renderArea.offset = { 0,0 };
			renderPassInfo.renderArea.extent = vulSwapChain.getSwapChainExtent();


			std::array<VkClearValue, 2> clearValues; {}
			clearValues[0].color = { 0.1f,0.1f,0.1f,1.0f };
			clearValues[1].depthStencil = { 1.0f,0 };

			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(cmdBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			vulPipeline->bind(cmdBuffers[i]);
			vkCmdDraw(cmdBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(cmdBuffers[i]);
			if (vkEndCommandBuffer(cmdBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer");
			}
		}
	}
	void app::draw()
	{
		uint32_t imageIndex;
		auto result = vulSwapChain.acquireNextImage(&imageIndex);
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image");
		}

		result = vulSwapChain.submitCommandBuffers(&cmdBuffers[imageIndex], &imageIndex);
		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}
	}
}