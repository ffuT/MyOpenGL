#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Shape.h"
#include "Camera.h"

class Renderer{

public:
	Renderer();
	~Renderer();

	void RenderObjects(std::vector<Shape*> objects, Camera cam, glm::mat4 proj, glm::vec3 lightpos, glm::vec3 lightcol);

private:
	ShaderManger m_ShaderManager = ShaderManger();
};
