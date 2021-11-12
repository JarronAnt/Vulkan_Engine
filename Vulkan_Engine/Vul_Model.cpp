#include "Vul_Model.h"
#include <cassert>
namespace vul {


	VulModel::VulModel(VulDevice& device, const std::vector<Vertex>& verts) : vulDevice{device}
	{
		createVertexBuffers(verts);
	}

	VulModel::~VulModel()
	{
		vkDestroyBuffer(vulDevice.device(), vertexBuffer, nullptr); 
		vkFreeMemory(vulDevice.device(), vertexBufferMemory, nullptr);
	}
	void VulModel::bind(VkCommandBuffer cmdBuffer)
	{
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(cmdBuffer, 0, 1, buffers, offsets);

	}
	void VulModel::draw(VkCommandBuffer cmdBuffer)
	{
		vkCmdDraw(cmdBuffer, vertexCount, 1, 0, 0);
	}
	void VulModel::createVertexBuffers(const std::vector<Vertex>& verts)
	{
		vertexCount = static_cast<uint32_t>(verts.size());
		assert(vertexCount >= 3);
		VkDeviceSize bufferSize = sizeof(verts[0]) * vertexCount;
		vulDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory);

		void* data;
		vkMapMemory(vulDevice.device(), vertexBufferMemory, 0, bufferSize, 0,&data);

		memcpy(data, verts.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(vulDevice.device(), vertexBufferMemory);

		
	}
	std::vector<VkVertexInputBindingDescription> VulModel::Vertex::getBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescriptions;
	}
	std::vector<VkVertexInputAttributeDescription> VulModel::Vertex::getAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = 0;
		return attributeDescriptions;
	}
}