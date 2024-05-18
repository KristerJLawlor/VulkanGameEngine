#pragma once

#include "vge_device.hpp"

#include <string>
#include <vector>

namespace vge {

	//data used to configure pipeline
	struct PipelineConfigInfo {	};

	class VgePipeline {
	public:
		//pipeline constructor
		VgePipeline(VgeDevice& device, 
			const std::string& vertFilepath, 
			const std::string& fragFilepath, 
			const PipelineConfigInfo& configInfo);
		//destructor
		~VgePipeline() {};

		//delete to avoid duplicating pointers to vulkan objects
		VgePipeline(const VgePipeline&) = delete;
		void operator=(const VgePipeline&) = delete;

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