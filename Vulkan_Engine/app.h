#pragma once

#include "vul_Window.h"
#include "Vul_Pipeline.h"
#include "Vul_Device.h"
#include "Vul_SwapChain.h"
#include <memory>
#include <vector>

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
		void createPipelineLayout();
		void createPipeline();
		void createCmdBuffers();
		void draw();

		VulWindow vulWindow{ WIDTH,HEIGHT, "Vulkan Window" };
		VulDevice vulDevice{ vulWindow };
		SwapChain vulSwapChain{ vulDevice,vulWindow.getExtent() };
		std::unique_ptr<Vul_Pipeline> vulPipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> cmdBuffers;

	};

}