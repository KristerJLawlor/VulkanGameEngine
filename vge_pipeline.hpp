#pragma once

#include <string>
#include <vector>

namespace vge {
	class VgePipeline {
	public:
		//pipeline constructor
		VgePipeline(const std::string& vertFilepath, const std::string& fragFilepath);

	private:
		//function to read file
		static std::vector<char> readFile(const std::string& filepath);

		//helper function
		void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath);
	};
}