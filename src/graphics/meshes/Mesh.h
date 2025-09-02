#pragma once
#include <vector>
#include <string>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementArrayBuffer.h"

class Mesh {
public:
    
    Mesh(const std::vector<float>& vertices,
        const std::vector<float>& texCoords,
        const std::vector<float>& normals,
        const std::vector<unsigned int>& indices);

    ~Mesh() = default;

    void Bind() const;
    void Unbind() const;
    void Render() const;

    static Mesh LoadMeshFromFile(std::string Path); // only takes in .OBJ 
private:
    unsigned int m_indexCount;
    VertexArray m_VAO;
    VertexBuffer m_VBOPos;
    VertexBuffer m_VBOTexCoords;
    VertexBuffer m_VBONorm;
    ElementArrayBuffer m_EBO;
};