#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace vul {
	
	class VulWindow {
	public:
		VulWindow(int w, int h, std::string name);
		~VulWindow();

		VulWindow(const VulWindow &) = delete;
		VulWindow& operator = (const VulWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }
		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
		bool wasFrameBufferResized() { return frameBufferResized; }
		void resetFrameBufferResizedFlag() { frameBufferResized = false; }

		VkExtent2D getExtent(){ return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
	private:
		GLFWwindow *window;
		int width, height;
		bool frameBufferResized = false;
		std::string windowName;
		static void frameBufferResizedCallback(GLFWwindow* window, int w, int h);

		void initWindow();

	};

}