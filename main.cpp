#include "vge_app.hpp"

//std lib
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
	vge::VgeApp app{};

	try {
		app.run();
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
	return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}