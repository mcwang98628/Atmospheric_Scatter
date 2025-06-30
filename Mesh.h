#pragma once

#include "VertexBuffer.h"

class Mesh {
public:
    Mesh();
    ~Mesh();

    bool LoadMesh(const std::string modelPath);

    void Draw(VkCommandBuffer cmd) const;

private:
    VertexBuffer* m_vBuffer;

};