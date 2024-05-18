#include "vge_app.hpp"

namespace vge {
	void VgeApp::run() {

		while (!vgeWindow.shouldClose()) {
			glfwPollEvents();
		}
	}
}