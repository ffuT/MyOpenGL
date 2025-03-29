#include "Renderer.h"
#include <iostream>

Renderer::Renderer() {
}

Renderer::~Renderer(){
}

void Renderer::RenderObjects(std::vector<Shape*>& objects, Camera& cam, glm::mat4& proj, glm::vec3& lightpos, glm::vec3& lightcol) {
	glDepthFunc(GL_LESS);
	for (Shape* obj: objects){
		Shader* currentshader = m_ShaderManager.GetShader(obj->GetShader());
		currentshader->Bind();
		currentshader->SetUniformMat4f("u_view", cam.GetViewMatrix());
		currentshader->SetUniformMat4f("u_proj", proj);
		currentshader->SetUniformMat4f("u_model", obj->GetModelMatrix());
		switch (obj->GetShaderName()){
			case NewShader:
				currentshader->SetUniform3f("u_Color", obj->GetColor());
				currentshader->SetUniform1f("u_ambientStrength", 0.01f);
				currentshader->SetUniform1f("u_specularStrength", obj->GetSpecular());
				currentshader->SetUniform3f("u_lightPos", lightpos);
				currentshader->SetUniform3f("u_viewPos", cam.GetPos());
				currentshader->SetUniform3f("u_lightColor", lightcol);
				break;
			case UnlitShader:
				currentshader->SetUniform3f("u_Color", obj->GetColor());
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
