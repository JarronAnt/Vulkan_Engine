#pragma once

#include "vul_Window.h"
#include "Vul_Pipeline.h"


namespace vul {
	class app
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;
		void run();
	private:
		VulWindow vulWindow{ WIDTH,HEIGHT, "Vulkan Window" };
		Vul_Pipeline vulPipeline{ "simple_shader.vert.spv", "simple_shader.frag.spv" };
	};

}