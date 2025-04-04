#pragma once
#include "Shader.h"

//temp shader stuff
enum ShaderProgram {
    SkyboxShader,
    NewShader,
    BasicShader,
    UnlitShader,
    CrosshairShader,
    WireframeShader
};

class ShaderManger {
public:
    ShaderManger();
    ~ShaderManger();

    Shader* GetShader(const ShaderProgram& ShaderName);

private:
    Shader m_SkyboxShader = Shader("res/shaders/Skybox.shader");
    Shader m_NewShader = Shader("res/shaders/NewShader2.shader");
    Shader m_BasicShader = Shader("res/shaders/BasicShader.shader");
    Shader m_UnlitShader = Shader("res/shaders/UnlitShader.shader");
    Shader m_CrosshairShader = Shader("res/shaders/Crosshair.shader");
    Shader m_WireframeShader = Shader("res/shaders/Wireframeshader.shader");
};
