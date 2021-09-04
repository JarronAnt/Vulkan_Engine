#include "vul_Window.h"


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

	void vul::VulWindow::initWindow()
	{
		glfwInit();
		//this line is to not use openGL stuff with glfw	
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	}
}