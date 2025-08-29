#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <tuple>

Mesh::Mesh(const std::vector<float>& vertices,
	const std::vector<float>& texCoords,
	const std::vector<float>& normals,
	const std::vector<unsigned int>& indices) :
	m_VBOPos(vertices.size() * sizeof(float), vertices.data()),
	m_VBOTexCoords(texCoords.size() * sizeof(float), texCoords.data()),
	m_VBONorm(normals.size() * sizeof(float), normals.data()),
	m_EBO(indices.size() * sizeof(unsigned int), indices.data()),
	m_indexCount(indices.size()) {

	m_VAO.Bind();
	m_VAO.AddVertexBuffer(m_VBOPos, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	m_VAO.AddVertexBuffer(m_VBOTexCoords, 1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	m_VAO.AddVertexBuffer(m_VBONorm, 2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	m_VAO.BindElementArrayBuffer(m_EBO);

	m_VAO.Unbind();
    m_EBO.Unbind();
}

void Mesh::Bind() const{
	m_VAO.Bind();
}

void Mesh::Unbind() const{
	m_VAO.Unbind();
}

void Mesh::Render() const{
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}

struct VertexKey {
    int v, vt, vn;
    bool operator==(const VertexKey& other) const {
        return v == other.v && vt == other.vt && vn == other.vn;
    }
};

template <>
struct std::hash<VertexKey> {
    size_t operator()(const VertexKey& k) const {
        return ((std::hash<int>()(k.v) ^
            (std::hash<int>()(k.vt) << 1)) >> 1) ^
            (std::hash<int>()(k.vn) << 1);
    }
};

Mesh Mesh::LoadMeshFromFile(std::string Path) {
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_texCoords;
    std::vector<glm::vec3> temp_normals;

    std::vector<float> vertices, texCoords, normals;
    std::vector<unsigned int> indices;

    std::unordered_map<VertexKey, unsigned int> uniqueVertices;

    std::ifstream file(Path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + Path);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            temp_positions.push_back({ x, y, z });
        }
        else if (prefix == "vt") {
            float u, v;
            iss >> u >> v;
            temp_texCoords.push_back({ u, v });
        }
        else if (prefix == "vn") {
            float nx, ny, nz;
            iss >> nx >> ny >> nz;
            temp_normals.push_back({ nx, ny, nz });
        }
        else if (prefix == "f") {
            std::string vertexStr;
            while (iss >> vertexStr) {
                int v = 0, vt = 0, vn = 0;

                size_t pos1 = vertexStr.find('/');
                size_t pos2 = vertexStr.find_last_of('/');

                if (pos1 == std::string::npos) {
                    v = std::stoi(vertexStr);
                } else if (pos1 == pos2) {
                    v = std::stoi(vertexStr.substr(0, pos1));
                    std::string after = vertexStr.substr(pos1 + 1);
                    if (!after.empty()) {
                        if (after.find('/') == std::string::npos)
                            vt = std::stoi(after);
                        else
                            vn = std::stoi(after);
                    }
                } else {
                    v = std::stoi(vertexStr.substr(0, pos1));
                    vt = std::stoi(vertexStr.substr(pos1 + 1, pos2 - pos1 - 1));
                    vn = std::stoi(vertexStr.substr(pos2 + 1));
                }

                VertexKey key{ v, vt, vn };

                if (uniqueVertices.count(key) == 0) {
                    uniqueVertices[key] = static_cast<unsigned int>(vertices.size() / 3);
                    // Add position
                    if (v > 0 && v <= (int)temp_positions.size()) {
                        vertices.push_back(temp_positions[v - 1].x);
                        vertices.push_back(temp_positions[v - 1].y);
                        vertices.push_back(temp_positions[v - 1].z);
                    } else {
                        vertices.push_back(0.f);
                        vertices.push_back(0.f);
                        vertices.push_back(0.f);
                    }
                    if (vt > 0 && vt <= (int)temp_texCoords.size()) {
                        texCoords.push_back(temp_texCoords[vt - 1].x);
                        texCoords.push_back(temp_texCoords[vt - 1].y);
                    } else {
                        texCoords.push_back(0.f);
                        texCoords.push_back(0.f);
                    }
                    // Add normal
                    if (vn > 0 && vn <= (int)temp_normals.size()) {
                        normals.push_back(temp_normals[vn - 1].x);
                        normals.push_back(temp_normals[vn - 1].y);
                        normals.push_back(temp_normals[vn - 1].z);
                    } else {
                        normals.push_back(0.f);
                        normals.push_back(0.f);
                        normals.push_back(0.f);
                    }
                }
                indices.push_back(uniqueVertices[key]);
            }
        }
    }
    file.close();
    return Mesh(vertices, texCoords, normals, indices);
}
