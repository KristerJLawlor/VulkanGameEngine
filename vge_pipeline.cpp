#include "vge_pipeline.hpp"

//std library
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace vge {

	//pipeline constructor
	VgePipeline::VgePipeline(const std::string& vertFilepath, const std::string& fragFilepath) {
		createGraphicsPipeline(vertFilepath, fragFilepath);
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
		const std::string& vertFilepath, const std::string& fragFilepath) {
			
		auto vertCode = readFile(vertFilepath);
		auto fragCode = readFile(fragFilepath);

		std::cout << "vertex shader code size: " << vertCode.size() << std::endl;
		std::cout << "fragment shader code size: " << fragCode.size() << std::endl;
	}


}