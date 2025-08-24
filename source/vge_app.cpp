#include "vge_app.hpp"

//std lib
#include <stdexcept>
#include <array>
#include <cassert>

//external libraries
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>


namespace vge {

	struct SimplePushConstantData 
	{
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};


	VgeApp::VgeApp() 
	{
		loadModels();	//load in the models for our application
		createPipelineLayout();	//predefines the pipeline layout for the shaders
		recreateSwapChain();	//creates the swap chain, then creates the pipeline
		createCommandBuffers();	//creates the command buffers for the swap chain images
	}


	VgeApp::~VgeApp() 
	{
		vkDestroyPipelineLayout(vgeDevice.device(), pipelineLayout, nullptr);
	}


	void VgeApp::run() 
	{
		while (!vgeWindow.shouldClose()) {
			glfwPollEvents();
			drawFrame();
		}

		vkDeviceWaitIdle(vgeDevice.device());
	}


	void VgeApp::loadModels() 
	{
		std::vector<VgeModel::Vertex> vertices{
	  {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	  {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
	  {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}} };

		vgeModel = std::make_unique<VgeModel>(vgeDevice, vertices);
	}


	void VgeApp::createPipelineLayout() 
	{

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		if (vkCreatePipelineLayout(vgeDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
			VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}

	//Called when a window resize is detected
	void VgeApp::recreateSwapChain() 
	{
		auto extent = vgeWindow.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = vgeWindow.getExtent();
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(vgeDevice.device());

		//check if render pass is compatible
		if (vgeSwapChain == nullptr) {
			vgeSwapChain = std::make_unique<VgeSwapChain>(vgeDevice, extent);
		}
		else {
			vgeSwapChain = std::make_unique<VgeSwapChain>(vgeDevice, extent, std::move(vgeSwapChain));
			if (vgeSwapChain->imageCount() != commandBuffers.size()) {
				freeCommandBuffers();
				createCommandBuffers();
			}
		}
		createPipeline();
	}


	void VgeApp::createPipeline() 
	{

		assert(vgeSwapChain != nullptr && "Cannot create pipeline before swap chain");
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		VgePipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = vgeSwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		vgePipeline = std::make_unique<VgePipeline>(
			vgeDevice,
			"shaders/simple_shader.vert.spv",
			"shaders/simple_shader.frag.spv",
			pipelineConfig);
	}


	void VgeApp::freeCommandBuffers() 
	{
		vkFreeCommandBuffers(
			vgeDevice.device(),
			vgeDevice.getCommandPool(),
			static_cast<uint32_t>(commandBuffers.size()),
			commandBuffers.data());

		commandBuffers.clear();
	}

	//Command buffers allow sequence of commands to be recorded and submitted to the GPU for execution.
	//Can be reused for multiple frames, which is more efficient than recording commands every frame (OpenGL does this).
	void VgeApp::createCommandBuffers() 
	{
		//resize the command buffers vector to the size of swap chain image count
		//2 for double buffering, 3 for triple buffering, etc.
		commandBuffers.resize(vgeSwapChain->imageCount());

		//Allocate command buffers
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = vgeDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(vgeDevice.device(), &allocInfo, commandBuffers.data()) !=
			VK_SUCCESS)
		{
			throw::std::runtime_error("failed to allocate command buffers!");
		}
	}


	void VgeApp::recordCommandBuffer(int imageIndex) 
	{

		static int frame = 30;
		frame = (frame + 1) % 100;

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		//First command to record is the render pass begin command.
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = vgeSwapChain->getRenderPass();
		renderPassInfo.framebuffer = vgeSwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = vgeSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };

		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(vgeSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(vgeSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, vgeSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

		vgePipeline->bind(commandBuffers[imageIndex]);	//binds the pipeline using function in vge_pipeline.cpp
		vgeModel->bind(commandBuffers[imageIndex]);

		for (int j = 0; j < 4; j++) {
			SimplePushConstantData push{};
			push.offset = { -0.5f + frame * 0.02f, -0.4f + j * 0.25f };
			push.color = { 0.0f, 0.0f, 0.2f + 0.2f * j };

			vkCmdPushConstants(
				commandBuffers[imageIndex],
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(SimplePushConstantData),
				&push);
			vgeModel->draw(commandBuffers[imageIndex]);
		}

		vkCmdEndRenderPass(commandBuffers[imageIndex]);
		if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}


	void VgeApp::drawFrame() 
	{
		uint32_t imageIndex;
		auto result = vgeSwapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		recordCommandBuffer(imageIndex);	//Must record command buffer before submitting it to the queue.

		// submit the provided command buffer to the graphics device queue
		// while handling CPU and GPU synchronization. 
		// The command buffer will then be executed followed by the Swap Chain will present the associated image to the screen.
		result = vgeSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
			vgeWindow.wasWindowResized()) {
			vgeWindow.resetWindowResizedFlag();
			recreateSwapChain();
			return;
		}
		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to preset swap chain image");
		}
	}
}