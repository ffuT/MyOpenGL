#include "Renderer.h"

Renderer::Renderer() {
}

Renderer::~Renderer(){
}

void Renderer::RenderObjects(std::vector<Shape*>& objects, Camera& cam, glm::mat4& proj, glm::vec3& lightpos, glm::vec3& lightcol) {
	glDepthFunc(GL_LESS);
	for (Shape* obj: objects){
		Shader* shader = m_ShaderManager.GetShader(obj->GetShader());
		shader->Bind();
		shader->SetUniformMat4f("u_view", cam.GetViewMatrix());
		shader->SetUniformMat4f("u_proj", proj);
		shader->SetUniformMat4f("u_model", obj->GetModelMatrix());
		switch (obj->GetShaderName()){
			case NewShader:
				shader->SetUniform3f("u_Color", obj->GetColor());
				shader->SetUniform1f("u_ambientStrength", 0.01f);
				shader->SetUniform1f("u_specularStrength", obj->GetSpecular());
				shader->SetUniform3f("u_lightPos", lightpos);
				shader->SetUniform3f("u_viewPos", cam.GetPos());
				shader->SetUniform3f("u_lightColor", lightcol);
				break;
			case UnlitShader:
				shader->SetUniform3f("u_Color", obj->GetColor());
				break;
		}
		obj->Render();
		shader->UnBind();
	}
}
