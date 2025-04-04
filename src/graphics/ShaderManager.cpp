#include "ShaderManager.h"
#include "Shader.h"

ShaderManger::ShaderManger() {

}

ShaderManger::~ShaderManger(){

}

Shader* ShaderManger::GetShader(const ShaderProgram& ShaderName) {
	switch (ShaderName){
		case ShaderProgram::SkyboxShader:
			return &m_SkyboxShader;

		case ShaderProgram::NewShader:
			return &m_NewShader;
	
		case ShaderProgram::UnlitShader:
			return &m_UnlitShader;

		case ShaderProgram::CrosshairShader:
			return &m_CrosshairShader;
		case ShaderProgram::WireframeShader:
			return &m_WireframeShader;
	}
}
