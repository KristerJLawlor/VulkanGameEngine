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

		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

		bool wasWindowResized() { return framebufferResized; }
		void resetWindowResizedFlag() { framebufferResized = false; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:

		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
		void initWindow();	//helper function to initialize window

		int width;
		int height;
		bool framebufferResized = false;	//used as a flag to indicate the window has been resized

		std::string windowName;
		GLFWwindow* window;	//pointer to a GLFWWindow object

	};
}