#pragma once

#include "vge_device.hpp"

#include <string>
#include <vector>

namespace vge {

	//data used to configure pipeline in input assembly stage
	struct PipelineConfigInfo {
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
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
		VgePipeline &operator=(const VgePipeline&) = delete;

		void bind(VkCommandBuffer commandBuffer);

		static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

	private:
		//function to read shader files
		static std::vector<char> readFile(const std::string& filepath);

		//helper function
		void createGraphicsPipeline(
			const std::string& vertFilepath, 
			const std::string& fragFilepath, 
			const PipelineConfigInfo& configInfo);

		//create Module for vertex and fragment shaders and initialize the variable. VkShaderModule is a pointer to a pointer to a shader module object
		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		VgeDevice& vgeDevice;	//stores device reference. A reference instead of a pointer to avoid dangling pointers
		VkPipeline graphicsPipeline;	//handle to vulkan pipeline object
		VkShaderModule vertShadermodule;
		VkShaderModule fragShadermodule;

	};
}