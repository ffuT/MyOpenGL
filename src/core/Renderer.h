#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "ShaderManager.h"
#include "Skybox.h"
#include "Shape.h"
#include "Camera.h"
#include "Extras.h"
#include "DebugCrosshair.h"
#include "ShadowMap.h"

class Renderer{
public:
	Renderer();
	~Renderer();

	void RenderShadowMap(std::vector<Shape>& objects, std::vector<Light>& lights);
	void RenderSkybox(Skybox& skybox, Camera& cam, glm::mat4& proj);
	void RenderObjects(std::vector<Shape>& objects, std::vector<Light>& lights, Camera& cam, glm::mat4& proj);
	void RenderCrosshair(const DebugCrosshair& xhair, const Camera& cam, const glm::mat4& proj);

	void SetRenderShader(const ShaderProgram& shader);

private:

	// shadow map 
	unsigned int shadowMapWidth = 4096, shadowMapHeight = shadowMapWidth;
	ShadowMap m_shadowMap = ShadowMap(shadowMapWidth, shadowMapHeight);
	// shadow map matrices
	glm::mat4 m_orthographicProjection = glm::ortho(-400.0f, 400.0f, -400.0f, 400.0f, -500.0f, 1000.0f);
	glm::mat4 m_lightView = glm::mat4(1.0);
	glm::mat4 m_lightSpaceMatrix = glm::mat4(1.0);

	ShaderProgram m_currentShader = ShaderProgram::UnlitShader;
	ShaderManger m_ShaderManager = ShaderManger();	
};
