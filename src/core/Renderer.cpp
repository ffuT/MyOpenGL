#include "Renderer.h"
#include <cstring>

Renderer::Renderer() {
}

Renderer::~Renderer(){
}

void Renderer::RenderShadowMap(std::vector<Shape>& objects, std::vector<Light>& lights, Camera& cam) {
    glViewport(0, 0, m_shadowMap.getHeight(), m_shadowMap.getWidth());
    m_shadowMap.Bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    Shader* currentShader = m_ShaderManager.GetShader(ShadowShader);
    currentShader->Bind();
	Light directional = lights[0]; // only first light used for shadowmap
	updateLightSpaceMatrix(directional, cam);

    currentShader->SetUniformMat4f("u_lightProjection", m_lightSpaceMatrix);
	for (size_t i = 1; i < objects.size(); i++) { // start at 1 to skip light sphere at index 0
        currentShader->SetUniformMat4f("u_model", objects[i].GetModelMatrix());
        objects[i].Render();
    }

    currentShader->UnBind();
    glCullFace(GL_BACK);
}

void Renderer::RenderSkybox(Skybox& skybox, Camera& cam, glm::mat4& proj, const Light& dir) {
	glDepthFunc(GL_LEQUAL);
	skybox.Bind();
    
	Shader* currentShader = m_ShaderManager.GetShader(SkyboxShader);
	currentShader->Bind();
	currentShader->SetUniformMat4f("u_view", glm::mat4(glm::mat3(cam.GetViewMatrix())));
	currentShader->SetUniformMat4f("u_proj", proj);
    currentShader->SetUniform3f("u_sunDir", dir.position);
    float sunHeight = glm::clamp(dir.position.y * 0.5f + 0.5f, 0.0f, 1.0f);
    float blend = sunHeight * sunHeight; // smooth transition
	currentShader->SetUniform1f("u_blend", blend);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	currentShader->UnBind();
	skybox.UnBind();
}

void Renderer::RenderObjects(std::vector<Shape>& objects, std::vector<Light>& lights, Camera& cam, glm::mat4& proj) {
    glDepthFunc(GL_LESS);

	const glm::mat4 view = cam.GetViewMatrix(); // cache cus used multiple times

    /* Rendering Lights - using unlit shader for full brightness */
    Shader* lightShader = m_ShaderManager.GetShader(ShaderProgram::UnlitShader);
    lightShader->Bind();
    lightShader->SetUniformMat4f("u_view", view);
    lightShader->SetUniformMat4f("u_proj", proj);

    for (const Light& light : lights) { // set per light specific uniforms
        if (light.type > 0)
			continue; // skip non-point lights, since they arent rendered
        objects[0].SetPosition(light.position);
        lightShader->SetUniformMat4f("u_model", objects[0].GetModelMatrix());
        lightShader->SetUniform4f("u_color", glm::vec4(light.color, 1.0f));
        objects[0].Render();
    }
    lightShader->UnBind();
    
    /* Render Objects - using the selected shader */
    Shader* objectShader = m_ShaderManager.GetShader(m_currentShader);
    objectShader->Bind();
    objectShader->SetUniformMat4f("u_view", view);
    objectShader->SetUniformMat4f("u_proj", proj);

    switch (m_currentShader) { // set currentshader specific uniforms
    case NewShader:
        objectShader->SetUniformMat4f("u_lightSpace", m_lightSpaceMatrix);
        objectShader->SetUniform3f("u_viewPos", cam.GetPos());
        objectShader->SetUniform1i("numLights", lights.size());
        for (size_t j = 0; j < lights.size(); j++) {
            std::string lightName = "lights[" + std::to_string(j) + "].";
            objectShader->SetUniform3f(lightName + "position", lights[j].position);
            objectShader->SetUniform3f(lightName + "color", lights[j].color);
            objectShader->SetUniform1f(lightName + "intensity", lights[j].intensity);
            objectShader->SetUniform1i(lightName + "type", lights[j].type);
        }
        break;
        //more shaders in future
    }

    for (size_t i = 1; i < objects.size(); i++) { // set per object uniforms
        objectShader->SetUniformMat4f("u_model", objects[i].GetModelMatrix());

        switch (m_currentShader) {
        case NewShader:
            objectShader->SetUniform4f("u_color", objects[i].GetColor());
            objectShader->SetUniform1f("u_specularStrength", objects[i].GetSpecular());
            break;
        case UnlitShader:
        case WireframeShader:
            objectShader->SetUniform4f("u_color", objects[i].GetColor());
            break;
        }
        objects[i].Render();
    }
    objectShader->UnBind();
}

