#pragma once
#include <vector>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementArrayBuffer.h"

class Mesh {
public:
    Mesh(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<unsigned int>& indices);
    void Bind() const;
    void Unbind() const;
    void Render() const;

private:
    VertexArray m_VAO;
    VertexBuffer m_VBOPos;
    VertexBuffer m_VBONorm;
    ElementArrayBuffer m_EBO;
    unsigned int m_indexCount;
};