#pragma once

#include "vul_Window.h"
#include "Vul_Pipeline.h"
#include "Vul_Device.h"


namespace vul {
	class app
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;
		void run();
	private:
		VulWindow vulWindow{ WIDTH,HEIGHT, "Vulkan Window" };
		VulDevice vulDevice{ vulWindow };
		Vul_Pipeline vulPipeline{vulDevice, "simple_shader.vert.spv", "simple_shader.frag.spv", Vul_Pipeline::defaultCfgInfo (WIDTH,HEIGHT)};
	};

}