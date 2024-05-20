#include "vge_pipeline.hpp"

//std library
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace vge {

	//graphics pipeline constructor
	VgePipeline::VgePipeline(
		VgeDevice& device,
		const std::string& vertFilepath,
		const std::string& fragFilepath,
		const PipelineConfigInfo& configInfo) : vgeDevice{ device } {

			createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
	}

	std::vector<char> VgePipeline::readFile(const std::string& filepath) {
		//make input file stream object to read file
		std::ifstream file{ filepath, std::ios::ate | std::ios::binary };

		//check if file was successfully opened
		if (!file.is_open()) {
			throw std::runtime_error("failed to open file: " + filepath);	//throw error if file failed to open
		}

		size_t fileSize = static_cast<size_t>(file.tellg()); //get file size
		std::vector<char> buffer(fileSize); //character buffer array with size of file

		file.seekg(0);	//start back at the beginning of file
		file.read(buffer.data(), fileSize); //read data into buffer

		file.close();	//close file and return
		return buffer;
	}

	void VgePipeline::createGraphicsPipeline(
		const std::string& vertFilepath, 
		const std::string& fragFilepath,
		const PipelineConfigInfo& configInfo) {
			
		auto vertCode = readFile(vertFilepath);
		auto fragCode = readFile(fragFilepath);

		std::cout << "vertex shader code size: " << vertCode.size() << std::endl;
		std::cout << "fragment shader code size: " << fragCode.size() << std::endl;
	}

	void VgePipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) {

		//create struct to hold paramters
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;	//struct type
		createInfo.codeSize = code.size();	//size of vector array
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());	//pointer to the code. needs to be cast to uint32_t, since vulkan expects a uint32_t array instead of a char array
		
		//create shader module by passing pointer of createInfo as a parameter
		if (vkCreateShaderModule(vgeDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module");
		}
	}

	PipelineConfigInfo VgePipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height) {
		PipelineConfigInfo configInfo{};

		return configInfo;
	}
}