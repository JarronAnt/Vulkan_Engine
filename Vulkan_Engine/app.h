#pragma once

#include "vul_Window.h"
#include "Vul_Pipeline.h"
#include "Vul_Device.h"
#include "Vul_SwapChain.h"
#include <memory>
#include <vector>
#include "Vul_Model.h"

namespace vul {
	class app
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;
		void run();

		app();
		~app(); 

		//delete copy constructor 
		app(const app&) = delete;
		app& operator=(const app&) = delete;

	private:
		void recreateSwapchain();
		void recordCmdBuffer(int imageIndex);
		void createPipelineLayout();
		void createPipeline();
		void createCmdBuffers();
		void draw();
		void sierpinski(std::vector<VulModel::Vertex>& vertices, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top);
		void loadModels();

		VulWindow vulWindow{ WIDTH,HEIGHT, "Vulkan Window" };
		VulDevice vulDevice{ vulWindow };
		std::unique_ptr<SwapChain> vulSwapChain;
		std::unique_ptr<Vul_Pipeline> vulPipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> cmdBuffers;
		std::unique_ptr<VulModel>vulModel;

	};

}