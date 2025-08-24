#include "vge_pipeline.hpp"

#include "vge_model.hpp"

//std library
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cassert>

namespace vge {

	//Will call the helper function and pass parameters to the graphics pipeline constructor
	VgePipeline::VgePipeline(
		VgeDevice& device,
		const std::string& vertFilepath,
		const std::string& fragFilepath,
		const PipelineConfigInfo& configInfo) : vgeDevice{ device } {

			createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
	}

	//Destructor for the VgePipeline class. It will destroy the shader modules and graphics pipeline
	VgePipeline::~VgePipeline() {
		vkDestroyShaderModule(vgeDevice.device(), vertShadermodule, nullptr);
		vkDestroyShaderModule(vgeDevice.device(), fragShadermodule, nullptr);
		vkDestroyPipeline(vgeDevice.device(), graphicsPipeline, nullptr);
	}

	//We need to read the compiled shader files from disk, so we can create the shader modules
	std::vector<char> VgePipeline::readFile(const std::string& filepath) {
		//make input file stream object to read compiled shader files
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
			const PipelineConfigInfo& configInfo) 
	{
			
		//help debug validation layers
		assert(configInfo.pipelineLayout != VK_NULL_HANDLE &&
			"Cannot create graphics pipeline:: no pipelineLayout provided in configInfo");
		assert(configInfo.renderPass != VK_NULL_HANDLE &&
			"Cannot create graphics pipeline:: no renderPass provided in configInfo");

		//create shader modules for vertex and fragment shaders
		auto vertCode = readFile(vertFilepath);
		auto fragCode = readFile(fragFilepath);

		//initialize the vertex and fragment shader modules
		createShaderModule(vertCode, &vertShadermodule);
		createShaderModule(fragCode, &fragShadermodule);

		//Create VkPipelineShaderStageCreateInfo array to hold structs for each shader stage
		VkPipelineShaderStageCreateInfo shaderStages[2];
		shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;	//specify stage 0 is for vertex shader
		shaderStages[0].module = vertShadermodule;
		shaderStages[0].pName = "main";	//name of the entry point function in the shader code
		shaderStages[0].flags = 0;
		shaderStages[0].pNext = nullptr;
		shaderStages[0].pSpecializationInfo = nullptr;
		shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;	//specify stage 1 is for fragment shader
		shaderStages[1].module = fragShadermodule;
		shaderStages[1].pName = "main";
		shaderStages[1].flags = 0;
		shaderStages[1].pNext = nullptr;
		shaderStages[1].pSpecializationInfo = nullptr;

		auto bindingDescriptions = VgeModel::Vertex::getBindingDescriptions();
		auto attributeDescriptions = VgeModel::Vertex::getAttributeDescriptions();

		//create vertex input state info struct. This struct describes how to interpret the vertex buffer data
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};	//struct used to describe how to interpret vertex buffer data
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexAttributeDescriptionCount =
			static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.vertexBindingDescriptionCount = 
			static_cast<uint32_t>(bindingDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
		vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

		//will use all above configuration to make the graphics pipeline object
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
		pipelineInfo.pViewportState = &configInfo.viewportInfo;
		pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
		pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;

		pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
		pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
		pipelineInfo.pDynamicState = &configInfo.dynamicStateInfo;

		pipelineInfo.layout = configInfo.pipelineLayout;
		pipelineInfo.renderPass = configInfo.renderPass;
		pipelineInfo.subpass = configInfo.subpass;

		//used to optimize performance by reusing existing pipelines
		pipelineInfo.basePipelineIndex = -1;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		//check if the graphics pipeline succeeded or not
		if (vkCreateGraphicsPipelines(
				vgeDevice.device(),
				VK_NULL_HANDLE, 
				1, 
				&pipelineInfo, 								
			    nullptr, 
				&graphicsPipeline)	!=  VK_SUCCESS) 
		{	throw std::runtime_error("Failed to create graphics pipeline");	}
		
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


	void VgePipeline::bind(VkCommandBuffer commandBuffer) {
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
	}


	//helper Function for the Fixed Function Pipeline stages
	//Fixed Function Pipeline stages are the stages that do not require shaders to be written, such as input assembly, rasterization, etc.
	void VgePipeline::defaultPipelineConfigInfo(PipelineConfigInfo &configInfo) 
	{

		//define the input assembly behavior 
		//input assembler takes list of vertices and groups them in to geometry
		configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;	//specifies how the list will be interpreted. In this case, it will form triangles with given vertices
		configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;	//tell the list to produce only connected triangle strip if false. True if desire separate triangles

		configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		configInfo.viewportInfo.viewportCount = 1;
		configInfo.viewportInfo.pViewports = nullptr;
		configInfo.viewportInfo.scissorCount = 1;
		configInfo.viewportInfo.pScissors = nullptr;

		//Rasterization stage
		configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		configInfo.rasterizationInfo.depthClampEnable = VK_FALSE; //keeps values within 0-1 range if true
		configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;//defines if we fill the geometry or not
		configInfo.rasterizationInfo.lineWidth = 1.0f;
		configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE; //defines what side of the geometry will be culled based on the perceived rotations of the geometry
		configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
		configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
		configInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
		configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

		//Multisampling config. Defines how the rasterization handles the edges of geometry
		//Multisampling manages aliasing, which causes jagged edges on lines that cross diagonally over pixels
		configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
		configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		configInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
		configInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
		configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
		configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

		//Color blending controls how to combine colors in the frame buffer
		configInfo.colorBlendAttachment.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT;
		configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
		configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
		configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

		configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
		configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
		configInfo.colorBlendInfo.attachmentCount = 1;
		configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
		configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
		configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
		configInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
		configInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

		//Depth buffer is an additional attachment to frame buffer that stores a value for every pixel's depth
		configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
		configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
		configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
		configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
		configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
		configInfo.depthStencilInfo.front = {};  // Optional
		configInfo.depthStencilInfo.back = {};   // Optional

		configInfo.dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		configInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		configInfo.dynamicStateInfo.pDynamicStates = configInfo.dynamicStateEnables.data();
		configInfo.dynamicStateInfo.dynamicStateCount =
			static_cast<uint32_t>(configInfo.dynamicStateEnables.size());
		configInfo.dynamicStateInfo.flags = 0;


	}
}