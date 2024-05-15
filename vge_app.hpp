#pragma once

#include "vge_window.hpp"

namespace vge {
	class VgeApp {
		public:
			static constexpr int WIDTH = 800;
			static constexpr int HEIGHT = 600;

			void run();

		private:
			VgeWindow VgeWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };

	};
}