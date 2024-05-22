#pragma once

#include "vge_device.hpp"

#include <string>
#include <vector>

namespace vge {

	//data used to configure pipeline in input assembly stage
	struct PipelineConfigInfo {
		VkViewport viewport;
		VkRect2D scissor;
		
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class VgePipeline {
	public:
		//pipeline constructor
		VgePipeline(VgeDevice& device, 
			const std::string& vertFilepath, 
			const std::string& fragFilepath, 
			const PipelineConfigInfo& configInfo);

		//destructor
		~VgePipeline();
		//delete to avoid duplicating pointers to vulkan objects
		VgePipeline(const VgePipeline&) = delete;
		void operator=(const VgePipeline&) = delete;

		void bind(VkCommandBuffer commandBuffer);

		static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

	private:
		//function to read file
		static std::vector<char> readFile(const std::string& filepath);

		//helper function
		void createGraphicsPipeline(
			const std::string& vertFilepath, 
			const std::string& fragFilepath, 
			const PipelineConfigInfo& configInfo);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		VgeDevice& vgeDevice;	//stores device reference
		VkPipeline graphicsPipeline;	//handle to vulkan pipeline object
		VkShaderModule vertShadermodule;
		VkShaderModule fragShadermodule;

	};
}