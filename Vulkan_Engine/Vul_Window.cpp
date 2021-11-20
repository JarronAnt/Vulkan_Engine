#include "vul_Window.h"
#include <stdexcept>


namespace vul {
	vul::VulWindow::VulWindow(int w, int h, std::string name) :width(w), height(h), windowName(name)
	{
		initWindow();
	}

	vul::VulWindow::~VulWindow()
	{
		glfwDestroyWindow(window); 
		glfwTerminate();
	}

	void VulWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance,window,nullptr,surface) != VK_SUCCESS) {

			throw std::runtime_error("error failed to ceate surface");
		}
	}

	void VulWindow::frameBufferResizedCallback(GLFWwindow* window, int w, int h)
	{
		auto myWin = reinterpret_cast<VulWindow*>(glfwGetWindowUserPointer(window));
		myWin->frameBufferResized = true;
		myWin->width = w;
		myWin->height = h;
	}

	void vul::VulWindow::initWindow()
	{
		glfwInit();
		//this line is to not use openGL stuff with glfw	
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, frameBufferResizedCallback);
	}
}