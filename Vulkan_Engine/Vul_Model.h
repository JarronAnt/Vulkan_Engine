#pragma once
#include "Vul_Device.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>

namespace vul {

	class VulModel
	{

	public:

		struct Vertex {
			glm::vec2 pos;
			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions(); 
		};



		VulModel(VulDevice& device, const std::vector<Vertex>& verts);
		~VulModel();

		//delete copy constructor 
		VulModel(const VulModel&) = delete;
		VulModel& operator=(const VulModel&) = delete;

		void bind(VkCommandBuffer cmdBuffer);
		void draw(VkCommandBuffer cmdBuffer);

	private:

		void createVertexBuffers(const std::vector<Vertex> &verts);
		VulDevice& vulDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;


	};

}