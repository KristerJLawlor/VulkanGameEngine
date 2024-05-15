#include "vge_window.hpp"

namespace vge {
	//window constructor
	VgeWindow::VgeWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name } {
		//call window initialize function
		initWindow();
	}

	VgeWindow::~VgeWindow() {
		glfwDestroyWindow(window);	//destroy window
		glfwTerminate();
	}

	void VgeWindow::initWindow() {
		glfwInit();	//initialize the GLFW Library
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);	//tell GLFW to not make OpenGL context when initialized
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);		//disable window from being resized

		//initialize window pointer by calling the GLFW Create Window command
		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);	//copy constructor must be deleted after invokation
	}
}