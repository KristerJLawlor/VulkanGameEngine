#pragma once

#include "vge_window.hpp"
#include "vge_pipeline.hpp"
#include "vge_device.hpp"

namespace vge {
	class VgeApp {
		public:
			static constexpr int WIDTH = 800;
			static constexpr int HEIGHT = 600;

			void run();

		private:
			VgeWindow vgeWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
			VgeDevice vgeDevice{ vgeWindow };
			VgePipeline vgePipline{
				vgeDevice, 
				"simple_shader.vert.spv", "simple_shader.frag.spv", 
				VgePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};

	};
}