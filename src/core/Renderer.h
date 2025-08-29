#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "Skybox.h"
#include "Shape.h"
#include "Camera.h"
#include "Extras.h"
#include "DebugCrosshair.h"

class Renderer{
public:
	Renderer();
	~Renderer();

	void RenderSkybox(Skybox& skybox, Camera& cam, glm::mat4& proj);
	void RenderObjects(std::vector<Shape>& objects, std::vector<Light>& lights, Camera& cam, glm::mat4& proj, glm::mat4& lightSpaceMatrix);
	void RenderXhair(const DebugCrosshair& xhair, const Camera& cam, const glm::mat4& proj);
	void RenderShadows();

	void SetRenderShader(const ShaderProgram& shader);

private:
	ShaderProgram m_currentShader = ShaderProgram::UnlitShader;
	ShaderManger m_ShaderManager = ShaderManger();
};
