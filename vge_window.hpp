#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace vge {

	class VgeWindow {
	public:
		VgeWindow(int w, int h, std::string name);	//window constructor
		~VgeWindow();	//window destructor

		//delete copy constructor to prevent doubles
		VgeWindow(const VgeWindow&) = delete;
		//delete copy operator to prevent doubles
		VgeWindow& operator=(const VgeWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:

		void initWindow();	//helper function to initialize window

		const int width;
		const int height;

		std::string windowName;
		GLFWwindow* window;	//pointer to a GLFWWindow object

	};
}