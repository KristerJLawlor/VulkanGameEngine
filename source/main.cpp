#include "vge_app.hpp"

//std lib
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
	vge::VgeApp app{};	//create VgeApp object of 600x800 pixels

	try {
		app.run();	//run app to spawn window

	}
	catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
	return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

	/*
	1: Create a VgeApp object (contains window dimensions), 
		which creates a SwapChain, Pipeline, and Command Buffers, and loads Models.
		The VgeApp constructor will also create a VgeDevice object with the window dimensions, 
		   
	2: When the VgeDevice is instantiated alongside the VgeApp, it will
		create a Vulkan instance, set up validation layers, create a surface,
		pick a physical device, and create a logical device.
		The VgeDevice constructor will also create a command pool for command buffer allocation.

	3: run() will create a window and start the main loop, polling for events and drawing frames.

	*/

	/*
	The general pattern that object creation function parameters in Vulkan follow is:
		-Pointer to struct with creation info
		-Pointer to custom allocator callbacks, usually nullptr in this project
		-Pointer to the variable that stores the handle to the new object
	*/


/*
Graphics Pipeline:

<Vertex/Index Buffer {List of numbers denoting vertex positions} -> 
<Input Assembler {Takes list of numbers and groups them together in to geometry} -> 
<Vertex Shader {Processes each vertex individually and performs transformations} -> 
<Rasterization {Breaks up geometry such that it fits over an area of pixels} -> 
<Fragment Shader {Processes each fragment individually and outputs values such as 
	color by using interpolated data fromtextures, lighting, etc} ->
<Color Blending {Applies operations to mix the values of multiple fragments 
	that lie within the same pixel} ->
<Frame Buffer {Final image rendered on screen}
*/

/*
Swap Chain:

Series of frame buffers (objects) use to display images to the window.
Using two buffers is double buffering. Consists of a front and back buffer.
Each frame alternates between the front and back buffer.
The buffers are timed to the device's V-sync, 
so 60fps will swap the front and back buffer every 16.6ms, 
allowing for the non-rendered frame to pre-load in the GPU.

Lack of double buffering can cause tears on-screen.

If the GPU rendering can't keep up with the refresh rate of the display, adding a second back-buffer
at the cost of memory can prove beneficial. This is triple-buffering.
There is always a frame buffer being loaded, meaning frame hiccups are less likely.
*/