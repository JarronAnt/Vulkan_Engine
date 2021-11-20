#include "app.h"
#include <stdexcept>
#include <array>

namespace vul {

	app::app() {
		loadModels();
		createPipelineLayout();
		recreateSwapchain();
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
			vulSwapChain->width(),
			vulSwapChain->height());
		pipelineConfig.renderPass = vulSwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		vulPipeline = std::make_unique<Vul_Pipeline>(
			vulDevice,
			"simple_shader.vert.spv",
			"simple_shader.frag.spv",
			pipelineConfig);
	}

	void app::recreateSwapchain() {
		auto extent = vulWindow.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = vulWindow.getExtent();
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(vulDevice.device());
		vulSwapChain = nullptr;
		vulSwapChain = std::make_unique<SwapChain>(vulDevice, extent);
		printf("swapchain made");
		createPipeline();
	}

	void app::createCmdBuffers()
	{
		cmdBuffers.resize(vulSwapChain->imageCount());
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = vulDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(cmdBuffers.size());

		if (vkAllocateCommandBuffers(vulDevice.device(), &allocInfo, cmdBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("Could not create command buffer");
		}
	}

	void app::recordCmdBuffer(int imageIndex) {
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		if (vkBeginCommandBuffer(cmdBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("Could not record command buffer");
		}


		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = vulSwapChain->getRenderPass();
		renderPassInfo.framebuffer = vulSwapChain->getFrameBuffer(imageIndex);
		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = vulSwapChain->getSwapChainExtent();


		std::array<VkClearValue, 2> clearValues; {}
		clearValues[0].color = { 0.1f,0.1f,0.1f,1.0f };
		clearValues[1].depthStencil = { 1.0f,0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(cmdBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vulPipeline->bind(cmdBuffers[imageIndex]);
		vulModel->bind(cmdBuffers[imageIndex]);
		vulModel->draw(cmdBuffers[imageIndex]);
		vkCmdEndRenderPass(cmdBuffers[imageIndex]);
		if (vkEndCommandBuffer(cmdBuffers[imageIndex]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer");
		}
	}
	
	void app::sierpinski(
		std::vector<VulModel::Vertex>& vertices,
		int depth,
		glm::vec2 left,
		glm::vec2 right,
		glm::vec2 top) {
		if (depth <= 0) {
			vertices.push_back({ top });
			vertices.push_back({ right });
			vertices.push_back({ left });
		}
		else {
			auto leftTop = 0.5f * (left + top);
			auto rightTop = 0.5f * (right + top);
			auto leftRight = 0.5f * (left + right);
			sierpinski(vertices, depth - 1, left, leftRight, leftTop);
			sierpinski(vertices, depth - 1, leftRight, right, rightTop);
			sierpinski(vertices, depth - 1, leftTop, rightTop, top);
		}
	}
	void app::loadModels()
	{
		std::vector<VulModel::Vertex> verts{ 
			{{0.0f,-0.5f},{1.0f,0.0f,0.0f}},
			{{0.5f,0.5f},{0.0f,1.0f,0.0f}},
			{{-0.5f,0.5f},{0.0f,0.0f,1.0f}} };
		//sierpinski(verts, 5, { -0.5f, 0.5f }, { 0.5f, 0.5f }, { 0.0f, -0.5f });
		vulModel = std::make_unique<VulModel>(vulDevice, verts);
	}

	void app::draw()
	{
		uint32_t imageIndex;
		auto result = vulSwapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapchain();
			return;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image");
		}

		recordCmdBuffer(imageIndex);
		result = vulSwapChain->submitCommandBuffers(&cmdBuffers[imageIndex], &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || vulWindow.wasFrameBufferResized()) {
			vulWindow.resetFrameBufferResizedFlag();
			recreateSwapchain();
			return;
		}

		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}
	}
}