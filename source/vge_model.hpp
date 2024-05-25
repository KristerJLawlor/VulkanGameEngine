#pragma once

#include "vge_device.hpp"

//external libraries
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

//std lib
#include <vector>

namespace vge {
	
	//The purpose of this class is to be able to take vertex data created by or read in a file
	//on the cpu, then allocate the memory and copy the data over to our gpu device
	//such that it can be rendered efficiently
	class VgeModel {
		
		public:

			struct Vertex {
				glm::vec2 position;
				glm::vec3 color;


				static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
				static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
			};
			
			VgeModel(VgeDevice &device, const std::vector<Vertex> &vertices);
			~VgeModel();

			VgeModel(const VgeModel &) = delete;
			VgeModel &operator=(const VgeModel&) = delete;

			void bind(VkCommandBuffer commandBuffer);
			void draw(VkCommandBuffer commandBuffer);

		private:

			void createVertexBuffers(const std::vector<Vertex> &vertices);

			VgeDevice &vgeDevice;
			VkBuffer vertexBuffer;
			VkDeviceMemory vertexBufferMemory;
			uint32_t vertexCount;		
	};
}