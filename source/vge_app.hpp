#pragma once

#include "vge_window.hpp"
#include "vge_pipeline.hpp"
#include "vge_device.hpp"
#include "vge_swapchain.hpp"

//std lib
#include <memory>
#include <vector>

namespace vge {
	class VgeApp {
		public:
		//members
			static constexpr int WIDTH = 800;
			static constexpr int HEIGHT = 600;

		//functions
			VgeApp();
			~VgeApp();

			//delete copy constructor to prevent doubles
			VgeApp(const VgeApp&) = delete;
			//delete copy operator to prevent doubles
			VgeApp& operator=(const VgeApp&) = delete;

			void run();

		private:
		//functions
			void createPipelineLayout();
			void createPipeline();
			void createCommandBuffers();
			void drawFrame();

		//members
			VgeWindow vgeWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
			VgeDevice vgeDevice{ vgeWindow };
			VgeSwapChain vgeSwapChain{ vgeDevice, vgeWindow.getExtent() };

			std::unique_ptr<VgePipeline> vgePipeline;
			VkPipelineLayout pipelineLayout;
			std::vector<VkCommandBuffer> commandBuffers;

	};
}