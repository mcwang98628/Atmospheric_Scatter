#pragma once

#include "Mesh.h"
#include "tiny_obj_loader.h"
#include "helper.h"

using namespace std;
#include <unordered_map>


Mesh::Mesh()
{
    //m_vBuffer = vBuffer;
    // m_pipeline = pipeline;
    // m_pipelineLayout = pipelineLayout;
    // m_descriptorSet = descriptorSet;
}

Mesh::~Mesh()
{
    // vkDestroyPipeline(VulkanControl::Get()->GetDevice(), m_pipeline, nullptr);

    delete m_vBuffer;
}

bool Mesh::LoadMesh(const std::string modelPath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str())) {
        throw std::runtime_error(warn + err);
    }
    vector<Vertex>  destVer;
    vector<uint32_t>  destIndices;

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.normal = { 1.0f, 1.0f, 1.0f };

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(destVer.size());
                destVer.push_back(vertex);
            }

            destIndices.push_back(uniqueVertices[vertex]);
        }
    }
    m_vBuffer = new VertexBuffer(destVer, destIndices);
    // std::cout << destVer.size() ;
}

void Mesh::Draw(VkCommandBuffer cmd) const
{
    // vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    // vkCmdBindDescriptorSets(
    //   cmd,
    //   VK_PIPELINE_BIND_POINT_GRAPHICS,
    //   m_pipelineLayout,
    //   0, 1,
    //   &m_descriptorSet,
    //   0, nullptr
    // );
    m_vBuffer->Draw(cmd);
}

