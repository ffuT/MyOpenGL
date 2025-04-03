#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Shape.h"
#include "Camera.h"
#include "Extras.h"

class Renderer{
public:
	Renderer();
	~Renderer();

	void RenderObjects(std::vector<Shape*>& objects, std::vector<Light>& lights, Camera& cam, glm::mat4& proj);
	void RenderXhair(const VertexArray &vao, const Camera& cam, const glm::mat4& proj);

private:
	ShaderManger m_ShaderManager = ShaderManger();
};
