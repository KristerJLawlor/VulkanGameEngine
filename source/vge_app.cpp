#include "vge_app.hpp"

//std lib
#include <stdexcept>

namespace vge {

	VgeApp::VgeApp() {
		createPipelineLayout();
		createPipeline();
		createCommandBuffers();
	}

	VgeApp::~VgeApp() {
		vkDestroyPipelineLayout(vgeDevice.device(), pipelineLayout, nullptr);
	}

	void VgeApp::run() {

		while (!vgeWindow.shouldClose()) {
			glfwPollEvents();
		}
	}

	void VgeApp::createPipelineLayout() {
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(vgeDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
			VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}

	void VgeApp::createPipeline() {

		auto pipelineConfig =
			VgePipeline::defaultPipelineConfigInfo(vgeSwapChain.width(), vgeSwapChain.height());

		//A renderpass acts as a blueprint to tell the graphics pipeline object what to expect for the output frame buffer 
		pipelineConfig.renderPass = vgeSwapChain.getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		vgePipeline = std::make_unique<VgePipeline>(
			vgeDevice,
			"shaders/simple_shader.vert.spv",
			"shaders/simple_shader.frag.spv",
			pipelineConfig);
	}

	void VgeApp::createCommandBuffers() {

	}

	void VgeApp::drawFrame() {

	}
}