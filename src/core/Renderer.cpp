#include "Renderer.h"
#include <cstring>

Renderer::Renderer() {
}

Renderer::~Renderer(){
}

void Renderer::RenderSkybox(Skybox& skybox, Camera& cam, glm::mat4& proj){
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
	for (Shape* obj: objects){
		Shader* currentshader = m_ShaderManager.GetShader(obj->GetShader());
		currentshader->Bind();
		currentshader->SetUniformMat4f("u_view", cam.GetViewMatrix());
		currentshader->SetUniformMat4f("u_proj", proj);
		currentshader->SetUniformMat4f("u_model", obj->GetModelMatrix());
		switch (obj->GetShaderName()){
			case NewShader:
				currentshader->SetUniform4f("u_color", obj->GetColor());
				currentshader->SetUniform1f("u_specularStrength", obj->GetSpecular());
				currentshader->SetUniform3f("u_viewPos", cam.GetPos());
				currentshader->SetUniform1i("numLights", lights.size());
				for (int i = 0; i < lights.size(); i++) {
					std::string lightName = "lights[" + std::to_string(i) + "].";
					currentshader->SetUniform3f(lightName + "position", lights[i].position);
					currentshader->SetUniform3f(lightName + "color", lights[i].color);
					currentshader->SetUniform1f(lightName + "intensity", lights[i].intensity);
				}
				break;
			case UnlitShader:
				if (strcmp(obj->GetTextID(), "PointLight") == 0) {	
					for (int i = 0; i < lights.size(); i++) {	//very inefficient
						obj->SetTransform(glm::translate(glm::mat4(1.0), lights[i].position));
						currentshader->SetUniformMat4f("u_model", obj->GetModelMatrix());
						currentshader->SetUniform4f("u_color", glm::vec4(lights[i].color, 1.0));
						obj->Render();
					}
				} else {
					currentshader->SetUniform3f("u_color", obj->GetColor());
				}
				break;
		}
		obj->Render();
		currentshader->UnBind();
	}
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
