#include "ShaderManager.h"
#include "Shader.h"

ShaderManger::ShaderManger() {

}

ShaderManger::~ShaderManger(){

}

Shader* ShaderManger::GetShader(ShaderProgram ShaderName){
	switch (ShaderName){
		case ShaderProgram::SkyboxShader:
			return &m_SkyboxShader;

		case ShaderProgram::NewShader:
			return &m_NewShader;
	
		case ShaderProgram::BasicShader:
			return &m_BasicShader;

		case ShaderProgram::UnlitShader:
			return &m_UnlitShader;
	}
}
