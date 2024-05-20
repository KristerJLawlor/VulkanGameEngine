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
Graphics Pipeline:

Vertex/Index Buffer {List of numbers denoting vertex positions} -> 
Input Assembler {Takes list of numbers and groups them together in to geometry} -> 
Vertex Shader {Processes each vertex individually and performs transformations} -> 
Rasterization {Breaks up geometry such that it fits over an area of pixels} -> 
Fragment Shader {Processes each fragment individually and outputs values such as 
	color by using interpolated data fromtextures, lighting, etc} ->
Color Blending {Applies operations to mix the values of multiple fragments 
	that lie within the same pixel} ->
Frame Buffer {Final image rendered on screen}
*/