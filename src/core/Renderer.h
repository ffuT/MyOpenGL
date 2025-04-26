#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "Skybox.h"
#include "Shape.h"
#include "Camera.h"
#include "Extras.h"

class Renderer{
public:
	Renderer();
	~Renderer();

	void RenderSkybox(Skybox& skybox, Camera& cam, glm::mat4& proj);
	void RenderObjects(std::vector<Shape*>& objects, std::vector<Light*>& lights, Camera& cam, glm::mat4& proj);
	void RenderXhair(const VertexArray &vao, const Camera& cam, const glm::mat4& proj);

	void SetRenderShader(const ShaderProgram& shader);

private:
	ShaderProgram m_currentShader = ShaderProgram::UnlitShader;
	ShaderManger m_ShaderManager = ShaderManger();
};
