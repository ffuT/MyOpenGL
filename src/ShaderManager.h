#pragma once
#include "Shader.h"

//temp shader stuff
enum ShaderProgram {
    SkyboxShader,
    NewShader,
    BasicShader,
    UnlitShader
};

class ShaderManger {
private:
    Shader m_SkyboxShader = Shader("res/shaders/Skybox.shader");
    Shader m_NewShader = Shader("res/shaders/NewShader.shader");
    Shader m_BasicShader = Shader("res/shaders/BasicShader.shader");
    Shader m_UnlitShader = Shader("res/shaders/UnlitShader.shader");

public:
    ShaderManger();
    ~ShaderManger();

    Shader* GetShader(ShaderProgram ShaderName);
};
