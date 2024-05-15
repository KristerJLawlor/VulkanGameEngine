#include "vge_app.hpp"

namespace vge {
	void VgeApp::run() {

		while (!VgeWindow.shouldClose()) {
			glfwPollEvents();
		}
	}
}