void Renderer::RenderCrosshair(const DebugCrosshair& xhair, const Camera& cam, const glm::mat4& proj) {
	glDisable(GL_DEPTH_TEST); // xhair rendered on top
	xhair.BindVAO();
	Shader* currentshader = m_ShaderManager.GetShader(ShaderProgram::CrosshairShader);
	currentshader->Bind();
	currentshader->SetUniformMat4f("u_view", cam.GetViewMatrix());
	currentshader->SetUniformMat4f("u_proj", proj);

	glLineWidth(2.0f);
	// X-axis (red)
	currentshader->SetUniform3f("u_lineColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_LINES, 0, 2);
	// Y-axis (green)
	currentshader->SetUniform3f("u_lineColor", glm::vec3(0.0f, 1.0f, 0.0f));
	glDrawArrays(GL_LINES, 2, 2);
	// Z-axis (blue)
	currentshader->SetUniform3f("u_lineColor", glm::vec3(0.0f, 0.33f, 1.0f));
	glDrawArrays(GL_LINES, 4, 2);
	currentshader->UnBind();
	xhair.UnBindVAO();
	glEnable(GL_DEPTH_TEST);
}

void Renderer::RenderTerrain(const Mesh& quad, const glm::mat4& model, const Camera& cam, const glm::mat4& proj, std::vector<Light>& lights) {
	Shader* currentshader = m_ShaderManager.GetShader(m_currentShader);
	currentshader->Bind();

    switch (m_currentShader) { // set currentshader specific uniforms
    case NewShader:
        currentshader->SetUniformMat4f("u_lightSpace", m_lightSpaceMatrix);
        currentshader->SetUniform3f("u_viewPos", cam.GetPos());
        currentshader->SetUniform1i("numLights", lights.size());
        for (size_t j = 0; j < lights.size(); j++) {
            std::string lightName = "lights[" + std::to_string(j) + "].";
            currentshader->SetUniform3f(lightName + "position", lights[j].position);
            currentshader->SetUniform3f(lightName + "color", lights[j].color);
            currentshader->SetUniform1f(lightName + "intensity", lights[j].intensity);
            currentshader->SetUniform1i(lightName + "type", lights[j].type);
        }
        break;
    }
	currentshader->SetUniformMat4f("u_model", model);
    currentshader->SetUniformMat4f("u_view", cam.GetViewMatrix());
    currentshader->SetUniformMat4f("u_proj", proj);
    currentshader->SetUniform4f("u_color", glm::vec4(1.0));
	quad.Bind();
	quad.Render();
	quad.Unbind();
    currentshader->UnBind();
}

void Renderer::RenderBufferToScreen(VertexArray& vao) {
    glDisable(GL_DEPTH_TEST);
	Shader* shader = m_ShaderManager.GetShader(ShaderProgram::BasicShader);
    
    shader->Bind();
	vao.Bind();
    m_shadowMap.getShadowTexture().Bind();
    shader->SetUniform1i("u_depthMap", 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	vao.Unbind();
	shader->UnBind();
    glEnable(GL_DEPTH_TEST);
}

void Renderer::SetRenderShader(const ShaderProgram& shader){
	m_currentShader = shader;
}

void Renderer::updateLightSpaceMatrix(Light light, Camera& cam) {
	/**** TODO make the orthographic projection follow the camera
    * 
    glm::vec3 campos = cam.GetPos();
    glm::vec3 pos = campos + light.position * 0.5f; // shadowmap following camera
    m_lightView = glm::lookAt(pos, campos, glm::vec3(0.0f, 1.0f, 0.0f));
    */
    m_lightView = glm::lookAt(light.position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_lightSpaceMatrix = m_orthographicProjection * m_lightView;
}