#include "Renderer.h"
#include <cstring>

Renderer::Renderer() {
}

Renderer::~Renderer(){
}

void Renderer::RenderSkybox(Skybox& skybox, Camera& cam, glm::mat4& proj) {
	glDepthFunc(GL_LEQUAL);
	skybox.Bind();
	Shader* currentShader = m_ShaderManager.GetShader(SkyboxShader);
	currentShader->Bind();
	currentShader->SetUniformMat4f("u_view", glm::mat4(glm::mat3(cam.GetViewMatrix())));
	currentShader->SetUniformMat4f("u_proj", proj);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	currentShader->UnBind();
	skybox.UnBind();
}

void Renderer::RenderObjects(std::vector<Shape*>& objects, std::vector<Light>& lights, Camera& cam, glm::mat4& proj) {
    glDepthFunc(GL_LESS);

    //light render
    Shader* lightShader = m_ShaderManager.GetShader(ShaderProgram::UnlitShader);
    lightShader->Bind();
    lightShader->SetUniformMat4f("u_view", cam.GetViewMatrix());
    lightShader->SetUniformMat4f("u_proj", proj);

    for (Light& light : lights) {
        objects[0]->SetTransform(glm::translate(glm::mat4(1.0f), light.position));
        lightShader->SetUniformMat4f("u_model", objects[0]->GetModelMatrix());
        lightShader->SetUniform4f("u_color", glm::vec4(light.color, 1.0f));
        objects[0]->Render();
    }
    lightShader->UnBind();

    //object render
    Shader* objectShader = m_ShaderManager.GetShader(m_currentShader);
    objectShader->Bind();
    objectShader->SetUniformMat4f("u_view", cam.GetViewMatrix());
    objectShader->SetUniformMat4f("u_proj", proj);

    switch (m_currentShader) {
    case NewShader:
        objectShader->SetUniform3f("u_viewPos", cam.GetPos());
        objectShader->SetUniform1i("numLights", lights.size());
        for (int j = 0; j < lights.size(); j++) {
            std::string lightName = "lights[" + std::to_string(j) + "].";
            objectShader->SetUniform3f(lightName + "position", lights[j].position);
            objectShader->SetUniform3f(lightName + "color", lights[j].color);
            objectShader->SetUniform1f(lightName + "intensity", lights[j].intensity);
        }
        break;
    }

    for (int i = 1; i < objects.size(); i++) {
        objectShader->SetUniformMat4f("u_model", objects[i]->GetModelMatrix());

        switch (m_currentShader) {
        case NewShader:
            objectShader->SetUniform4f("u_color", objects[i]->GetColor());
            objectShader->SetUniform1f("u_specularStrength", objects[i]->GetSpecular());
            break;
        case UnlitShader:
        case WireframeShader:
            objectShader->SetUniform4f("u_color", objects[i]->GetColor());
            break;
        }
        objects[i]->Render();
    }
    objectShader->UnBind();
}


void Renderer::RenderXhair(const VertexArray &vao, const Camera& cam, const glm::mat4& proj) {
	glDisable(GL_DEPTH_TEST);
	vao.Bind();
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
	vao.Unbind();
	glEnable(GL_DEPTH_TEST);
}

void Renderer::SetRenderShader(const ShaderProgram& shader){
	m_currentShader = shader;
}
