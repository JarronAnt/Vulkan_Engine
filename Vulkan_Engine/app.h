#pragma once

#include "vul_Window.h"


namespace vul {
	class app
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;
		void run();
	private:
		VulWindow vulWindow{ WIDTH,HEIGHT, "Vulkan Window" };
	};

}