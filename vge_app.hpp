#pragma once

#include "vge_window.hpp"
#include "vge_pipeline.hpp"

namespace vge {
	class VgeApp {
		public:
			static constexpr int WIDTH = 800;
			static constexpr int HEIGHT = 600;

			void run();

		private:
			VgeWindow VgeWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
			VgePipeline vgePipline{ "simple_shader.vert.spv", "simple_shader.frag.spv" };

	};
}