#include "app.h"


namespace vul {
	
	void vul::app::run(){
		while (!vulWindow.shouldClose()) {
			glfwPollEvents();
		}
	}
}