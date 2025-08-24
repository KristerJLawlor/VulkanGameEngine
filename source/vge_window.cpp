#include "vge_window.hpp"

//std lib
#include <stdexcept>
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
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);		//disable window from being resized

		//initialize window pointer by calling the GLFW Create Window command
		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);	//copy constructor must be deleted after invokation
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}

	void VgeWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create window surface");
		}
	}

	void VgeWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height) 
	{
		auto vgeWindow = reinterpret_cast<VgeWindow*>(glfwGetWindowUserPointer(window));
		vgeWindow->framebufferResized = true;
		vgeWindow->width = width;
		vgeWindow->height = height;
	}
}