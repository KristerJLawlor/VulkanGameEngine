#include "vge_model.hpp"

//std lib
#include <cassert>
#include <cstring>

namespace vge {

	VgeModel::VgeModel(VgeDevice &device, const std::vector<Vertex> &vertices) : vgeDevice{ device } 
	{
		createVertexBuffers(vertices); //create vertex buffers from the vertices passed in
	}

	VgeModel::~VgeModel() {
		vkDestroyBuffer(vgeDevice.device(), vertexBuffer, nullptr);
		vkFreeMemory(vgeDevice.device(), vertexBufferMemory, nullptr);
	}


	//Vertex Buffer Creation
	//A vertex buffer is a block of GPU memory that contains vertex data, which is input to the vertex shader.
	//This is done by attaching the buffer to a vertex array object, then binding the vertex array object before making a draw call
	void VgeModel::createVertexBuffers(const std::vector<Vertex> &vertices) 
	{
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count must be at least 3");
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount; //make the size equal to the number of bytes that the vertices take up in memory

		//helper function in vge_device.cpp
		vgeDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,		//host = cpu, device = gpu
			vertexBuffer,
			vertexBufferMemory);

		void *data;
		vkMapMemory(vgeDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);	//creates region of host memory that is mapped to the device memory and sets data to point to the beginning of the mapped memory range
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));		//copies the vertices data to the host mapped memory. This automatically updates the device memory as well, since we used VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		vkUnmapMemory(vgeDevice.device(), vertexBufferMemory);
	}

	void VgeModel::draw(VkCommandBuffer commandBuffer) 
	{
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}

	//Binding the vertex buffer means that we are telling Vulkan which vertex buffer to use for the draw call.
	//Essentially means we are setting the buffer as "active".
	//If using multiple vertex buffers, you will need to bind them in order to operate on them.
	void VgeModel::bind(VkCommandBuffer commandBuffer) 
	{
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	std::vector<VkVertexInputBindingDescription> VgeModel::Vertex::getBindingDescriptions() 
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription> VgeModel::Vertex::getAttributeDescriptions() 
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);
		return attributeDescriptions;
	}

}