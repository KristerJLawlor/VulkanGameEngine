#pragma once

#include "vge_window.hpp"
#include "vge_pipeline.hpp"
#include "vge_device.hpp"
#include "vge_swapchain.hpp"
#include "vge_model.hpp"

//std lib
#include <memory>
#include <vector>

#include "glm/glm.hpp"

namespace vge {
	class VgeApp {
		//This class is the main application class that will handle the window, device, swap chain, pipeline, and command buffers.
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

		//members
			VgeWindow vgeWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
			VgeDevice vgeDevice{ vgeWindow };	//VgeDevice object that will be used to create the Vulkan instance
			std::unique_ptr<VgeSwapChain> vgeSwapChain;	//Use pointer instead of stack allocated variable, we can recreate the swap chain with a new W x H more easily

			std::unique_ptr<VgePipeline> vgePipeline;
			VkPipelineLayout pipelineLayout;
			std::vector<VkCommandBuffer> commandBuffers;
			std::unique_ptr<VgeModel> vgeModel;

		//functions
			void loadModels();
			void createPipelineLayout();
			void createPipeline();
			void createCommandBuffers();
			void freeCommandBuffers();
			void drawFrame();
			void recreateSwapChain();
			void recordCommandBuffer(int imageIndex);


	};
}